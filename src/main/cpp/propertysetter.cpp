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
#include <log4cxxNG/config/propertysetter.h>
#include <log4cxxNG/helpers/object.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/optionconverter.h>
#include <log4cxxNG/spi/optionhandler.h>
#include <log4cxxNG/helpers/properties.h>
#include <log4cxxNG/appender.h>
#include <log4cxxNG/layout.h>
#include <log4cxxNG/helpers/pool.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::spi;
using namespace log4cxxng::config;

PropertySetter::PropertySetter(const helpers::ObjectPtr& obj1) : obj(obj1)
{
}

void PropertySetter::setProperties(const helpers::ObjectPtr& obj,
	helpers::Properties& properties,
	const LogString& prefix,
	Pool& p)
{
	PropertySetter(obj).setProperties(properties, prefix, p);
}


void PropertySetter::setProperties(helpers::Properties& properties,
	const LogString& prefix,
	Pool& p)
{
	size_t len = prefix.length();

	std::vector<LogString> names = properties.propertyNames();
	std::vector<LogString>::iterator it;

	for (it = names.begin(); it != names.end(); it++)
	{
		LogString key = *it;

		// handle only properties that start with the desired frefix.
		if (key.find(prefix) == 0)
		{
			// ignore key if it contains dots after the prefix
			if (key.find(0x2E /* '.' */, len + 1) != LogString::npos)
			{
				continue;
			}

			LogString value = OptionConverter::findAndSubst(key, properties);
			key = key.substr(len);

			if (key == LOG4CXXNG_STR("layout")
				&& obj != 0
				&& obj->instanceof(Appender::getStaticClass()))
			{
				continue;
			}

			setProperty(key, value, p);
		}
	}

	activate(p);
}

void PropertySetter::setProperty(const LogString& option,
	const LogString& value,
	Pool&)
{
	if (value.empty())
	{
		return;
	}

	if (obj != 0 && obj->instanceof(OptionHandler::getStaticClass()))
	{
		LogLog::debug(LOG4CXXNG_STR("Setting option name=[") +
			option + LOG4CXXNG_STR("], value=[") + value + LOG4CXXNG_STR("]"));
		OptionHandlerPtr(obj)->setOption(option, value);
	}
}

void PropertySetter::activate(Pool& p)
{
	if (obj != 0 && obj->instanceof(OptionHandler::getStaticClass()))
	{
		OptionHandlerPtr(obj)->activateOptions(p);
	}
}
