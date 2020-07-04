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

#include "../util/compare.h"
#include "../logunit.h"
#include "../insertwide.h"
#include <log4cxxNG/logmanager.h>
#include <log4cxxNG/xml/domconfigurator.h>
#include <log4cxxNG/patternlayout.h>
#include <log4cxxNG/rolling/rollingfileappender.h>
#include <log4cxxNG/rolling/fixedwindowrollingpolicy.h>
#include <log4cxxNG/rolling/filterbasedtriggeringpolicy.h>
#include <log4cxxNG/filter/levelrangefilter.h>
#include <log4cxxNG/helpers/pool.h>
#include <log4cxxNG/logger.h>

using namespace log4cxxng;
using namespace log4cxxng::rolling;
using namespace log4cxxng::xml;
using namespace log4cxxng::filter;
using namespace log4cxxng::helpers;

/**
 *
 * Tests of rolling file appender with a filter based triggering policy.
 *
 *
 *
 *
 */
LOGUNIT_CLASS(FilterBasedRollingTest)
{
	LOGUNIT_TEST_SUITE(FilterBasedRollingTest);
	LOGUNIT_TEST(test1);
	LOGUNIT_TEST(test2);
	LOGUNIT_TEST_SUITE_END();

public:
	void tearDown()
	{
		LogManager::getLoggerRepository()->resetConfiguration();
	}

	/**
	 * Test basic rolling functionality using configuration file.
	 */
	void test1()
	{
		DOMConfigurator::configure(
			"./input/rolling/filter1.xml" /*, LogManager::getLoggerRepository() */);

		common(LOG4CXXNG_STR("output/filterBased-test1"));
	}

	/**
	 * Test basic rolling functionality using explicit configuration.
	 * @remarks Test fails when run immediately after test1.
	 */
	void test2()
	{
		LayoutPtr layout(new PatternLayout(LOG4CXXNG_STR("%m\n")));
		RollingFileAppenderPtr rfa(new RollingFileAppender());
		rfa->setName(LOG4CXXNG_STR("ROLLING"));
		rfa->setLayout(layout);

		FixedWindowRollingPolicyPtr swrp(new FixedWindowRollingPolicy());
		FilterBasedTriggeringPolicyPtr fbtp(new FilterBasedTriggeringPolicy());

		LevelRangeFilterPtr rf(new LevelRangeFilter());
		rf->setLevelMin(Level::getInfo());
		fbtp->addFilter(rf);

		Pool p;
		fbtp->activateOptions(p);

		swrp->setMinIndex(0);
		rfa->setFile(LOG4CXXNG_STR("output/filterBased-test2.log"));
		rfa->setAppend(false);

		swrp->setFileNamePattern(LOG4CXXNG_STR("output/filterBased-test2.%i"));
		swrp->activateOptions(p);

		rfa->setRollingPolicy(swrp);
		rfa->setTriggeringPolicy(fbtp);
		rfa->activateOptions(p);
		Logger::getRootLogger()->addAppender(rfa);
		Logger::getRootLogger()->setLevel(Level::getDebug());

		common(LOG4CXXNG_STR("output/filterBased-test2"));
	}

private:
	/**
	 *   Common aspects of test1 and test2
	 */
	void common(const LogString & baseName)
	{
		LoggerPtr logger(Logger::getLogger("org.apache.log4j.rolling.FilterBasedRollingTest"));

		// Write exactly 10 bytes with each log
		for (int i = 0; i < 25; i++)
		{
			char msg[10];
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
			strcpy_s(msg, sizeof msg, "Hello---?");
#else
			strcpy(msg, "Hello---?");
#endif

			if (i < 10)
			{
				msg[8] = (char) ('0' + i);
				LOG4CXXNG_DEBUG(logger, msg);
			}
			else if (i < 100)
			{
				msg[7] = (char) ('0' + (i / 10));
				msg[8] = (char) ('0' + (i % 10));

				if ((i % 10) == 0)
				{
					LOG4CXXNG_WARN(logger, msg);
				}
				else
				{
					LOG4CXXNG_DEBUG(logger, msg);
				}
			}
		}

		//
		//  test was constructed to mimic SizeBasedRollingTest.test2
		//
		LOGUNIT_ASSERT_EQUAL(true,
			Compare::compare(baseName + LOG4CXXNG_STR(".log"), LogString(LOG4CXXNG_STR("witness/rolling/sbr-test2.log"))));
		LOGUNIT_ASSERT_EQUAL(true,
			Compare::compare(baseName + LOG4CXXNG_STR(".0"), LogString(LOG4CXXNG_STR("witness/rolling/sbr-test2.0"))));
		LOGUNIT_ASSERT_EQUAL(true,
			Compare::compare(baseName + LOG4CXXNG_STR(".1"), LogString(LOG4CXXNG_STR("witness/rolling/sbr-test2.1"))));
	}
};

LOGUNIT_TEST_SUITE_REGISTRATION(FilterBasedRollingTest);

