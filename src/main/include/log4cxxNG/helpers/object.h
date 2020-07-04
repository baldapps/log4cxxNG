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

#ifndef _LOG4CXXNG_HELPERS_OBJECT_H
#define _LOG4CXXNG_HELPERS_OBJECT_H

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/helpers/class.h>
#include <log4cxxNG/helpers/objectptr.h>
#include <log4cxxNG/helpers/classregistration.h>


#define DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(object)\
	public:\
	class Clazz##object : public helpers::Class\
	{\
		public:\
			Clazz##object() : helpers::Class() {}\
			virtual ~Clazz##object() {}\
			virtual log4cxxng::LogString getName() const { return LOG4CXXNG_STR(#object); } \
	};\
	virtual const helpers::Class& getClass() const;\
	static const helpers::Class& getStaticClass(); \
	static const log4cxxng::helpers::ClassRegistration& registerClass();

#define DECLARE_LOG4CXXNG_OBJECT(object)\
	public:\
	class Clazz##object : public helpers::Class\
	{\
		public:\
			Clazz##object() : helpers::Class() {}\
			virtual ~Clazz##object() {}\
			virtual log4cxxng::LogString getName() const { return LOG4CXXNG_STR(#object); } \
			virtual helpers::ObjectPtr newInstance() const\
			{\
				return new object();\
			}\
	};\
	virtual const helpers::Class& getClass() const;\
	static const helpers::Class& getStaticClass(); \
	static const log4cxxng::helpers::ClassRegistration& registerClass();

#define DECLARE_LOG4CXXNG_OBJECT_WITH_CUSTOM_CLASS(object, class)\
	public:\
	virtual const helpers::Class& getClass() const;\
	static const helpers::Class& getStaticClass();\
	static const log4cxxng::helpers::ClassRegistration&  registerClass();

#define IMPLEMENT_LOG4CXXNG_OBJECT(object)\
	const ::log4cxxng::helpers::Class& object::getClass() const { return getStaticClass(); }\
	const ::log4cxxng::helpers::Class& object::getStaticClass() { \
		static Clazz##object theClass;                         \
		return theClass;                                       \
	}                                                                      \
	const log4cxxng::helpers::ClassRegistration& object::registerClass() {   \
		static log4cxxng::helpers::ClassRegistration classReg(object::getStaticClass); \
		return classReg; \
	}\
	namespace log4cxxng { namespace classes { \
	const ::log4cxxng::helpers::ClassRegistration& object##Registration = object::registerClass(); \
	} }


#define IMPLEMENT_LOG4CXXNG_OBJECT_WITH_CUSTOM_CLASS(object, class)\
	const log4cxxng::helpers::Class& object::getClass() const { return getStaticClass(); }\
	const log4cxxng::helpers::Class& object::getStaticClass() { \
		static class theClass;                                 \
		return theClass;                                       \
	}                                                         \
	const log4cxxng::helpers::ClassRegistration& object::registerClass() {   \
		static log4cxxng::helpers::ClassRegistration classReg(object::getStaticClass); \
		return classReg; \
	}\
	namespace log4cxxng { namespace classes { \
	const log4cxxng::helpers::ClassRegistration& object##Registration = object::registerClass(); \
	} }

namespace log4cxxng
{
class AppenderSkeleton;
class Logger;

namespace helpers
{
class Pool;

/** base class for java-like objects.*/
class LOG4CXXNG_EXPORT Object
{
	public:
		DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(Object)
		virtual ~Object() {}
		virtual void addRef() const = 0;
		virtual void releaseRef() const = 0;
		virtual bool instanceof(const Class& clazz) const = 0;
		virtual const void* cast(const Class& clazz) const = 0;
};
LOG4CXXNG_PTR_DEF(Object);
}
}

#define BEGIN_LOG4CXXNG_CAST_MAP()\
	const void * cast(const helpers::Class& clazz) const\
	{\
		const void * object = 0;\
		if (&clazz == &helpers::Object::getStaticClass()) return (const helpers::Object *)this;

#define END_LOG4CXXNG_CAST_MAP()\
	return object;\
	}\
	bool instanceof(const helpers::Class& clazz) const\
	{ return cast(clazz) != 0; }

#define LOG4CXXNG_CAST_ENTRY(Interface)\
	if (&clazz == &Interface::getStaticClass()) return (const Interface *)this;

#define LOG4CXXNG_CAST_ENTRY2(Interface, interface2)\
	if (&clazz == &Interface::getStaticClass()) return (Interface *)(interface2 *)this;

#define LOG4CXXNG_CAST_ENTRY_CHAIN(Interface)\
	object = Interface::cast(clazz);\
	if (object != 0) return object;

#endif //_LOG4CXXNG_HELPERS_OBJECT_H
