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
#define __STDC_CONSTANT_MACROS
#include <log4cxxNG/logstring.h>
#include <log4cxxNG/helpers/relativetimedateformat.h>
#include <log4cxxNG/spi/loggingevent.h>

#include <apr.h>
#include <apr_time.h>
#include <log4cxxNG/helpers/stringhelper.h>


log4cxxng::helpers::RelativeTimeDateFormat::RelativeTimeDateFormat()
	: DateFormat(), startTime(log4cxxng::spi::LoggingEvent::getStartTime())
{
}

void log4cxxng::helpers::RelativeTimeDateFormat::format(
	LogString& s,
	log4cxxng_time_t date,
	Pool& p) const
{
	log4cxxng_int64_t interval = (date - startTime) / APR_INT64_C(1000);
	StringHelper::toString(interval, p, s);
}
