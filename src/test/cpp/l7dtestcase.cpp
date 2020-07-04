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
#include <log4cxxNG/propertyconfigurator.h>
#include <log4cxxNG/helpers/propertyresourcebundle.h>
#include <log4cxxNG/helpers/locale.h>

#include "util/compare.h"

#include <vector>
#include <sstream>

#include "testchar.h"
#include "logunit.h"
#include <log4cxxNG/spi/loggerrepository.h>


typedef std::basic_ostringstream<testchar> StringBuffer;

using namespace log4cxxng;
using namespace log4cxxng::helpers;

LOGUNIT_CLASS(L7dTestCase)
{
	LOGUNIT_TEST_SUITE(L7dTestCase);
	LOGUNIT_TEST(test1);
	LOGUNIT_TEST_SUITE_END();

	LoggerPtr root;
	ResourceBundlePtr bundles[3];

public:
	void setUp()
	{
		Locale localeUS(LOG4CXXNG_STR("en"), LOG4CXXNG_STR("US"));
		bundles[0] =
			ResourceBundle::getBundle(LOG4CXXNG_STR("L7D"), localeUS);
		LOGUNIT_ASSERT(bundles[0] != 0);

		Locale localeFR(LOG4CXXNG_STR("fr"), LOG4CXXNG_STR("FR"));
		bundles[1] =
			ResourceBundle::getBundle(LOG4CXXNG_STR("L7D"), localeFR);
		LOGUNIT_ASSERT(bundles[1] != 0);

		Locale localeCH(LOG4CXXNG_STR("fr"), LOG4CXXNG_STR("CH"));
		bundles[2] =
			ResourceBundle::getBundle(LOG4CXXNG_STR("L7D"), localeCH);
		LOGUNIT_ASSERT(bundles[2] != 0);

		root = Logger::getRootLogger();
	}

	void tearDown()
	{
		root->getLoggerRepository()->resetConfiguration();
	}

	void test1()
	{
		PropertyConfigurator::configure(LOG4CXXNG_FILE("input/l7d1.properties"));

		log4cxxng::helpers::Pool pool;

		for (int i = 0; i < 3; i++)
		{
			root->setResourceBundle(bundles[i]);

			LOG4CXXNG_L7DLOG(root, Level::getDebug(), LOG4CXXNG_TEST_STR("bogus1"));
			LOG4CXXNG_L7DLOG(root, Level::getInfo(), LOG4CXXNG_TEST_STR("test"));
			LOG4CXXNG_L7DLOG(root, Level::getWarn(), LOG4CXXNG_TEST_STR("hello_world"));


			StringBuffer os;
			os << (i + 1);
			LOG4CXXNG_L7DLOG2(root, Level::getDebug(), LOG4CXXNG_TEST_STR("msg1"), os.str().c_str(),
				LOG4CXXNG_TEST_STR("log4j"));
			LOG4CXXNG_L7DLOG2(root, Level::getError(), LOG4CXXNG_TEST_STR("bogusMsg"), os.str().c_str(),
				LOG4CXXNG_TEST_STR("log4j"));
			LOG4CXXNG_L7DLOG2(root, Level::getError(), LOG4CXXNG_TEST_STR("msg1"), os.str().c_str(),
				LOG4CXXNG_TEST_STR("log4j"));
			LOG4CXXNG_L7DLOG(root, Level::getInfo(), LOG4CXXNG_TEST_STR("bogus2"));

			LOG4CXXNG_L7DLOG3(root, Level::getError(), LOG4CXXNG_TEST_STR("msg3"), os.str().c_str(),
				LOG4CXXNG_TEST_STR("log4j"), LOG4CXXNG_TEST_STR("log4cxx"));
		}

		LOGUNIT_ASSERT(Compare::compare(LOG4CXXNG_FILE("output/temp"),
				LOG4CXXNG_FILE("witness/l7d.1")));
	}

};

LOGUNIT_TEST_SUITE_REGISTRATION(L7dTestCase);
