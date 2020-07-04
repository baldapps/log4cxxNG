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

#if !defined(_LOG4CXXNG_ROLLING_ROLLING_POLICY_BASE_H)
#define _LOG4CXXNG_ROLLING_ROLLING_POLICY_BASE_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif


#include <log4cxxNG/helpers/object.h>
#include <log4cxxNG/logger.h>
#include <log4cxxNG/logmanager.h>
#include <log4cxxNG/rolling/rollingpolicy.h>
#include <log4cxxNG/pattern/patternconverter.h>
#include <log4cxxNG/pattern/formattinginfo.h>
#include <log4cxxNG/pattern/patternparser.h>

namespace log4cxxng
{
namespace rolling
{
LOG4CXXNG_LIST_DEF(PatternConverterList, log4cxxng::pattern::PatternConverterPtr);
LOG4CXXNG_LIST_DEF(FormattingInfoList, log4cxxng::pattern::FormattingInfoPtr);

/**
 * Implements methods common to most, it not all, rolling
 * policies.
 *
 *
 *
 */
class LOG4CXXNG_EXPORT RollingPolicyBase :
	public virtual RollingPolicy,
	public virtual helpers::ObjectImpl
{
	protected:
		DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(RollingPolicyBase)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(RollingPolicy)
		LOG4CXXNG_CAST_ENTRY(spi::OptionHandler)
		END_LOG4CXXNG_CAST_MAP()


	private:
		/**
		 * File name pattern converters.
		 */
		PatternConverterList patternConverters;

		/**
		 * File name field specifiers.
		 */
		FormattingInfoList patternFields;

		/**
		 * File name pattern.
		 */
		LogString fileNamePatternStr;


	public:
		RollingPolicyBase();
		virtual ~RollingPolicyBase();
		void addRef() const;
		void releaseRef() const;
		virtual void activateOptions(log4cxxng::helpers::Pool& p) = 0;
		virtual log4cxxng::pattern::PatternMap getFormatSpecifiers() const = 0;

		virtual void setOption(const LogString& option,
			const LogString& value);

		/**
		 * Set file name pattern.
		 * @param fnp file name pattern.
		 */
		void setFileNamePattern(const LogString& fnp);

		/**
		 * Get file name pattern.
		 * @return file name pattern.
		 */
		LogString getFileNamePattern() const;


#ifdef LOG4CXXNG_MULTI_PROCESS
		PatternConverterList getPatternConverterList()
		{
			return patternConverters;
		}
#endif
	protected:
		/**
		 *   Parse file name pattern.
		 */
		void parseFileNamePattern();

		/**
		 * Format file name.
		 *
		 * @param obj object to be evaluted in formatting, may not be null.
		 * @param buf string buffer to which formatted file name is appended, may not be null.
		 * @param p memory pool.
		 */
		void formatFileName(log4cxxng::helpers::ObjectPtr& obj,
			LogString& buf, log4cxxng::helpers::Pool& p) const;

		log4cxxng::pattern::PatternConverterPtr getIntegerPatternConverter() const;
		log4cxxng::pattern::PatternConverterPtr getDatePatternConverter() const;


};
}
}


#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif

#endif
