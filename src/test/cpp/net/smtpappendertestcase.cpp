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

#define LOG4CXXNG_TEST 1
#include <log4cxxNG/private/log4cxxng_private.h>

#if LOG4CXXNG_HAVE_SMTP

#include <log4cxxNG/net/smtpappender.h>
#include "../appenderskeletontestcase.h"
#include <log4cxxNG/xml/domconfigurator.h>
#include <log4cxxNG/logmanager.h>
#include <log4cxxNG/ttcclayout.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::net;
using namespace log4cxxng::xml;
using namespace log4cxxng::spi;

namespace log4cxxng
{
namespace net
{

class MockTriggeringEventEvaluator :
	public virtual spi::TriggeringEventEvaluator,
	public virtual helpers::ObjectImpl
{
	public:
		DECLARE_LOG4CXXNG_OBJECT(MockTriggeringEventEvaluator)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(MockTriggeringEventEvaluator)
		LOG4CXXNG_CAST_ENTRY(spi::TriggeringEventEvaluator)
		END_LOG4CXXNG_CAST_MAP()

		MockTriggeringEventEvaluator()
		{
		}

		virtual bool isTriggeringEvent(const spi::LoggingEventPtr& event)
		{
			return true;
		}
	private:
		MockTriggeringEventEvaluator(const MockTriggeringEventEvaluator&);
		MockTriggeringEventEvaluator& operator=(const MockTriggeringEventEvaluator&);
};
}
}

IMPLEMENT_LOG4CXXNG_OBJECT(MockTriggeringEventEvaluator)


/**
   Unit tests of log4cxxng::SocketAppender
 */
class SMTPAppenderTestCase : public AppenderSkeletonTestCase
{
		LOGUNIT_TEST_SUITE(SMTPAppenderTestCase);
		//
		//    tests inherited from AppenderSkeletonTestCase
		//
		LOGUNIT_TEST(testDefaultThreshold);
		LOGUNIT_TEST(testSetOptionThreshold);
		LOGUNIT_TEST(testTrigger);
		LOGUNIT_TEST(testInvalid);
		LOGUNIT_TEST_SUITE_END();


	public:

		AppenderSkeleton* createAppenderSkeleton() const
		{
			return new log4cxxng::net::SMTPAppender();
		}

		void setUp()
		{
		}

		void tearDown()
		{
			LogManager::resetConfiguration();
		}

		/**
		 * Tests that triggeringPolicy element will set evaluator.
		 */
		void testTrigger()
		{
			DOMConfigurator::configure("input/xml/smtpAppender1.xml");
			SMTPAppenderPtr appender(Logger::getRootLogger()->getAppender(LOG4CXXNG_STR("A1")));
			TriggeringEventEvaluatorPtr evaluator(appender->getEvaluator());
			LOGUNIT_ASSERT_EQUAL(true, evaluator->instanceof(MockTriggeringEventEvaluator::getStaticClass()));
		}

		void testInvalid()
		{
			SMTPAppenderPtr appender(new SMTPAppender());
			appender->setSMTPHost(LOG4CXXNG_STR("smtp.invalid"));
			appender->setTo(LOG4CXXNG_STR("you@example.invalid"));
			appender->setFrom(LOG4CXXNG_STR("me@example.invalid"));
			appender->setLayout(new TTCCLayout());
			Pool p;
			appender->activateOptions(p);
			LoggerPtr root(Logger::getRootLogger());
			root->addAppender(appender);
			LOG4CXXNG_INFO(root, "Hello, World.")
			LOG4CXXNG_ERROR(root, "Sending Message")
		}

};

LOGUNIT_TEST_SUITE_REGISTRATION(SMTPAppenderTestCase);

#endif
