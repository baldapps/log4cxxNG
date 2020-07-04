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
#include <log4cxxNG/helpers/appenderattachableimpl.h>
#include <log4cxxNG/appender.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <algorithm>
#include <log4cxxNG/helpers/pool.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::spi;

IMPLEMENT_LOG4CXXNG_OBJECT(AppenderAttachableImpl)


AppenderAttachableImpl::AppenderAttachableImpl(Pool& pool)
	: appenderList(),
	  mutex(pool)
{
}

void AppenderAttachableImpl::addRef() const
{
	ObjectImpl::addRef();
}

void AppenderAttachableImpl::releaseRef() const
{
	ObjectImpl::releaseRef();
}


void AppenderAttachableImpl::addAppender(const AppenderPtr& newAppender)
{
	// Null values for newAppender parameter are strictly forbidden.
	if (newAppender == 0)
	{
		return;
	}

	AppenderList::iterator it = std::find(
			appenderList.begin(), appenderList.end(), newAppender);

	if (it == appenderList.end())
	{
		appenderList.push_back(newAppender);
	}
}

int AppenderAttachableImpl::appendLoopOnAppenders(
	const spi::LoggingEventPtr& event,
	Pool& p)
{
	for (AppenderList::iterator it = appenderList.begin();
		it != appenderList.end();
		it++)
	{
		(*it)->doAppend(event, p);
	}

	return (int)appenderList.size();
}

AppenderList AppenderAttachableImpl::getAllAppenders() const
{
	return appenderList;
}

AppenderPtr AppenderAttachableImpl::getAppender(const LogString& name) const
{
	if (name.empty())
	{
		return 0;
	}

	AppenderList::const_iterator it, itEnd = appenderList.end();
	AppenderPtr appender;

	for (it = appenderList.begin(); it != itEnd; it++)
	{
		appender = *it;

		if (name == appender->getName())
		{
			return appender;
		}
	}

	return 0;
}

bool AppenderAttachableImpl::isAttached(const AppenderPtr& appender) const
{
	if (appender == 0)
	{
		return false;
	}

	AppenderList::const_iterator it = std::find(
			appenderList.begin(), appenderList.end(), appender);

	return it != appenderList.end();
}

void AppenderAttachableImpl::removeAllAppenders()
{
	AppenderList::iterator it, itEnd = appenderList.end();
	AppenderPtr a;

	for (it = appenderList.begin(); it != itEnd; it++)
	{
		a = *it;
		a->close();
	}

	appenderList.clear();
}

void AppenderAttachableImpl::removeAppender(const AppenderPtr& appender)
{
	if (appender == 0)
	{
		return;
	}

	AppenderList::iterator it = std::find(
			appenderList.begin(), appenderList.end(), appender);

	if (it != appenderList.end())
	{
		appenderList.erase(it);
	}
}

void AppenderAttachableImpl::removeAppender(const LogString& name)
{
	if (name.empty())
	{
		return;
	}

	AppenderList::iterator it, itEnd = appenderList.end();
	AppenderPtr appender;

	for (it = appenderList.begin(); it != itEnd; it++)
	{
		appender = *it;

		if (name == appender->getName())
		{
			appenderList.erase(it);
			return;
		}
	}
}


