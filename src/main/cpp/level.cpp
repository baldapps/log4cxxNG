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
#include <log4cxxNG/level.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/transcoder.h>
#if !defined(LOG4CXXNG)
	#define LOG4CXXNG 1
#endif
#include <log4cxxNG/helpers/aprinitializer.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT_WITH_CUSTOM_CLASS(Level, LevelClass)

LevelPtr Level::getOff()
{
    static LevelPtr level(new Level(Level::OFF_INT, LOG4CXXNG_STR("OFF"), 0));
	return level;
}

LevelPtr Level::getFatal()
{
	static LevelPtr level(new Level(Level::FATAL_INT, LOG4CXXNG_STR("FATAL"), 0));
	return level;
}

LevelPtr Level::getError()
{
	static LevelPtr level(new Level(Level::ERROR_INT, LOG4CXXNG_STR("ERROR"), 3));
	return level;
}

LevelPtr Level::getWarn()
{
    static LevelPtr level(new Level(Level::WARN_INT, LOG4CXXNG_STR("WARN"), 4));
    return level;
}

LevelPtr Level::getInfo()
{
	static LevelPtr level(new Level(Level::INFO_INT, LOG4CXXNG_STR("INFO"), 6));
	return level;
}

LevelPtr Level::getDebug()
{
	static LevelPtr level(new Level(Level::DEBUG_INT, LOG4CXXNG_STR("DEBUG"), 7));
	return level;
}

LevelPtr Level::getTrace()
{
	static LevelPtr level(new Level(Level::TRACE_INT, LOG4CXXNG_STR("TRACE"), 7));
	return level;
}


LevelPtr Level::getAll()
{
	static LevelPtr level(new Level(Level::ALL_INT, LOG4CXXNG_STR("ALL"), 7));
	return level;
}



Level::Level(int level1,
	const LogString& name1, int syslogEquivalent1)
	: level(level1), name(name1), syslogEquivalent(syslogEquivalent1)
{
	APRInitializer::initialize();
}


LevelPtr Level::toLevelLS(const LogString& sArg)
{
	return toLevelLS(sArg, Level::getDebug());
}

LogString Level::toString() const
{
	return name;
}


LevelPtr Level::toLevel(int val)
{
	return toLevel(val, Level::getDebug());
}

LevelPtr Level::toLevel(int val, const LevelPtr& defaultLevel)
{
	switch (val)
	{
		case ALL_INT:
			return getAll();

		case DEBUG_INT:
			return getDebug();

		case TRACE_INT:
			return getTrace();

		case INFO_INT:
			return getInfo();

		case WARN_INT:
			return getWarn();

		case ERROR_INT:
			return getError();

		case FATAL_INT:
			return getFatal();

		case OFF_INT:
			return getOff();

		default:
			return defaultLevel;
	}
}

LevelPtr Level::toLevel(const std::string& sArg)
{
	return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::string& sArg, const LevelPtr& defaultLevel)
{
	LOG4CXXNG_DECODE_CHAR(s, sArg);
	return toLevelLS(s, defaultLevel);
}

void Level::toString(std::string& dst) const
{
	Transcoder::encode(name, dst);
}

#if LOG4CXXNG_WCHAR_T_API
LevelPtr Level::toLevel(const std::wstring& sArg)
{
	return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::wstring& sArg, const LevelPtr& defaultLevel)
{
	LOG4CXXNG_DECODE_WCHAR(s, sArg);
	return toLevelLS(s, defaultLevel);
}

void Level::toString(std::wstring& dst) const
{
	Transcoder::encode(name, dst);
}

#endif

#if LOG4CXXNG_UNICHAR_API
LevelPtr Level::toLevel(const std::basic_string<UniChar>& sArg)
{
	return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::basic_string<UniChar>& sArg, const LevelPtr& defaultLevel)
{
	LOG4CXXNG_DECODE_UNICHAR(s, sArg);
	return toLevelLS(s, defaultLevel);
}

void Level::toString(std::basic_string<UniChar>& dst) const
{
	Transcoder::encode(name, dst);
}

#endif

#if LOG4CXXNG_CFSTRING_API
LevelPtr Level::toLevel(const CFStringRef& sArg)
{
	return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const CFStringRef& sArg, const LevelPtr& defaultLevel)
{
	LogString s;
	Transcoder::decode(sArg, s);
	return toLevelLS(s, defaultLevel);
}

void Level::toString(CFStringRef& dst) const
{
	dst = Transcoder::encode(name);
}
#endif


LevelPtr Level::toLevelLS(const LogString& sArg, const LevelPtr& defaultLevel)
{
	const LogString trimmed(StringHelper::trim(sArg));
	const size_t len = trimmed.length();

	if (len == 4)
	{
		if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("INFO"), LOG4CXXNG_STR("info")))
		{
			return getInfo();
		}

		if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("WARN"), LOG4CXXNG_STR("warn")))
		{
			return getWarn();
		}
	}
	else
	{
		if (len == 5)
		{
			if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("DEBUG"), LOG4CXXNG_STR("debug")))
			{
				return getDebug();
			}

			if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("TRACE"), LOG4CXXNG_STR("trace")))
			{
				return getTrace();
			}

			if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("ERROR"), LOG4CXXNG_STR("error")))
			{
				return getError();
			}

			if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("FATAL"), LOG4CXXNG_STR("fatal")))
			{
				return getFatal();
			}
		}
		else
		{
			if (len == 3)
			{
				if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("OFF"), LOG4CXXNG_STR("off")))
				{
					return getOff();
				}

				if (StringHelper::equalsIgnoreCase(trimmed, LOG4CXXNG_STR("ALL"), LOG4CXXNG_STR("all")))
				{
					return getAll();
				}
			}
		}
	}

	return defaultLevel;
}


bool Level::equals(const LevelPtr& level1) const
{
	return (this->level == level1->level);
}

bool Level::isGreaterOrEqual(const LevelPtr& level1) const
{
	return this->level >= level1->level;
}

