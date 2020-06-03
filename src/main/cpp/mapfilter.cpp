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

#include <log4cxx/logstring.h>
#include <log4cxx/filter/mapfilter.h>
#include <log4cxx/spi/loggingevent.h>
#include <log4cxx/helpers/stringhelper.h>
#include <log4cxx/helpers/optionconverter.h>

using namespace log4cxx;
using namespace log4cxx::filter;
using namespace log4cxx::spi;
using namespace log4cxx::helpers;

IMPLEMENT_LOG4CXX_OBJECT(MapFilter)

void MapFilter::setOption(const LogString& option,
	const LogString& value)
{

	if (StringHelper::equalsIgnoreCase(option, LOG4CXX_STR("ACCEPTONMATCH"), LOG4CXX_STR("acceptonmatch")))
	{
		acceptOnMatch = OptionConverter::toBoolean(value, acceptOnMatch);
	}
	else if (StringHelper::equalsIgnoreCase(option, LOG4CXX_STR("OPERATOR"), LOG4CXX_STR("operator")))
	{
		mustMatchAll = StringHelper::equalsIgnoreCase(value, LOG4CXX_STR("AND"), LOG4CXX_STR("and")) ? true : false;
	}
	else if (!option.empty() && !value.empty())
	{
		mapKeyValList[option] = value;
	}
}

Filter::FilterDecision MapFilter::decide(
	const log4cxx::spi::LoggingEventPtr& event) const
{
	if (mapKeyValList.empty())
	{
		return Filter::NEUTRAL;
	}

	bool    matched = true;

	for (auto it = mapKeyValList.cbegin(); it != mapKeyValList.cend(); ++it)
	{
		LogString curval;
		event->getMDC(it->first, curval);

		if (curval.empty() || curval != it->second)
		{
			matched = false;
		}
		else
		{
			matched = true;
		}

		if (mustMatchAll != matched)
		{
			break;
		}
	}

	if (acceptOnMatch)
	{
		return matched ? Filter::ACCEPT : Filter::DENY;
	}
	else
	{
		return matched ? Filter::DENY : Filter::ACCEPT;
	}
}
