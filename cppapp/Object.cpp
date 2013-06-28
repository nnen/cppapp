/**
 * \file   leela/Object.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Implementation of the Object class.
 */

#include <cassert>
#include <exception>

#include <iostream>

#include "Object.h"
#include "Debug.h"


namespace cppapp {


Object::Object()
{
	refCount_ = 0;
	sentinel_ = SENTINEL;
}


Object::~Object()
{
	checkHealth();
	
	refCount_ = 0;
	sentinel_ = DEAD_SENTINEL;
}


void Object::claim()
{
	checkHealth();
	refCount_++;
}


void Object::checkHealth() const
{
	CPPAPP_ASSERT(refCount_ >= 0);
	CPPAPP_ASSERT(sentinel_ == SENTINEL);
}


Object* Object::release(Object* obj)
{
	if (obj != NULL) obj->checkHealth();
	
	CPPAPP_ASSERT(obj->refCount_ > 0);
	
	if (--(obj->refCount_) <= 0) {
		delete obj;
		return NULL;
	}
	return obj;
}


void Object::deleteAll()
{
}


} // namespace cppapp

