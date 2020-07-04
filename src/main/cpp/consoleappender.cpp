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
#include <log4cxxNG/logstring.h>
#include <log4cxxNG/consoleappender.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/systemoutwriter.h>
#include <log4cxxNG/helpers/systemerrwriter.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/layout.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(ConsoleAppender)

ConsoleAppender::ConsoleAppender()
	: target(getSystemOut())
{
}

ConsoleAppender::ConsoleAppender(const LayoutPtr& layout1)
	: target(getSystemOut())
{
	setLayout(layout1);
	Pool p;
	WriterPtr writer1(new SystemOutWriter());
	setWriter(writer1);
	WriterAppender::activateOptions(p);
}

ConsoleAppender::ConsoleAppender(const LayoutPtr& layout1, const LogString& target1)
	: target(target1)
{
	setLayout(layout1);
	Pool p;
	ConsoleAppender::activateOptions(p);
}

ConsoleAppender::~ConsoleAppender()
{
	finalize();
}

const LogString& ConsoleAppender::getSystemOut()
{
	static const LogString name(LOG4CXXNG_STR("System.out"));
	return name;
}

const LogString& ConsoleAppender::getSystemErr()
{
	static const LogString name(LOG4CXXNG_STR("System.err"));
	return name;
}

void ConsoleAppender::setTarget(const LogString& value)
{
	LogString v = StringHelper::trim(value);

	if (StringHelper::equalsIgnoreCase(v,
			LOG4CXXNG_STR("SYSTEM.OUT"), LOG4CXXNG_STR("system.out")))
	{
		target = getSystemOut();
	}
	else if (StringHelper::equalsIgnoreCase(v,
			LOG4CXXNG_STR("SYSTEM.ERR"), LOG4CXXNG_STR("system.err")))
	{
		target = getSystemErr();
	}
	else
	{
		targetWarn(value);
	}
}

LogString ConsoleAppender::getTarget() const
{
	return target;
}

void ConsoleAppender::targetWarn(const LogString& val)
{
	LogLog::warn(((LogString) LOG4CXXNG_STR("["))
		+ val +  LOG4CXXNG_STR("] should be system.out or system.err."));
	LogLog::warn(LOG4CXXNG_STR("Using previously set target, System.out by default."));
}

void ConsoleAppender::activateOptions(Pool& p)
{
	if (StringHelper::equalsIgnoreCase(target,
			LOG4CXXNG_STR("SYSTEM.OUT"), LOG4CXXNG_STR("system.out")))
	{
		WriterPtr writer1(new SystemOutWriter());
		setWriter(writer1);
	}
	else if (StringHelper::equalsIgnoreCase(target,
			LOG4CXXNG_STR("SYSTEM.ERR"), LOG4CXXNG_STR("system.err")))
	{
		WriterPtr writer1(new SystemErrWriter());
		setWriter(writer1);
	}

	WriterAppender::activateOptions(p);
}

void ConsoleAppender::setOption(const LogString& option, const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("TARGET"), LOG4CXXNG_STR("target")))
	{
		setTarget(value);
	}
	else
	{
		WriterAppender::setOption(option, value);
	}
}






