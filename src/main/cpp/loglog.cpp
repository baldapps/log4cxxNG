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
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/transcoder.h>
#include <iostream>
#if !defined(LOG4CXXNG)
	#define LOG4CXXNG 1
#endif
#include <log4cxxNG/private/log4cxxNG_private.h>
#include <log4cxxNG/helpers/synchronized.h>
#include <log4cxxNG/helpers/aprinitializer.h>
#include <log4cxxNG/helpers/systemerrwriter.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;

LogLog::LogLog() : mutex(APRInitializer::getRootPool())
{
	synchronized sync(mutex);

	debugEnabled	= false;
	quietMode		= false;
}

LogLog& LogLog::getInstance()
{
	static LogLog internalLogger;

	return internalLogger;
}

void LogLog::setInternalDebugging(bool debugEnabled1)
{
	synchronized sync(getInstance().mutex);

	getInstance().debugEnabled = debugEnabled1;
}

void LogLog::debug(const LogString& msg)
{
	if (!getInstance().debugEnabled)
	{
		return;
	}

	synchronized sync(getInstance().mutex);

	emit(msg);
}

void LogLog::debug(const LogString& msg, const std::exception& e)
{
	if (!getInstance().debugEnabled)
	{
		return;
	}

	synchronized sync(getInstance().mutex);

	debug(msg);
	emit(e);
}


void LogLog::error(const LogString& msg)
{
	synchronized sync(getInstance().mutex);

	emit(msg);
}

void LogLog::error(const LogString& msg, const std::exception& e)
{
	synchronized sync(getInstance().mutex);

	error(msg);
	emit(e);
}

void LogLog::setQuietMode(bool quietMode1)
{
	synchronized sync(getInstance().mutex);

	getInstance().quietMode = quietMode1;
}

void LogLog::warn(const LogString& msg)
{
	synchronized sync(getInstance().mutex);

	emit(msg);
}

void LogLog::warn(const LogString& msg, const std::exception& e)
{
	synchronized sync(getInstance().mutex);

	warn(msg);
	emit(e);
}

void LogLog::emit(const LogString& msg)
{
	if (getInstance().quietMode) {
		return;
	}

	LogString out(LOG4CXX_STR("log4cxx: "));

	out.append(msg);
	out.append(1, (logchar) 0x0A);

	SystemErrWriter::write(out);
}

void LogLog::emit(const std::exception& ex)
{
	if (getInstance().quietMode) {
		return;
	}

	LogString out(LOG4CXX_STR("log4cxx: "));
	const char* raw = ex.what();

	if (raw != 0)
	{
		Transcoder::decode(raw, out);
	}
	else
	{
		out.append(LOG4CXXNG_STR("std::exception::what() == null"));
	}

	out.append(1, (logchar) 0x0A);

	SystemErrWriter::write(out);
}
