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

#ifndef _LOG4CXXNG_HELPERS_DATE_H
#define _LOG4CXXNG_HELPERS_DATE_H

#include <log4cxxNG/helpers/objectimpl.h>
#include <log4cxxNG/log4cxxNG.h>


namespace log4cxxng
{
namespace helpers
{
/**
*    Simple transcoder for converting between
*      external char and wchar_t strings and
*      internal strings.
*
*/
class LOG4CXXNG_EXPORT Date : public ObjectImpl
{
		const log4cxxng_time_t time;

	public:
		DECLARE_LOG4CXXNG_OBJECT(Date)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(Date)
		END_LOG4CXXNG_CAST_MAP()

		Date();
		Date(log4cxxng_time_t time);
		virtual ~Date();

		inline log4cxxng_time_t getTime() const
		{
			return time;
		}

		/**
		 *   Get start of next second
		 */
		log4cxxng_time_t getNextSecond() const;


		static log4cxxng_time_t getMicrosecondsPerDay();
		static log4cxxng_time_t getMicrosecondsPerSecond();

};

LOG4CXXNG_PTR_DEF(Date);

}
}


#endif
