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
#if defined(_MSC_VER)
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/pattern/linelocationpatternconverter.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/spi/location/locationinfo.h>
#include <log4cxxNG/helpers/stringhelper.h>

using namespace log4cxxng;
using namespace log4cxxng::pattern;
using namespace log4cxxng::spi;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(LineLocationPatternConverter)

LineLocationPatternConverter::LineLocationPatternConverter() :
	LoggingEventPatternConverter(LOG4CXXNG_STR("Line"),
		LOG4CXXNG_STR("line"))
{
}

PatternConverterPtr LineLocationPatternConverter::newInstance(
	const std::vector<LogString>& /* options */)
{
	static PatternConverterPtr instance(new LineLocationPatternConverter());
	return instance;
}

void LineLocationPatternConverter::format(
	const LoggingEventPtr& event,
	LogString& toAppendTo,
	Pool& p) const
{
	StringHelper::toString(
		event->getLocationInformation().getLineNumber(),
		p, toAppendTo);
}
