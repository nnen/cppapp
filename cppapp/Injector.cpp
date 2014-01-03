/**
 * \file   Injector.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-12-21
 * 
 * \brief  Implementation file for the Injector class.
 */

#include "Injector.h"


namespace cppapp {


////////////////////////////////////////////////////////////////////////////////
// DIFactory
////////////////////////////////////////////////////////////////////////////////


DIFunctionRegistration::DIFunctionRegistration(std::string name,
									  DIFunctionFactory::Function fn)
{
	Injector::getInstance().registerFactory(
		name,
		new DIFunctionFactory(fn)
	);
}


////////////////////////////////////////////////////////////////////////////////
// DIPlan
////////////////////////////////////////////////////////////////////////////////


Ref<DIObject> DIPlan::instantiate()
{
	Ref<DIObject> obj = factory_->create(config_);
	
	FOR_EACH(children_, it) {
		Ref<DIObject> child = (*it)->instantiate();
		obj->injectDependency(child, (*it)->getKey());
	}
	
	return obj;
}


////////////////////////////////////////////////////////////////////////////////
// Injector
////////////////////////////////////////////////////////////////////////////////


void Injector::registerFactory(std::string name, Ref<DIFactory> factory)
{
	VAR(found, factories_.find(name));
	if (found != factories_.end()) {
		LOG_WARNING("DI factory already registered: " << name);
	}
	factories_[name] = factory;
}


Ref<DIFactory> Injector::getFactory(std::string name)
{
	return factories_[name];
}


Ref<DIPlan> Injector::makePlan(Ref<DynObject> config)
{
	CPPAPP_ASSERT(config.isNotNull());
	CPPAPP_ASSERT(!config->isError());
	
	if (!config->hasStrItem(DI_KEY_CFG_KEY)) {
		LOG_ERROR(
			"Could not instantiate object configured at " <<
			config->getLocation() <<
			" - configuration doesn't have the \"" <<
			DI_KEY_CFG_KEY <<
			"\" key."
		);
		return NULL;
	}
	
	if (!config->hasStrItem(DI_FACTORY_CFG_KEY)) {
		LOG_ERROR(
			"Could not instantiate object configured at " <<
			config->getLocation() <<
			" - configuration doesn't have the \"" <<
			DI_FACTORY_CFG_KEY <<
			"\" key."
		);
		return NULL;
	}
	
	std::string key =
		config->getStrItem(DI_KEY_CFG_KEY)->getString();
	std::string factoryName =
		config->getStrItem(DI_FACTORY_CFG_KEY)->getString();
	
	Ref<DIFactory> factory = factories_[factoryName];
	if (factory.isNull()) {
		LOG_ERROR(
			"Could not instantiate object configured at " <<
			config->getLocation() <<
			" - factory \"" <<
			factoryName <<
			"\" could not be found."
		);
		return NULL;
	}
	
	Ref<DynObject> childrenConfig = config->getStrItem("children");
	std::vector<Ref<DIPlan> > children;
	
	if (childrenConfig.isNotNull()) {
		DYN_FOR_EACH(child, childrenConfig) {
			CPPAPP_ASSERT(child.isNotNull());
			CPPAPP_ASSERT(!child->isError());
			
			Ref<DIPlan> plan = makePlan(child);
			if (plan.isNull())
				return NULL;
			children.push_back(plan);
		}
	}
	
	return new DIPlan(
		key,
		factory,
		children,
		config
	);
}


Ref<DIPlan> Injector::getPlan(std::string name)
{
	return plans_[name];
}


void Injector::makePlans(Ref<DynObject> config)
{
	DYN_FOR_EACH(planConfig, config) {
		Ref<DIPlan> plan = makePlan(planConfig);
		
		if (plan.isNotNull()) {
			plans_[plan->getKey()] = plan;
		}
	}
}


Ref<DIObject> Injector::instantiate(std::string planName)
{
	Ref<DIPlan> plan = getPlan(planName);
	if (plan.isNull())
		return NULL;
	
	return plan->instantiate();
}


Injector& Injector::getInstance()
{
	static Injector injector;
	return injector;
}


} // namespace cppapp


