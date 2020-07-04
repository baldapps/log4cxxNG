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

#ifndef _LOG4CXXNG_TESTS_UTIL_FILENAME_FILTER_H
#define _LOG4CXXNG_TESTS_UTIL_FILENAME_FILTER_H

#include "filter.h"

namespace log4cxxng
{
class FilenameFilter : public Filter
{
	public:
		FilenameFilter(const std::string& actual, const std::string& expected);
		static const std::string getMatch(const std::string& actual);
};
}


#endif //_LOG4CXXNG_TESTS_UTIL_FILENAME_FILTER_H
