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
#include <log4cxxNG/dailyrollingfileappender.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/optionconverter.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/rolling/rollingfileappender.h>
#include <log4cxxNG/rolling/timebasedrollingpolicy.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::rolling;

IMPLEMENT_LOG4CXXNG_OBJECT(DailyRollingFileAppender)



DailyRollingFileAppender::DailyRollingFileAppender()
{
}


DailyRollingFileAppender::DailyRollingFileAppender(
	const LayoutPtr& l,
	const LogString& filename,
	const LogString& datePattern1)
	: datePattern(datePattern1)
{
	setLayout(l);
	setFile(filename);
	Pool p;
	activateOptions(p);
}

void DailyRollingFileAppender::setDatePattern(const LogString& newPattern)
{
	datePattern = newPattern;
}

LogString DailyRollingFileAppender::getDatePattern() const
{
	return datePattern;
}

void DailyRollingFileAppender::activateOptions(log4cxxng::helpers::Pool& p)
{
	TimeBasedRollingPolicyPtr policy = new TimeBasedRollingPolicy();
	LogString pattern(getFile());
	bool inLiteral = false;
	bool inPattern = false;

	for (size_t i = 0; i < datePattern.length(); i++)
	{
		if (datePattern[i] == 0x27 /* '\'' */)
		{
			inLiteral = !inLiteral;

			if (inLiteral && inPattern)
			{
				pattern.append(1, (logchar) 0x7D /* '}' */);
				inPattern = false;
			}
		}
		else
		{
			if (!inLiteral && !inPattern)
			{
				const logchar dbrace[] = { 0x25, 0x64, 0x7B, 0 }; // "%d{"
				pattern.append(dbrace);
				inPattern = true;
			}

			pattern.append(1, datePattern[i]);
		}
	}

	if (inPattern)
	{
		pattern.append(1, (logchar) 0x7D /* '}' */);
	}

	policy->setFileNamePattern(pattern);
	policy->activateOptions(p);
	setTriggeringPolicy(policy);
	setRollingPolicy(policy);

	RollingFileAppenderSkeleton::activateOptions(p);
}


void DailyRollingFileAppender::setOption(const LogString& option,
	const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("DATEPATTERN"), LOG4CXXNG_STR("datepattern")))
	{
		setDatePattern(value);
	}
	else
	{
		RollingFileAppenderSkeleton::setOption(option, value);
	}
}




