/**
 * \file   Object.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-27
 *
 * \brief  Header file for the Object class.
 */

#ifndef OBJECT_H_408JFDVEW32
#define OBJECT_H_408JFDVEW32

#include <cstddef>
#include <exception>
#include <utility>

#include "Debug.h"


namespace cppapp {


#define REF(var, expr) Ref<typeof(expr)> var = (expr)
#define SENTINEL 1337
#define DEAD_SENTINEL 31415


/**
 * \defgroup obj Object System
 */


/**
 * \brief Represents an item in a ring (circular double-linked list).
 *
 * Originally, this was supposed to be used to implement a ring
 * that would keep references to all allocated \ref Objects
 * so that memory leaks could be detected. Currently, it is not
 * used however.
 *
 * \ingroup obj
 */
template<class T>
struct RingItem {
private:
	RingItem<T>* previous_;
	RingItem<T>* next_;
	
	RingItem(const RingItem<T>& other);
	RingItem<T>& operator=(const RingItem<T>& other);

public:
	RingItem() : previous_(NULL), next_(NULL) {}
	~RingItem() { remove(); }
	
	inline RingItem<T>* previous() { return previous_; }
	inline RingItem<T>* next()     { return next_; }
	
	inline const RingItem<T>* previous() const { return previous_; }
	inline const RingItem<T>* next() const     { return next_; }
	
	void append(RingItem<T>& item)
	{
		item.remove();
		
		item.previous_ = this;
		item.next_     = next_;
		
		next_->previous_ = &item;
		next_            = &item;
	}
	
	void remove()
	{
		if (previous_ != NULL)
			previous_->next_ = next_;
		if (next_ != NULL)
			next_->previous_ = previous_;
		
		previous_ = NULL;
		next_     = NULL;
	}
};


/**
 * \brief Represents a reference-counted object.
 *
 * \note Instances of classes derived from Object may not be allocated on stack. For example:
 *
 * \code
 * class SomeClass : public Object {
 * }
 *
 * int main(void) {
 *     SomeClass instance;
 *     // do something
 *     // throws assertion exception here if compiled without NDEBUG
 * }
 * \endcode
 *
 * \ingroup obj
 */
class Object {
private:
	int refCount_;
	int sentinel_;

public:
	/**
	 * \brief Constructor
	 */
	Object();
	/**
	 * \brief Destructor
	 */
	virtual ~Object();
	
	/**
	 * \brief Increments object's reference count.
	 */
	void claim();
	/**
	 * \brief Attempts to detect whether this is actual live instance.
	 */
	void checkHealth() const;
	/**
	 * \brief Decreases object's reference count and frees the object
	 *        if the ref count reached 0.
	 *
	 * \param obj Object to be released.
	 */
	static Object* release(Object* obj);
	
	/**
	 * \brief Frees all allocated objects. <b>Not implemented</b>.
	 *
	 * \deprecated This method hasn't been implemented.
	 */
	static void deleteAll();
};


/**
 * \ingroup obj 
 */
template<class T>
class Box : public Object {
private:
	T value_;

public:
	Box() : value_() {}
	Box(T value) : value_(value) {}
	virtual ~Box() {}

	const T* operator -> () const { return &value_; }
	T* operator -> ()             { return &value_; }

	const T& operator * () const { return value_; }
	T& operator * () { return value_; }
};


/**
 * \ingroup obj
 */
template<class T>
class HeapBox : public Object {
private:
	T * value_;

public:
	HeapBox() : value_(NULL) {}
	HeapBox(T * value) : value_(value) {}
	virtual ~HeapBox() { delete value_; value_ = NULL; }
	
	const T* operator -> () const { return value_; }
	T* operator -> ()             { return value_; }
	
	const T& operator * () const  { return *value_; }
	T& operator * ()              { return *value_; }
};


/**
 * \brief Exception thrown when null \ref Ref reference is accessed.
 *
 * \ingroup obj
 */
class NullReferenceException : public std::exception {
public:
	NullReferenceException() throw() : std::exception() {}
	NullReferenceException(const NullReferenceException& e) throw() : std::exception() {}
	NullReferenceException& operator= (const NullReferenceException& e) throw() { return *this; }
	virtual ~NullReferenceException() throw() {}

	virtual const char* what() const throw() { return "Attempted to access null reference."; }
};


/**
 * \brief Exception thrown when dynamic cast fails.
 *
 * \ingroup obj
 */
class InvalidCastException : public std::exception {
public:
	InvalidCastException() throw() : std::exception() {}
	InvalidCastException(const InvalidCastException& e) throw() : std::exception() {}
	InvalidCastException& operator= (const InvalidCastException& e) throw() { return *this; }
	virtual ~InvalidCastException() throw() {}
	
