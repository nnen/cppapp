/**
 * \file   Injector.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-21
 *
 * \brief  Header file for the Injector class.
 */

#ifndef INJECTOR_D3Q01JMO
#define INJECTOR_D3Q01JMO


#include "Object.h"
#include "DynObject.h"
#include "Logger.h"
#include "Debug.h"


namespace cppapp {


/** \addtogroup di
 *
 * \brief Dependency injection
 *
 * @{
 */


class Injector;


//// DIObject ///////////////////////////////////////////////////////


class DIObject : public Object {
public:
	virtual ~DIObject() {} 
	
	virtual bool injectDependency(Ref<DIObject> obj, std::string key)
	{
		return false;
	}
};


template<class T>
class DIBox : public DIObject {
private:
	Ref<T> value_;

public:
	DIBox(Ref<T> value) : value_(value) {}
	virtual ~DIBox() { value_ = NULL; }
	
	Ref<T> getValue() { return value_; }
	
	virtual bool injectDependency(Ref<DIObject> obj, std::string key)
	{
		return value_->injectDependency(obj, key);
	}
};


//// DIFactory //////////////////////////////////////////////////////


class DIFactory : public Object {
public:
	virtual ~DIFactory() {}
	
	virtual Ref<DIObject> create(Ref<DynObject> config, Ref<DIObject> parent) = 0;
};


class DIFunctionFactory : public DIFactory {
public:
	typedef Ref<DIObject> (*Function)(Ref<DynObject> config, Ref<DIObject> parent);

private:
	Function function_;

public:
	DIFunctionFactory(Function function) :
		function_(function)
	{ }
	
	virtual ~DIFunctionFactory()
	{
		function_ = NULL;
	}
	
	virtual Ref<DIObject> create(Ref<DynObject> config, Ref<DIObject> parent)
	{
		return function_(config, parent);
	}
};


struct DIFunctionRegistration {
public:
	DIFunctionRegistration(std::string name, DIFunctionFactory::Function fn);
};


#define CPPAPP_DI_FUNCTION(name__, fn__) \
	DIFunctionRegistration DIFunctionRegistration__##fn__(name__, fn__);

#define CPPAPP_DI_METHOD(name__, cls__, fn__) \
	DIFunctionRegistration DIFunctionRegistration__##cls__##fn__(name__, cls__::fn__);


//// DIPlan /////////////////////////////////////////////////////////


class DIPlan : public Object {
private:
	bool                      hasKey_;
	std::string               key_;
	Ref<DIFactory>            factory_;
	std::vector<Ref<DIPlan> > children_;
	
	Ref<DynObject>            config_;

public:
	DIPlan(bool                             hasKey,
		  std::string                      key,
	       Ref<DIFactory>                   factory,
		  const std::vector<Ref<DIPlan> > &children,
		  Ref<DynObject>                   config) :
		hasKey_(hasKey),
		key_(key),
		factory_(factory),
		children_(children),
		config_(config)
	{}

	DIPlan(Ref<DIFactory>                   factory,
		  const std::vector<Ref<DIPlan> > &children,
		  Ref<DynObject>                   config) :
		hasKey_(false),
		key_(""),
		factory_(factory),
		children_(children),
		config_(config)
	{}
	
	DIPlan(std::string                      key,
		  Ref<DIFactory>                   factory,
		  const std::vector<Ref<DIPlan> > &children,
		  Ref<DynObject>                   config) :
		hasKey_(true),
		key_(key),
		factory_(factory),
		children_(children),
		config_(config)
	{}
	
	virtual ~DIPlan() {}
	
	virtual bool        hasKey() const { return hasKey_; }
	virtual std::string getKey() const { return key_; }
	
	virtual Ref<DIFactory> getFactory() const { return factory_; }
	
	virtual TextLoc getOrigin() { return config_->getLocation(); }
	
	virtual Ref<DIObject> instantiate(Ref<DIObject> parent);
};


//// Injector ///////////////////////////////////////////////////////


#define DI_KEY_CFG_KEY     "key"
#define DI_FACTORY_CFG_KEY "factory"


/**
 * \todo Write documentation for class Injector.
 */
class Injector {
private:
	std::map<std::string, Ref<DIFactory> > factories_;
	std::map<std::string, Ref<DIPlan> >    plans_;
	
	Injector(const Injector& other);
	
public:
	/**
	 * Constructor.
	 */
	Injector() {}
	
	/**
	 * Destructor.
	 */
	virtual ~Injector() {}
	
	void           registerFactory(std::string name, Ref<DIFactory> factory);
	Ref<DIFactory> getFactory(std::string name);
	
	Ref<DIPlan> makePlan(Ref<DynObject> config);
	Ref<DIPlan> getPlan(std::string name);
	void        makePlans(Ref<DynObject> config);
	
	Ref<DIObject> instantiate(std::string planName);
	
	template<class T>
	Ref<T> instantiateAs(std::string planName)
	{
		Ref<DIObject> obj = instantiate(planName);
		if (obj.isNull()) return NULL;
		Ref<T> result = obj.as<T>();
		if (result.isNull()) {
			Ref<DIPlan> plan = getPlan(planName);
			LOG_ERROR(
				"Could not dynamically cast a configured object (did you use the right factory?). "
				"Object's configuration is at " << plan->getOrigin() << "."
			);
		}
		return result;
	}
	
	void clear()
	{
		factories_.clear();
		plans_.clear();
	}
	
	static Injector& getInstance();
};


/** @} */


} // namespace cppapp


#endif /* end of include guard: INJECTOR_D3Q01JMO */

