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

#include <log4cxxNG/logger.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/helpers/system.h>
#include <log4cxxNG/level.h>

#include "num343patternconverter.h"
#include "../testchar.h"
#include "../insertwide.h"
#include "../logunit.h"
#include <log4cxxNG/spi/loggerrepository.h>
#include <log4cxxNG/pattern/patternparser.h>
#include <log4cxxNG/pattern/patternconverter.h>
#include <log4cxxNG/helpers/relativetimedateformat.h>
#include <log4cxxNG/helpers/simpledateformat.h>


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


using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::spi;
using namespace log4cxx::pattern;

#define RULES_PUT(spec, cls) \
	map.insert(PatternMap::value_type(LOG4CXX_STR(spec), (PatternConstructor) cls ::newInstance))


LOGUNIT_CLASS(PatternParserTestCase)
{
	LOGUNIT_TEST_SUITE(PatternParserTestCase);
	LOGUNIT_TEST(testNewWord);
	LOGUNIT_TEST(testNewWord2);
	LOGUNIT_TEST(testBogusWord1);
	LOGUNIT_TEST(testBogusWord2);
	LOGUNIT_TEST(testBasic1);
	LOGUNIT_TEST(testBasic2);
	LOGUNIT_TEST(testMultiOption);
	LOGUNIT_TEST_SUITE_END();

	LoggingEventPtr event;

public:
	void setUp()
	{
		event = new LoggingEvent(
			LOG4CXX_STR("org.foobar"), Level::getInfo(), LOG4CXX_STR("msg 1"), LOG4CXX_LOCATION);
	}

	void tearDown()
	{
	}


	PatternMap getFormatSpecifiers()
	{
		PatternMap map;
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

		return map;

	}

	void assertFormattedEquals(const LogString & pattern,
		const PatternMap & patternMap,
		const LogString & expected)
	{
		std::vector<PatternConverterPtr> converters;
		std::vector<FormattingInfoPtr> fields;
		PatternParser::parse(pattern, converters, fields, patternMap);
		Pool p;
		LogString actual;
		std::vector<FormattingInfoPtr>::const_iterator fieldIter = fields.begin();

		for (std::vector<PatternConverterPtr>::const_iterator converterIter = converters.begin();
			converterIter != converters.end();
			converterIter++, fieldIter++)
		{
			int fieldStart = actual.length();
			(*converterIter)->format(event, actual, p);
			(*fieldIter)->format(fieldStart, actual);
		}

		LOGUNIT_ASSERT_EQUAL(expected, actual);
	}


	void testNewWord()
	{
		PatternMap testRules(getFormatSpecifiers());
		testRules.insert(
			PatternMap::value_type(LOG4CXX_STR("z343"),
				(PatternConstructor) Num343PatternConverter::newInstance));
		assertFormattedEquals(LOG4CXX_STR("%z343"), testRules, LOG4CXX_STR("343"));
	}


	/* Test whether words starting with the letter 'n' are treated differently,
	 * which was previously the case by mistake.
	 */
	void testNewWord2()
	{
		PatternMap testRules(getFormatSpecifiers());
		testRules.insert(
			PatternMap::value_type(LOG4CXX_STR("n343"),
				(PatternConstructor) Num343PatternConverter::newInstance));
		assertFormattedEquals(LOG4CXX_STR("%n343"), testRules, LOG4CXX_STR("343"));
	}

	void testBogusWord1()
	{
		assertFormattedEquals(LOG4CXX_STR("%, foobar"),
			getFormatSpecifiers(),
			LOG4CXX_STR("%, foobar"));
	}

	void testBogusWord2()
	{
		assertFormattedEquals(LOG4CXX_STR("xyz %, foobar"),
			getFormatSpecifiers(),
			LOG4CXX_STR("xyz %, foobar"));
	}

	void testBasic1()
	{
		assertFormattedEquals(LOG4CXX_STR("hello %-5level - %m%n"),
			getFormatSpecifiers(),
			LogString(LOG4CXX_STR("hello INFO  - msg 1")) + LOG4CXX_EOL);
	}

	void testBasic2()
	{
		Pool pool;
		RelativeTimeDateFormat relativeFormat;
		LogString expected;
		relativeFormat.format(expected, event->getTimeStamp(), pool);

		expected.append(LOG4CXX_STR(" INFO  ["));
		expected.append(event->getThreadName());
		expected.append(LOG4CXX_STR("] org.foobar - msg 1"));
		expected.append(LOG4CXX_EOL);

		assertFormattedEquals(LOG4CXX_STR("%relative %-5level [%thread] %logger - %m%n"),
			getFormatSpecifiers(),
			expected);
	}

	void testMultiOption()
	{
		Pool pool;

		SimpleDateFormat dateFormat(LOG4CXX_STR("HH:mm:ss"));
		LogString localTime;
		dateFormat.format(localTime, event->getTimeStamp(), pool);

		dateFormat.setTimeZone(TimeZone::getGMT());
		LogString utcTime;
		dateFormat.format(utcTime, event->getTimeStamp(), pool);

		LogString expected(utcTime);
		expected.append(1, LOG4CXX_STR(' '));
		expected.append(localTime);
		expected.append(LOG4CXX_STR(" org.foobar  - msg 1"));


		assertFormattedEquals(LOG4CXX_STR("%d{HH:mm:ss}{GMT} %d{HH:mm:ss} %c  - %m"),
			getFormatSpecifiers(),
			expected);
	}

};

//
//   See bug LOGCXX-204
//
#if !defined(_MSC_VER) || _MSC_VER > 1200
	LOGUNIT_TEST_SUITE_REGISTRATION(PatternParserTestCase);
#endif