	virtual const char* what() const throw() { return "Invalid dynamic cast."; }
};


template<class T>
class BRef;


/**
 * \brief Represents a reference (smart pointer) to an instance of \ref Object or derived class.
 *
 * \ingroup obj
 */
template<class T>
class Ref {
private:
	T* ptr_;

	void setPtr(T* value)
	{
		if (ptr_ == value)
			return;
		
		if (ptr_ != NULL)
			Object::release(ptr_);
		ptr_ = value;
		if (ptr_ != NULL)
			ptr_->claim();
	}

public:
	/**
	 * \brief Default constructor. Initializes reference to \c NULL.
	 */
	Ref()
	{
		ptr_ = NULL;
		setPtr(NULL);
	}
	
	/**
	 * \brief Pointer constructor. Claims object (see \ref Object::claim()).
	 */
	Ref(T* ptr)
	{
		ptr_ = NULL;
		setPtr(ptr);
	}
	
	/**
	 * \brief Copy constructor.
	 */
	Ref(const Ref<T>& ref)
	{
		ptr_ = NULL;
		setPtr(ref.getPtr());
	}
	
	/**
	 * \brief Destructor. Releases object (see \ref Object:release()).
	 */
	~Ref()
	{
		setPtr(NULL);
	}
	
	T& operator*()
	{
		if (isNull()) {
#ifndef NDEBUG
			Backtrace::print();
#endif
			throw NullReferenceException();
		}
		return *getPtr();
	}

	const T& operator*() const
	{
		if (isNull()) {
#ifndef NDEBUG
			Backtrace::print();
#endif
			throw NullReferenceException();
		}
		return *getPtr();
	}

	T* operator->() const
	{
		if (isNull()) {
#ifndef NDEBUG
			Backtrace::print();
#endif
			throw NullReferenceException();
		}
		return getPtr();
	}
	
	template<class U>
	operator Ref<U> () const
	{
		// Ref<U> r(dynamic_cast<U*>(getPtr()));
		Ref<U> ref = this->as<U>();
		if (ref.isNull() && !this->isNull())
			throw InvalidCastException();
		return ref;
	}
	
	Ref<T>& operator=(T* ptr)
	{
		setPtr(ptr);
		return *this;
	}
	
	Ref<T>& operator=(const Ref<T>& other)
	{
		if (this == &other) return *this;
		
		setPtr(other.getPtr());
		return *this;
	}
	
	bool operator==(const Ref<T>& other) const
	{
		return (getPtr() == other.getPtr());
	}
	
	bool operator==(const T* other) const
	{
		return (getPtr() == other);
	}
	
	bool operator<(const Ref<T>& other) const
	{
		return (getPtr() < other.getPtr());
	}
	
	/*
	template<class U>
	Ref<T>& operator=(const Ref<U>& other)
	{
		if (this == &other) return *this;
		
		// setPtr(dynamic_cast<T*>(other.getPtr()));
		setPtr((T*) other.getPtr());
		return *this;
	}
	*/
	
	/**
	 * \brief Returns \c true if the reference is \c NULL
	 *        (doesn't point to any object), \c false otherwise.
	 */
	bool isNull() const { return ptr_ == NULL; }
	/**
	 * \brief Return \c true if the reference is not \c NULL
	 *        (points to an object), \c false otherwise.
	 */
	bool isNotNull() const { return !isNull(); }
	
	T* getPtr() const
	{
		if (ptr_ != NULL) ptr_->checkHealth();
		return ptr_;
	}
	
	template<class U>
	Ref<U> as() const
	{
		return Ref<U>(dynamic_cast<U*>(getPtr()));
	}
};


typedef Ref<Object> ObjRef;


/**
 * \brief Like \ref Ref, except it references \ref Box instances.
 *
 * \ingroup obj
 */
template<class T>
class BRef {
private:
	Ref<Box<T> > box_;

public:
	BRef() : box_() {}
	BRef(T value) : box_(new Box<T>(value)) {}
	BRef(const BRef<T>& other) : box_(other.box_) {}
	BRef(const Ref<Box<T> >& other) : box_(other) {}
	~BRef() {}
	
	BRef<T>& operator=(const Ref<Object>& reference)
	{
		box_ = reference.as<Box<T> >();
		return *this;
	}
	
	T* operator -> () { return &(**box_); }
	const T* operator -> () const { return &(**box_); }
	
	T& operator * () { return (**box_); }
	const T& operator * () const { return (**box_); }
	
	operator Ref<Box<T> > () const { return box_; }
	operator Ref<Object> () const { return box_; }
	
	void         makeNew() { box_ = new Box<T>(); }
	Ref<Box<T> > box() { return box_; }
};


} // namespace cppapp


#endif /* end of include guard: OBJECT_H_408JFDVEW32 */

