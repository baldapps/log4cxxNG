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
#include <log4cxxNG/rollingfileappender.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/optionconverter.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/rolling/rollingfileappenderskeleton.h>
#include <log4cxxNG/rolling/sizebasedtriggeringpolicy.h>
#include <log4cxxNG/rolling/fixedwindowrollingpolicy.h>


using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::spi;

namespace log4cxxng
{
class ClassRollingFileAppender : public Class
{
	public:
		ClassRollingFileAppender() : helpers::Class() {}
		virtual LogString getName() const
		{
			return LOG4CXXNG_STR("org.apache.log4j.RollingFileAppender");
		}
		virtual ObjectPtr newInstance() const
		{
			return new RollingFileAppender();
		}
};
}

const log4cxxng::helpers::Class& RollingFileAppender::getClass() const
{
	return getStaticClass();
}
const log4cxxng::helpers::Class& RollingFileAppender::getStaticClass()
{
	static ClassRollingFileAppender theClass;
	return theClass;
}
const log4cxxng::helpers::ClassRegistration& RollingFileAppender::registerClass()
{
	static log4cxxng::helpers::ClassRegistration classReg(RollingFileAppender::getStaticClass);
	return classReg;
}
namespace log4cxxng
{
namespace classes
{
const log4cxxng::helpers::ClassRegistration& ObsoleteRollingFileAppenderRegistration =
	RollingFileAppender::registerClass();
}
}



RollingFileAppender::RollingFileAppender()
	: maxFileSize(10 * 1024 * 1024), maxBackupIndex(1)
{
}

RollingFileAppender::RollingFileAppender(
	const LayoutPtr& newLayout,
	const LogString& filename,
	bool append)
	: maxFileSize(10 * 1024 * 1024), maxBackupIndex(1)
{
	setLayout(newLayout);
	setFile(filename);
	setAppend(append);
	Pool p;
	activateOptions(p);
}

RollingFileAppender::RollingFileAppender(const LayoutPtr& newLayout,
	const LogString& filename)
	: maxFileSize(10 * 1024 * 1024), maxBackupIndex(1)
{
	setLayout(newLayout);
	setFile(filename);
	Pool p;
	activateOptions(p);
}

RollingFileAppender::~RollingFileAppender()
{
}


void RollingFileAppender::setOption(const LogString& option,
	const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("MAXFILESIZE"), LOG4CXXNG_STR("maxfilesize"))
		|| StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("MAXIMUMFILESIZE"), LOG4CXXNG_STR("maximumfilesize")))
	{
		setMaxFileSize(value);
	}
	else if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("MAXBACKUPINDEX"), LOG4CXXNG_STR("maxbackupindex"))
		|| StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("MAXIMUMBACKUPINDEX"), LOG4CXXNG_STR("maximumbackupindex")))
	{
		maxBackupIndex = StringHelper::toInt(value);
	}
	else
	{
		using namespace log4cxxng::rolling;
		RollingFileAppenderSkeleton::setOption(option, value);
	}
}


int RollingFileAppender::getMaxBackupIndex() const
{
	return maxBackupIndex;
}

long RollingFileAppender::getMaximumFileSize() const
{
	return maxFileSize;
}

void RollingFileAppender::setMaxBackupIndex(int maxBackups)
{
	maxBackupIndex = maxBackups;
}

void RollingFileAppender::setMaximumFileSize(int maxFileSize1)
{
	maxFileSize = maxFileSize1;
}

void RollingFileAppender::setMaxFileSize(const LogString& value)
{
	maxFileSize = OptionConverter::toFileSize(value, maxFileSize + 1);
}

void RollingFileAppender::activateOptions(Pool& p)
{
	log4cxxng::rolling::SizeBasedTriggeringPolicyPtr trigger(
		new log4cxxng::rolling::SizeBasedTriggeringPolicy());
	trigger->setMaxFileSize(maxFileSize);
	trigger->activateOptions(p);
	setTriggeringPolicy(trigger);

	log4cxxng::rolling::FixedWindowRollingPolicyPtr rolling(
		new log4cxxng::rolling::FixedWindowRollingPolicy());
	rolling->setMinIndex(1);
	rolling->setMaxIndex(maxBackupIndex);
	rolling->setFileNamePattern(getFile() + LOG4CXXNG_STR(".%i"));
	rolling->activateOptions(p);
	setRollingPolicy(rolling);

	using namespace log4cxxng::rolling;
	RollingFileAppenderSkeleton::activateOptions(p);
}


