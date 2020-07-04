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
#include "num343patternconverter.h"

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::pattern;

IMPLEMENT_LOG4CXXNG_OBJECT(Num343PatternConverter)


Num343PatternConverter::Num343PatternConverter() :
	LoggingEventPatternConverter(LOG4CXXNG_STR("Num343"), LOG4CXXNG_STR("num343"))
{
}

PatternConverterPtr Num343PatternConverter::newInstance(
	const std::vector<LogString>&)
{
	return new Num343PatternConverter();
}


void Num343PatternConverter::format(
	const spi::LoggingEventPtr&,
	LogString& sbuf,
	Pool&) const
{
	sbuf.append(LOG4CXXNG_STR("343"));
}

