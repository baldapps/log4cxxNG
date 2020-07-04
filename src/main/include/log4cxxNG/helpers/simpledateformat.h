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

#ifndef _LOG4CXXNG_HELPERS_SIMPLE_DATE_FORMAT_H
#define _LOG4CXXNG_HELPERS_SIMPLE_DATE_FORMAT_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif



#include <log4cxxNG/helpers/dateformat.h>
#include <vector>
#include <time.h>

#include <locale>

using std::locale;

namespace log4cxxng
{
namespace helpers
{
namespace SimpleDateFormatImpl
{
class PatternToken;
}

LOG4CXXNG_LIST_DEF(PatternTokenList, log4cxxng::helpers::SimpleDateFormatImpl::PatternToken*);


/**
 * Concrete class for formatting and parsing dates in a
 * locale-sensitive manner.
 */
class LOG4CXXNG_EXPORT SimpleDateFormat : public DateFormat
{
	public:
		/**
		 * Constructs a DateFormat using the given pattern and the default
		 * time zone.
		 *
		 * @param pattern the pattern describing the date and time format
		 */
		SimpleDateFormat(const LogString& pattern);
		SimpleDateFormat(const LogString& pattern, const std::locale* locale);
		~SimpleDateFormat();

		virtual void format(LogString& s,
			log4cxxng_time_t tm,
			log4cxxng::helpers::Pool& p) const;

		/**
		 * Set time zone.
		 * @param zone new time zone.
		 */
		void setTimeZone(const TimeZonePtr& zone);

	private:
		/**
		 * Time zone.
		 */
		TimeZonePtr timeZone;

		/**
		 * List of tokens.
		 */
		PatternTokenList pattern;

		static void addToken(const logchar spec, const int repeat, const std::locale* locale, PatternTokenList& pattern);
		static void parsePattern(const LogString& spec, const std::locale* locale, PatternTokenList& pattern);
};


}  // namespace helpers
} // namespace log4cxxng

#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif



#endif // _LOG4CXXNG_HELPERS_SIMPLE_DATE_FORMAT_H
