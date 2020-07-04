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
#include <log4cxxNG/rolling/rollingpolicybase.h>
#include <log4cxxNG/pattern/formattinginfo.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/pattern/patternparser.h>
#include <log4cxxNG/pattern/integerpatternconverter.h>
#include <log4cxxNG/pattern/datepatternconverter.h>

using namespace log4cxxng;
using namespace log4cxxng::rolling;
using namespace log4cxxng::helpers;
using namespace log4cxxng::pattern;

IMPLEMENT_LOG4CXXNG_OBJECT(RollingPolicyBase)

RollingPolicyBase::RollingPolicyBase()
{
}

RollingPolicyBase::~RollingPolicyBase()
{
}

void RollingPolicyBase::addRef() const
{
	ObjectImpl::addRef();
}

void RollingPolicyBase::releaseRef() const
{
	ObjectImpl::releaseRef();
}

void RollingPolicyBase::activateOptions(log4cxxng::helpers::Pool& /* pool */)
{
	if (fileNamePatternStr.length() > 0)
	{
		parseFileNamePattern();
	}
	else
	{
		LogString msg(LOG4CXXNG_STR("The FileNamePattern option must be set before using FixedWindowRollingPolicy."));
		LogString ref1(LOG4CXXNG_STR("See also http://logging.apache.org/log4j/codes.html#tbr_fnp_not_set"));
		LogLog::warn(msg);
		LogLog::warn(ref1);
		throw IllegalStateException();
	}
}


void RollingPolicyBase::setOption(const LogString& option, const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("FILENAMEPATTERN"),
			LOG4CXXNG_STR("filenamepattern")))
	{
		fileNamePatternStr = value;
	}
}

void RollingPolicyBase::setFileNamePattern(const LogString& fnp)
{
	fileNamePatternStr = fnp;
}


LogString RollingPolicyBase::getFileNamePattern() const
{
	return fileNamePatternStr;
}

/**
 *   Parse file name pattern.
 */
void RollingPolicyBase::parseFileNamePattern()
{
	patternConverters.erase(patternConverters.begin(), patternConverters.end());
	patternFields.erase(patternFields.begin(), patternFields.end());
	PatternParser::parse(fileNamePatternStr,
		patternConverters,
		patternFields,
		getFormatSpecifiers());
}

/**
 * Format file name.
 *
 * @param obj object to be evaluted in formatting, may not be null.
 * @param buf string buffer to which formatted file name is appended, may not be null.
 */
void RollingPolicyBase::formatFileName(
	ObjectPtr& obj,
	LogString& toAppendTo,
	Pool& pool) const
{
	std::vector<FormattingInfoPtr>::const_iterator formatterIter =
		patternFields.begin();

	for (std::vector<PatternConverterPtr>::const_iterator
		converterIter = patternConverters.begin();
		converterIter != patternConverters.end();
		converterIter++, formatterIter++)
	{
		int startField = toAppendTo.length();
		(*converterIter)->format(obj, toAppendTo, pool);
		(*formatterIter)->format(startField, toAppendTo);
	}
}


PatternConverterPtr RollingPolicyBase::getIntegerPatternConverter() const
{
	for (std::vector<PatternConverterPtr>::const_iterator
		converterIter = patternConverters.begin();
		converterIter != patternConverters.end();
		converterIter++)
	{
		IntegerPatternConverterPtr intPattern(*converterIter);

		if (intPattern != NULL)
		{
			return *converterIter;
		}
	}

	PatternConverterPtr noMatch;
	return noMatch;
}

PatternConverterPtr RollingPolicyBase::getDatePatternConverter() const
{
	for (std::vector<PatternConverterPtr>::const_iterator
		converterIter = patternConverters.begin();
		converterIter != patternConverters.end();
		converterIter++)
	{
		DatePatternConverterPtr datePattern(*converterIter);

		if (datePattern != NULL)
		{
			return *converterIter;
		}
	}

	PatternConverterPtr noMatch;
	return noMatch;
}


