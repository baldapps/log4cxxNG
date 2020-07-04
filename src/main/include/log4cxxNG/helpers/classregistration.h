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

#ifndef _LOG4CXXNG_HELPERS_CLASSREGISTRATION_H
#define _LOG4CXXNG_HELPERS_CLASSREGISTRATION_H

#include <log4cxxNG/log4cxxNG.h>

namespace log4cxxng
{
namespace helpers
{
class Class;
class LOG4CXXNG_EXPORT ClassRegistration
{
	public:
		typedef const Class& (*ClassAccessor)();
		ClassRegistration(ClassAccessor classAccessor);

	private:
		ClassRegistration(const ClassRegistration&);
		ClassRegistration& operator=(const ClassRegistration&);
};
}  // namespace log4cxxng
} // namespace helper

#endif //_LOG4CXXNG_HELPERS_CLASSREGISTRATION_H
