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
#include <log4cxxNG/defaultconfigurator.h>
#include <log4cxxNG/helpers/pool.h>
#include <log4cxxNG/spi/loggerrepository.h>
#include <log4cxxNG/file.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/optionconverter.h>


using namespace log4cxxng;
using namespace log4cxxng::spi;
using namespace log4cxxng::helpers;

void DefaultConfigurator::configure(LoggerRepository* repository)
{
	repository->setConfigured(true);
	const LogString configuratorClassName(getConfiguratorClass());

	LogString configurationOptionStr(getConfigurationFileName());
	Pool pool;
	File configuration;

	if (configurationOptionStr.empty())
	{
		const char* names[] = { "log4cxx.xml", "log4cxx.properties", "log4j.xml", "log4j.properties", 0 };

		for (int i = 0; names[i] != 0; i++)
		{
			File candidate(names[i]);

			if (candidate.exists(pool))
			{
				configuration = candidate;
				break;
			}
		}
	}
	else
	{
		configuration.setPath(configurationOptionStr);
	}

	if (configuration.exists(pool))
	{
		LogString msg(LOG4CXXNG_STR("Using configuration file ["));
		msg += configuration.getPath();
		msg += LOG4CXXNG_STR("] for automatic log4cxx configuration");
		LogLog::debug(msg);

		LoggerRepositoryPtr repo(repository);
		OptionConverter::selectAndConfigure(
			configuration,
			configuratorClassName,
			repo);
	}
	else
	{
		if (configurationOptionStr.empty())
		{
			LogLog::debug(LOG4CXXNG_STR("Could not find default configuration file."));
		}
		else
		{
			LogString msg(LOG4CXXNG_STR("Could not find configuration file: ["));
			msg += configurationOptionStr;
			msg += LOG4CXXNG_STR("].");
			LogLog::debug(msg);
		}
	}

}


const LogString DefaultConfigurator::getConfiguratorClass()
{

	// Use automatic configration to configure the default hierarchy
	const LogString log4jConfiguratorClassName(
		OptionConverter::getSystemProperty(LOG4CXXNG_STR("log4j.configuratorClass"), LOG4CXXNG_STR("")));
	const LogString configuratorClassName(
		OptionConverter::getSystemProperty(LOG4CXXNG_STR("LOG4CXXNG_CONFIGURATOR_CLASS"),
			log4jConfiguratorClassName));
	return configuratorClassName;
}


const LogString DefaultConfigurator::getConfigurationFileName()
{
	static const LogString LOG4CXXNG_DEFAULT_CONFIGURATION_KEY(LOG4CXXNG_STR("LOG4CXXNG_CONFIGURATION"));
	static const LogString LOG4J_DEFAULT_CONFIGURATION_KEY(LOG4CXXNG_STR("log4j.configuration"));
	const LogString log4jConfigurationOptionStr(
		OptionConverter::getSystemProperty(LOG4J_DEFAULT_CONFIGURATION_KEY, LOG4CXXNG_STR("")));
	const LogString configurationOptionStr(
		OptionConverter::getSystemProperty(LOG4CXXNG_DEFAULT_CONFIGURATION_KEY,
			log4jConfigurationOptionStr));
	return configurationOptionStr;
}




