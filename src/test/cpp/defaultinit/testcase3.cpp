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

#include <log4cxxNG/logmanager.h>
#include <log4cxxNG/logger.h>
#include <log4cxxNG/helpers/pool.h>
#include <apr_file_io.h>
#include "../insertwide.h"
#include "../logunit.h"

using namespace log4cxxng;

LOGUNIT_CLASS(TestCase3)
{
	LOGUNIT_TEST_SUITE(TestCase3);
	LOGUNIT_TEST(testProperties);
	LOGUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		helpers::Pool p;
		apr_file_copy("input/defaultInit3.properties", "log4cxx.properties", APR_FPROT_UREAD | APR_FPROT_UWRITE, p.getAPRPool());
	}

	void tearDown()
	{
		helpers::Pool p;
		apr_file_remove("log4cxx.properties", p.getAPRPool());
		LogManager::shutdown();
	}

	void testProperties()
	{
		LoggerPtr root = Logger::getRootLogger();
		LOG4CXXNG_DEBUG(root, "Hello, world");
		bool rootIsConfigured = !root->getAllAppenders().empty();
		LOGUNIT_ASSERT(rootIsConfigured);

		AppenderList list = root->getAllAppenders();
		AppenderPtr appender = list.front();
		LOGUNIT_ASSERT_EQUAL((LogString) LOG4CXXNG_STR("D3"), appender->getName());
	}

};

LOGUNIT_TEST_SUITE_REGISTRATION(TestCase3)
