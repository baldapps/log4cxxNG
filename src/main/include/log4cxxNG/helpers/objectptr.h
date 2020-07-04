/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXXNG_HELPERS_OBJECT_PTR_H
#define _LOG4CXXNG_HELPERS_OBJECT_PTR_H

#include <log4cxxNG/log4cxxNG.h>

//
//   Helgrind (race detection tool for Valgrind) will complain if pointer
//   is not initialized in an atomic operation.  Static analysis tools
//   (gcc's -Weffc++, for example) will complain if pointer is not initialized
//   in member initialization list.  The use of a macro allows quick
//   switching between the initialization styles.
//
#if LOG4CXXNG_HELGRIND
	#define _LOG4CXXNG_OBJECTPTR_INIT(x) : ObjectPtrBase() { exchange(x);
#else
	#define _LOG4CXXNG_OBJECTPTR_INIT(x) : ObjectPtrBase(), p(x) {
#endif

namespace log4cxxng
{
namespace helpers
{
class Class;

class LOG4CXXNG_EXPORT ObjectPtrBase
{
	public:
		ObjectPtrBase();
		virtual ~ObjectPtrBase();
		static void checkNull(const int& null);
		static void* exchange(void** destination, void* newValue);
		virtual void* cast(const Class& cls) const = 0;
};


/** smart pointer to a Object descendant */
template<typename T> class ObjectPtrT : public ObjectPtrBase
{
	public:
		ObjectPtrT(const int& null)
		_LOG4CXXNG_OBJECTPTR_INIT(0)
		ObjectPtrBase::checkNull(null);
}

ObjectPtrT()
_LOG4CXXNG_OBJECTPTR_INIT(0)
}

ObjectPtrT(T* p1)
_LOG4CXXNG_OBJECTPTR_INIT(p1)

if (this->p != 0)
{
	this->p->addRef();
}
}


ObjectPtrT(const ObjectPtrT& p1)
_LOG4CXXNG_OBJECTPTR_INIT(p1.p)

if (this->p != 0)
{
	this->p->addRef();
}
}

ObjectPtrT(const ObjectPtrBase& p1)
_LOG4CXXNG_OBJECTPTR_INIT(reinterpret_cast<T*>(p1.cast(T::getStaticClass())))

if (this->p != 0)
{
	this->p->addRef();
}
}

ObjectPtrT(ObjectPtrBase& p1)
_LOG4CXXNG_OBJECTPTR_INIT(reinterpret_cast<T*>(p1.cast(T::getStaticClass())))

if (this->p != 0)
{
	this->p->addRef();
}
}


~ObjectPtrT()
{
	if (p != 0)
	{
		p->releaseRef();
	}
}

ObjectPtrT& operator=(const ObjectPtrT& p1)
{
	T* newPtr = p1.p;

	if (newPtr != 0)
	{
		newPtr->addRef();
	}

	T* oldPtr = exchange(newPtr);

	if (oldPtr != 0)
	{
		oldPtr->releaseRef();
	}

	return *this;
}

ObjectPtrT& operator=(const int& null)   //throw(IllegalArgumentException)
{
	//
	//   throws IllegalArgumentException if null != 0
	//
	ObjectPtrBase::checkNull(null);
	T* oldPtr = exchange(0);

	if (oldPtr != 0)
	{
		oldPtr->releaseRef();
	}

	return *this;
}

ObjectPtrT& operator=(T* p1)
{
	if (p1 != 0)
	{
		p1->addRef();
	}

	T* oldPtr = exchange(p1);

	if (oldPtr != 0)
	{
		oldPtr->releaseRef();
	}

	return *this;
}


ObjectPtrT& operator=(ObjectPtrBase& p1)
{
	T* newPtr = reinterpret_cast<T*>(p1.cast(T::getStaticClass()));
	return operator=(newPtr);
}

ObjectPtrT& operator=(const ObjectPtrBase& p1)
{
	T* newPtr = reinterpret_cast<T*>(p1.cast(T::getStaticClass()));
	return operator=(newPtr);
}

bool operator==(const ObjectPtrT& p1) const
{
	return (this->p == p1.p);
}
bool operator!=(const ObjectPtrT& p1) const
{
	return (this->p != p1.p);
}
bool operator<(const ObjectPtrT& p1) const
{
	return (this->p < p1.p);
}
bool operator==(const T* p1) const
{
	return (this->p == p1);
}
bool operator!=(const T* p1) const
{
	return (this->p != p1);
}
bool operator<(const T* p1) const
{
	return (this->p < p1);
}
T* operator->() const
{
	return p;
}
T& operator*() const
{
	return *p;
}
operator T* () const
{
	return p;
}



private:
T* p;
virtual void* cast(const Class& cls) const
{
	if (p != 0)
	{
		return const_cast<void*>(p->cast(cls));
	}

	return 0;
}
T* exchange(const T* newValue)
{
	// Avoid GCC strict aliasing warnings
	union
	{
		T** in;
		void** out;
	} temp = { &p };
	return static_cast<T*>(ObjectPtrBase::exchange(
				temp.out,
				const_cast<T*>(newValue)));
}

};


}
}

#endif //_LOG4CXXNG_HELPERS_OBJECT_PTR_H
