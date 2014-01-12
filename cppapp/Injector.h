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


/**
 * \defgroup di Dependency Injection
 * \brief Simple dependency injection system.
 */
/** \addtogroup di
 * @{
 */


class Injector;


//// DIObject ///////////////////////////////////////////////////////


/**
 * \brief Base class for objects that can have dependencies injected
 *        or can be injected as a dependency.
 */
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


//// DIAdvanceObject ////////////////////////////////////////////////


class DIAbstractProperty : public Object {
protected:
	std::string name_;
	bool        required_;

public:
	DIAbstractProperty(std::string name, bool required) :
		name_(name), required_(required)
	{}
	
	DIAbstractProperty(std::string name) :
		name_(name), required_(false)
	{}
	
	virtual ~DIAbstractProperty() {}
	
	virtual std::string getName() { return name_; }
	virtual bool        isRequired() { return required_; }
	
	virtual bool set(Ref<DIObject> value) = 0;
};


template<class T, class U>
class DIProperty : public DIAbstractProperty {
public:
	typedef void (U::*Method)(T value);

private:
	std::string  name_;
	U           *obj_;
	Method       method_;

public:
	DIProperty(std::string name, U *obj, Method method) :
		name_(name), obj_(obj), method_(method)
	{}
	
	virtual ~DIProperty() {}
	
	virtual bool set(Ref<DIObject> value)
	{
		LOG_ERROR("Failed to set DI property \"" << name_ << "\".");
		return false;
	}
};


template<class T, class U>
class DIProperty<Ref<T>, U> : public DIAbstractProperty {
public:
	typedef void (U::*Method)(T value);

private:
	std::string  name_;
	U           *obj_;
	Method       method_;

public:
	DIProperty(std::string name, U obj, Method method) :
		name_(name), obj_(obj), method_(method)
	{}
	
	virtual ~DIProperty() {}
	
	virtual bool set(Ref<DIObject> value)
	{
		Ref<DynObject> obj = value.as<DynObject>();
		if (obj.isNull()) {
			LOG_ERROR(
				"Failed to set DI property \"" << name_ <<
				"\" - data type conversion failed. (" << value->getClassName() << ")."
			);
			return false;
		}
		
		(obj_->*method_)(*value);
		return true;
	}
};


class Injector;


class DIAdvancedObject : public DIObject {
private:
	std::map<std::string, Ref<DIAbstractProperty> > properties_;
	
protected:
	void addDIProperty(Ref<DIAbstractProperty> property);
	
public:
	virtual ~DIAdvancedObject() {}
	
	virtual bool injectDependency(Ref<DIObject> obj, std::string key);
	
	virtual void diConfig(Injector &injector, Ref<DynObject> config);
};


//// DIFactory //////////////////////////////////////////////////////


/**
 * \brief Base class for factory objects that are used instantiate dependencies.
 */
class DIFactory : public Object {
public:
	virtual ~DIFactory() {}
	
	virtual Ref<DIObject> create(Ref<DynObject> config, Ref<DIObject> parent) = 0;
};


/**
 * \brief DI factory that wraps around a function or static method.
 */
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


/**
 * \brief Helper class for statically registering functions or static
 *        methods as DI factories.
 */
struct DIFunctionRegistration {
public:
	DIFunctionRegistration(std::string name, DIFunctionFactory::Function fn);
};


/**
 * \brief Helper macro to register functions as DI factories.
 */
#define CPPAPP_DI_FUNCTION(name__, fn__) \
	DIFunctionRegistration DIFunctionRegistration__##fn__(name__, fn__);

/**
 * \brief Helper macro to register methods as DI factories.
 */
#define CPPAPP_DI_METHOD(name__, cls__, fn__) \
	DIFunctionRegistration DIFunctionRegistration__##cls__##fn__(name__, cls__::fn__);


//// DIPlan /////////////////////////////////////////////////////////


/**
 * \brief Contains information necessary to instantiate a dependecy
 *        (including its dependencies) by name.
 */
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


#define DI_KEY_CFG_KEY      "key"
#define DI_FACTORY_CFG_KEY  "factory"
#define DI_CHILDREN_CFG_KEY "children"


/**
 * \brief Manager class for the dependency injection mechanism.
 *
 * Use this class to register dependency injection factories and
 * instantiate dependencies.
 */
class Injector {
private:
	std::map<std::string, Ref<DIFactory> > factories_;
	std::map<std::string, Ref<DIPlan> >    plans_;
	
	Injector(const Injector& other);
	
public:
	/**
	 * \brief Constructor.
	 */
	Injector() {}
	
	/**
	 * \brief Destructor.
	 */
	virtual ~Injector() {}
	
	/**
	 * \name Factory Management Methods
	 */
	///@{
	/**
	 * \brief Register a named factory with this injector.
	 *
	 * \param name    name of the factory
	 * \param factory factory instance to register
	 */
	void           registerFactory(std::string name, Ref<DIFactory> factory);
	/**
	 * \brief Returns a named factory.
	 *
	 * \param name name of the factory to return
	 *
	 * A named factory must be registered using \ref registerFactory first.
	 */
	Ref<DIFactory> getFactory(std::string name);
	///@}
	
	/**
	 * \name Plan Management Methods
	 */
	///@{
	Ref<DIPlan> makePlan(std::string name, Ref<DIFactory> factory, Ref<DynObject> config);
	Ref<DIPlan> makePlan(Ref<DynObject> config);
	Ref<DIPlan> getPlan(std::string name);
	void        makePlans(Ref<DynObject> config);
	///@}
	
	/**
	 * \brief Instantiate a dependency using a named plan.
	 *
	 * \param planName name of the plan to instantiate
	 */
	Ref<DIObject> instantiate(std::string planName);
	
	/**
	 * \brief Instantiate a dependency using a named plan and cast it to a type.
	 *
	 * \param planName name of the plan to instantiate
	 */
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
	
	/**
	 * \brief Removes all factories and plans.
	 */
	void clear()
	{
		factories_.clear();
		plans_.clear();
	}
	
	/**
	 * \brief Returns the singleton instance.
	 */
	static Injector& getInstance();
};


/** @} */


} // namespace cppapp


#endif /* end of include guard: INJECTOR_D3Q01JMO */

