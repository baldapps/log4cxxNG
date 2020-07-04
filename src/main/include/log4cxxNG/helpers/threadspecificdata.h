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

#ifndef _LOG4CXXNG_HELPERS_THREAD_SPECIFIC_DATA_H
#define _LOG4CXXNG_HELPERS_THREAD_SPECIFIC_DATA_H

#include <log4cxxNG/ndc.h>
#include <log4cxxNG/mdc.h>

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4251 )
#endif

namespace log4cxxng
{
namespace helpers
{
/**
  *   This class contains all the thread-specific
  *   data in use by log4cxx.
  */
class LOG4CXXNG_EXPORT ThreadSpecificData
{
	public:
		ThreadSpecificData();
		~ThreadSpecificData();

		/**
		 *  Gets current thread specific data.
		 *  @return thread specific data, may be null.
		 */
		static ThreadSpecificData* getCurrentData();
		/**
		 *  Release this ThreadSpecficData if empty.
		 */
		void recycle();

		static void put(const LogString& key, const LogString& val);
		static void push(const LogString& val);
		static void inherit(const log4cxxng::NDC::Stack& stack);

		log4cxxng::NDC::Stack& getStack();
		log4cxxng::MDC::Map& getMap();


	private:
		static ThreadSpecificData& getDataNoThreads();
		static ThreadSpecificData* createCurrentData();
		log4cxxng::NDC::Stack ndcStack;
		log4cxxng::MDC::Map mdcMap;
};

}  // namespace helpers
} // namespace log4cxxng

#if defined(_MSC_VER)
	#pragma warning (pop)
#endif

#endif
