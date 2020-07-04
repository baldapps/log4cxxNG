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

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/filter/stringmatchfilter.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/optionconverter.h>

using namespace log4cxxng;
using namespace log4cxxng::filter;
using namespace log4cxxng::spi;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(StringMatchFilter)

StringMatchFilter::StringMatchFilter() :
	acceptOnMatch(true),
	stringToMatch()
{
}

void StringMatchFilter::setOption(const LogString& option,
	const LogString& value)
{

	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("STRINGTOMATCH"), LOG4CXXNG_STR("stringtomatch")))
	{
		stringToMatch = value;
	}
	else if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("ACCEPTONMATCH"), LOG4CXXNG_STR("acceptonmatch")))
	{
		acceptOnMatch = OptionConverter::toBoolean(value, acceptOnMatch);
	}
}

Filter::FilterDecision StringMatchFilter::decide(
	const log4cxxng::spi::LoggingEventPtr& event) const
{
	const LogString& msg = event->getRenderedMessage();

	if (msg.empty() || stringToMatch.empty())
	{
		return Filter::NEUTRAL;
	}


	if ( msg.find(stringToMatch) == LogString::npos )
	{
		return Filter::NEUTRAL;
	}
	else
	{
		// we've got a match
		if (acceptOnMatch)
		{
			return Filter::ACCEPT;
		}
		else
		{
			return Filter::DENY;
		}
	}
}

