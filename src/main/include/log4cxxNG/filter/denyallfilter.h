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

#ifndef _LOG4CXXNG_FILTER_DENY_ALL_FILTER_H
#define _LOG4CXXNG_FILTER_DENY_ALL_FILTER_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif


#include <log4cxxNG/spi/filter.h>

namespace log4cxxng
{
namespace filter
{
/**
This filter drops all logging events.
<p>You can add this filter to the end of a filter chain to
switch from the default "accept all unless instructed otherwise"
filtering behaviour to a "deny all unless instructed otherwise"
behaviour.
*/

class LOG4CXXNG_EXPORT DenyAllFilter : public spi::Filter
{
	public:
		DenyAllFilter() : spi::Filter()
		{
		}

		typedef spi::Filter BASE_CLASS;
		DECLARE_LOG4CXXNG_OBJECT(DenyAllFilter)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(DenyAllFilter)
		LOG4CXXNG_CAST_ENTRY_CHAIN(BASE_CLASS)
		END_LOG4CXXNG_CAST_MAP()

		/**
		Always returns the integer constant {@link spi::Filter#DENY DENY}
		regardless of the {@link spi::LoggingEventPtr LoggingEvent} parameter.
		@param event The LoggingEvent to filter.
		@return Always returns {@link spi::Filter#DENY DENY}.
		*/
		FilterDecision decide(const spi::LoggingEventPtr& event) const
		{
			return spi::Filter::DENY;
		}
}; // class DenyAllFilter

LOG4CXXNG_PTR_DEF(DenyAllFilter);
}  // namespace filter
} // namespace log4cxxng

#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif


#endif // _LOG4CXXNG_FILTER_DENY_ALL_FILTER_H
