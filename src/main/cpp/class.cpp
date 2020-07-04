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
#include <log4cxxNG/helpers/class.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/helpers/object.h>
#include <map>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/log4cxxNG.h>
#if !defined(LOG4CXXNG)
	#define LOG4CXXNG 1
#endif
#include <log4cxxNG/private/log4cxxNG_private.h>
#include <log4cxxNG/rollingfileappender.h>
#include <log4cxxNG/dailyrollingfileappender.h>


#include <log4cxxNG/asyncappender.h>
#include <log4cxxNG/consoleappender.h>
#include <log4cxxNG/fileappender.h>
#include <log4cxxNG/db/odbcappender.h>
#if defined(WIN32) || defined(_WIN32)
	#if !defined(_WIN32_WCE)
		#include <log4cxxNG/nt/nteventlogappender.h>
	#endif
	#include <log4cxxNG/nt/outputdebugstringappender.h>
#endif
#include <log4cxxNG/net/smtpappender.h>
#include <log4cxxNG/net/socketappender.h>
#include <log4cxxNG/net/sockethubappender.h>
#include <log4cxxNG/helpers/datagramsocket.h>
#include <log4cxxNG/net/syslogappender.h>
#include <log4cxxNG/net/telnetappender.h>
#include <log4cxxNG/writerappender.h>
#include <log4cxxNG/net/xmlsocketappender.h>
#include <log4cxxNG/layout.h>
#include <log4cxxNG/patternlayout.h>
#include <log4cxxNG/htmllayout.h>
#include <log4cxxNG/simplelayout.h>
#include <log4cxxNG/xml/xmllayout.h>
#include <log4cxxNG/ttcclayout.h>

#include <log4cxxNG/filter/levelmatchfilter.h>
#include <log4cxxNG/filter/levelrangefilter.h>
#include <log4cxxNG/filter/stringmatchfilter.h>
#include <log4cxxNG/rolling/filterbasedtriggeringpolicy.h>
#include <log4cxxNG/rolling/fixedwindowrollingpolicy.h>
#include <log4cxxNG/rolling/manualtriggeringpolicy.h>
#include <log4cxxNG/rolling/rollingfileappender.h>
#include <log4cxxNG/rolling/sizebasedtriggeringpolicy.h>
#include <log4cxxNG/rolling/timebasedrollingpolicy.h>

#include <log4cxxNG/xml/domconfigurator.h>
#include <log4cxxNG/propertyconfigurator.h>
#include <apr.h>


using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::net;
using namespace log4cxxng::filter;
using namespace log4cxxng::xml;
using namespace log4cxxng::rolling;

Class::Class()
{
}

Class::~Class()
{
}

LogString Class::toString() const
{
	return getName();
}

ObjectPtr Class::newInstance() const
{
	throw InstantiationException(LOG4CXXNG_STR("Cannot create new instances of Class."));
#if LOG4CXXNG_RETURN_AFTER_THROW
	return 0;
#endif
}



Class::ClassMap& Class::getRegistry()
{
	static ClassMap registry;
	return registry;
}

const Class& Class::forName(const LogString& className)
{
	LogString lowerName(StringHelper::toLowerCase(className));
	//
	//  check registry using full class name
	//
	const Class* clazz = getRegistry()[lowerName];

	if (clazz == 0)
	{
		LogString::size_type pos = className.find_last_of(LOG4CXXNG_STR(".$"));

		if (pos != LogString::npos)
		{
			LogString terminalName(lowerName, pos + 1, LogString::npos);
			clazz = getRegistry()[terminalName];

			if (clazz == 0)
			{
				registerClasses();
				clazz = getRegistry()[lowerName];

				if (clazz == 0)
				{
					clazz = getRegistry()[terminalName];
				}
			}
		}
		else
		{
			registerClasses();
			clazz = getRegistry()[lowerName];
		}
	}

	if (clazz == 0)
	{
		throw ClassNotFoundException(className);
	}

	return *clazz;
}

bool Class::registerClass(const Class& newClass)
{
	getRegistry()[StringHelper::toLowerCase(newClass.getName())] = &newClass;
	return true;
}

void Class::registerClasses()
{
#if APR_HAS_THREADS
	AsyncAppender::registerClass();
#endif
	ConsoleAppender::registerClass();
	FileAppender::registerClass();
	log4cxxng::db::ODBCAppender::registerClass();
#if (defined(WIN32) || defined(_WIN32))
#if !defined(_WIN32_WCE)
	log4cxxng::nt::NTEventLogAppender::registerClass();
#endif
	log4cxxng::nt::OutputDebugStringAppender::registerClass();
#endif
	log4cxxng::RollingFileAppender::registerClass();
	SMTPAppender::registerClass();
	SocketAppender::registerClass();
#if APR_HAS_THREADS
	SocketHubAppender::registerClass();
#endif
	SyslogAppender::registerClass();
#if APR_HAS_THREADS
	TelnetAppender::registerClass();
#endif
	XMLSocketAppender::registerClass();
	DateLayout::registerClass();
	HTMLLayout::registerClass();
	PatternLayout::registerClass();
	SimpleLayout::registerClass();
	TTCCLayout::registerClass();
	XMLLayout::registerClass();
	LevelMatchFilter::registerClass();
	LevelRangeFilter::registerClass();
	StringMatchFilter::registerClass();
	log4cxxng::RollingFileAppender::registerClass();
	log4cxxng::rolling::RollingFileAppender::registerClass();
	DailyRollingFileAppender::registerClass();
	log4cxxng::rolling::SizeBasedTriggeringPolicy::registerClass();
	log4cxxng::rolling::TimeBasedRollingPolicy::registerClass();
	log4cxxng::rolling::ManualTriggeringPolicy::registerClass();
	log4cxxng::rolling::FixedWindowRollingPolicy::registerClass();
	log4cxxng::rolling::FilterBasedTriggeringPolicy::registerClass();
	log4cxxng::xml::DOMConfigurator::registerClass();
	log4cxxng::PropertyConfigurator::registerClass();
}

