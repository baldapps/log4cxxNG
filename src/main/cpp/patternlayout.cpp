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
#include <log4cxxNG/patternlayout.h>
#include <log4cxxNG/pattern/patternparser.h>
#include <log4cxxNG/pattern/loggingeventpatternconverter.h>
#include <log4cxxNG/pattern/formattinginfo.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/pool.h>
#include <log4cxxNG/helpers/optionconverter.h>

#include <log4cxxNG/pattern/loggerpatternconverter.h>
#include <log4cxxNG/pattern/literalpatternconverter.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/pattern/classnamepatternconverter.h>
#include <log4cxxNG/pattern/datepatternconverter.h>
#include <log4cxxNG/pattern/filedatepatternconverter.h>
#include <log4cxxNG/pattern/filelocationpatternconverter.h>
#include <log4cxxNG/pattern/fulllocationpatternconverter.h>
#include <log4cxxNG/pattern/integerpatternconverter.h>
#include <log4cxxNG/pattern/linelocationpatternconverter.h>
#include <log4cxxNG/pattern/messagepatternconverter.h>
#include <log4cxxNG/pattern/lineseparatorpatternconverter.h>
#include <log4cxxNG/pattern/methodlocationpatternconverter.h>
#include <log4cxxNG/pattern/levelpatternconverter.h>
#include <log4cxxNG/pattern/relativetimepatternconverter.h>
#include <log4cxxNG/pattern/threadpatternconverter.h>
#include <log4cxxNG/pattern/ndcpatternconverter.h>
#include <log4cxxNG/pattern/propertiespatternconverter.h>
#include <log4cxxNG/pattern/throwableinformationpatternconverter.h>


using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::spi;
using namespace log4cxxng::pattern;

IMPLEMENT_LOG4CXXNG_OBJECT(PatternLayout)


PatternLayout::PatternLayout()
{
}

/**
Constructs a PatternLayout using the supplied conversion pattern.
*/
PatternLayout::PatternLayout(const LogString& pattern)
	: conversionPattern(pattern)
{
	Pool pool;
	activateOptions(pool);
}

void PatternLayout::setConversionPattern(const LogString& pattern)
{
	conversionPattern = pattern;
	Pool pool;
	activateOptions(pool);
}

void PatternLayout::format(LogString& output,
	const spi::LoggingEventPtr& event,
	Pool& pool) const
{
	std::vector<FormattingInfoPtr>::const_iterator formatterIter =
		patternFields.begin();

	for (std::vector<LoggingEventPatternConverterPtr>::const_iterator
		converterIter = patternConverters.begin();
		converterIter != patternConverters.end();
		converterIter++, formatterIter++)
	{
		int startField = (int)output.length();
		(*converterIter)->format(event, output, pool);
		(*formatterIter)->format(startField, output);
	}

}

void PatternLayout::setOption(const LogString& option, const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("CONVERSIONPATTERN"),
			LOG4CXXNG_STR("conversionpattern")))
	{
		conversionPattern = OptionConverter::convertSpecialChars(value);
	}
}

void PatternLayout::activateOptions(Pool&)
{
	LogString pat(conversionPattern);

	if (pat.empty())
	{
		pat = LOG4CXXNG_STR("%m%n");
	}

	patternConverters.erase(patternConverters.begin(), patternConverters.end());
	patternFields.erase(patternFields.begin(), patternFields.end());
	std::vector<PatternConverterPtr> converters;
	PatternParser::parse(pat,
		converters,
		patternFields,
		getFormatSpecifiers());

	//
	//   strip out any pattern converters that don't handle LoggingEvents
	//
	//
	for (std::vector<PatternConverterPtr>::const_iterator converterIter = converters.begin();
		converterIter != converters.end();
		converterIter++)
	{
		LoggingEventPatternConverterPtr eventConverter(*converterIter);

		if (eventConverter != NULL)
		{
			patternConverters.push_back(eventConverter);
		}
	}
}

#define RULES_PUT(spec, cls) \
	specs.insert(PatternMap::value_type(LogString(LOG4CXXNG_STR(spec)), (PatternConstructor) cls ::newInstance))


log4cxxng::pattern::PatternMap PatternLayout::getFormatSpecifiers()
{
	PatternMap specs;
	RULES_PUT("c", LoggerPatternConverter);
	RULES_PUT("logger", LoggerPatternConverter);

	RULES_PUT("C", ClassNamePatternConverter);
	RULES_PUT("class", ClassNamePatternConverter);

	RULES_PUT("d", DatePatternConverter);
	RULES_PUT("date", DatePatternConverter);

	RULES_PUT("F", FileLocationPatternConverter);
	RULES_PUT("file", FileLocationPatternConverter);

	RULES_PUT("l", FullLocationPatternConverter);

	RULES_PUT("L", LineLocationPatternConverter);
	RULES_PUT("line", LineLocationPatternConverter);

	RULES_PUT("m", MessagePatternConverter);
	RULES_PUT("message", MessagePatternConverter);

	RULES_PUT("n", LineSeparatorPatternConverter);

	RULES_PUT("M", MethodLocationPatternConverter);
	RULES_PUT("method", MethodLocationPatternConverter);

	RULES_PUT("p", LevelPatternConverter);
	RULES_PUT("level", LevelPatternConverter);

	RULES_PUT("r", RelativeTimePatternConverter);
	RULES_PUT("relative", RelativeTimePatternConverter);

	RULES_PUT("t", ThreadPatternConverter);
	RULES_PUT("thread", ThreadPatternConverter);

	RULES_PUT("x", NDCPatternConverter);
	RULES_PUT("ndc", NDCPatternConverter);

	RULES_PUT("X", PropertiesPatternConverter);
	RULES_PUT("properties", PropertiesPatternConverter);

	RULES_PUT("throwable", ThrowableInformationPatternConverter);
	return specs;
}





