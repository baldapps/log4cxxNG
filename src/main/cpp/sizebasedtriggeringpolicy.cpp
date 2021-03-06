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

#include <log4cxxNG/rolling/sizebasedtriggeringpolicy.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/optionconverter.h>

using namespace log4cxxng;
using namespace log4cxxng::rolling;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(SizeBasedTriggeringPolicy)

SizeBasedTriggeringPolicy::SizeBasedTriggeringPolicy()
	: maxFileSize(10 * 1024 * 1024)
{
}

bool SizeBasedTriggeringPolicy::isTriggeringEvent(Appender* /* appender */,
	const log4cxxng::spi::LoggingEventPtr& /* event */,
	const LogString& /* file */,
	size_t fileLength)
{
	return (fileLength >= maxFileSize);
}

size_t SizeBasedTriggeringPolicy::getMaxFileSize()
{
	return maxFileSize;
}

void SizeBasedTriggeringPolicy::setMaxFileSize(size_t l)
{
	maxFileSize = l;
}

void SizeBasedTriggeringPolicy::activateOptions(Pool& /* p */)
{
}

void SizeBasedTriggeringPolicy::setOption(const LogString& option, const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("MAXFILESIZE"),
			LOG4CXXNG_STR("maxfilesize")))
	{
		maxFileSize = OptionConverter::toFileSize(value, 10 * 1024 * 1024);
	}
}
