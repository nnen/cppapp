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


namespace cppapp {


Object::RingItem* Object::_ring = NULL;


Object::Object()
{
	_refCount = 0;
	_sentinel = SENTINEL;
}


Object::~Object()
{
	checkHealth();
	
	_refCount = 0;
	_sentinel = DEAD_SENTINEL;
}


void Object::claim()
{
	checkHealth();
	_refCount++;
}


void Object::checkHealth() const
{
	assert(_refCount >= 0);
	assert(_sentinel == SENTINEL);
}


Object* Object::release(Object* obj)
{
	if (obj != NULL) obj->checkHealth();
	
	if (--(obj->_refCount) <= 0) {
		delete obj;
		return NULL;
	}
	return obj;
}


void Object::deleteAll()
{
}


} // namespace cppapp

