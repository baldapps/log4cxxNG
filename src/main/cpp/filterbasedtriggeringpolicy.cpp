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
#include <log4cxxNG/rolling/filterbasedtriggeringpolicy.h>
#include <log4cxxNG/spi/filter.h>

using namespace log4cxxng;
using namespace log4cxxng::rolling;
using namespace log4cxxng::spi;

IMPLEMENT_LOG4CXXNG_OBJECT(FilterBasedTriggeringPolicy)

FilterBasedTriggeringPolicy::FilterBasedTriggeringPolicy()
{
}


FilterBasedTriggeringPolicy::~FilterBasedTriggeringPolicy()
{
}


bool FilterBasedTriggeringPolicy::isTriggeringEvent(
	Appender* /* appender */,
	const log4cxxng::spi::LoggingEventPtr& event,
	const LogString& /* filename */,
	size_t /* fileLength */ )
{
	if (headFilter == NULL)
	{
		return false;
	}

	for (log4cxxng::spi::FilterPtr f = headFilter; f != NULL; f = f->getNext())
	{
		switch (f->decide(event))
		{
			case Filter::DENY:
				return false;

			case Filter::ACCEPT:
				return true;

			case Filter::NEUTRAL:
				break;
		}
	}

	return true;
}

/**
 * Add a filter to end of the filter list.
 * @param newFilter filter to add to end of list.
 */
void FilterBasedTriggeringPolicy::addFilter(const log4cxxng::spi::FilterPtr& newFilter)
{
	if (headFilter == NULL)
	{
		headFilter = newFilter;
		tailFilter = newFilter;
	}
	else
	{
		tailFilter->setNext(newFilter);
		tailFilter = newFilter;
	}
}

void FilterBasedTriggeringPolicy::clearFilters()
{
	log4cxxng::spi::FilterPtr empty;
	headFilter = empty;
	tailFilter = empty;
}

log4cxxng::spi::FilterPtr& FilterBasedTriggeringPolicy::getFilter()
{
	return headFilter;
}

/**
 *  Prepares the instance for use.
 */
void FilterBasedTriggeringPolicy::activateOptions(log4cxxng::helpers::Pool& p)
{
	for (log4cxxng::spi::FilterPtr f = headFilter; f != NULL; f = f->getNext())
	{
		f->activateOptions(p);
	}
}

void FilterBasedTriggeringPolicy::setOption(const LogString& /* option */, const LogString& /* value */ )
{
}


