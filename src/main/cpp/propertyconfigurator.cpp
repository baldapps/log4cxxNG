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
#include <log4cxxNG/propertyconfigurator.h>
#include <log4cxxNG/spi/loggerfactory.h>
#include <log4cxxNG/helpers/properties.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/logmanager.h>
#include <log4cxxNG/helpers/optionconverter.h>
#include <log4cxxNG/level.h>
#include <log4cxxNG/defaultloggerfactory.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/appender.h>
#include <log4cxxNG/logger.h>
#include <log4cxxNG/layout.h>
#include <log4cxxNG/config/propertysetter.h>
#include <log4cxxNG/spi/loggerrepository.h>
#include <log4cxxNG/helpers/stringtokenizer.h>
#include <log4cxxNG/helpers/synchronized.h>
#include <apr_file_io.h>
#include <apr_file_info.h>
#include <apr_pools.h>
#include <log4cxxNG/helpers/transcoder.h>
#include <log4cxxNG/helpers/fileinputstream.h>

#define LOG4CXXNG 1
#include <log4cxxNG/helpers/aprinitializer.h>


using namespace log4cxxng;
using namespace log4cxxng::spi;
using namespace log4cxxng::helpers;
using namespace log4cxxng::config;


#if APR_HAS_THREADS
#include <log4cxxNG/helpers/filewatchdog.h>
namespace log4cxxng
{
class PropertyWatchdog  : public FileWatchdog
{
	public:
		PropertyWatchdog(const File& filename) : FileWatchdog(filename)
		{
		}

		/**
		Call PropertyConfigurator#doConfigure(const String& configFileName,
		const spi::LoggerRepositoryPtr& hierarchy) with the
		<code>filename</code> to reconfigure log4cxx.
		*/
		void doOnChange()
		{
			PropertyConfigurator().doConfigure(file,
				LogManager::getLoggerRepository());
		}
};
}

PropertyWatchdog* PropertyConfigurator::pdog = NULL;

#endif

IMPLEMENT_LOG4CXXNG_OBJECT(PropertyConfigurator)

PropertyConfigurator::PropertyConfigurator()
	: registry(new std::map<LogString, AppenderPtr>()), loggerFactory(new DefaultLoggerFactory())
{
}

PropertyConfigurator::~PropertyConfigurator()
{
	delete registry;
}

void PropertyConfigurator::addRef() const
{
	ObjectImpl::addRef();
}

void PropertyConfigurator::releaseRef() const
{
	ObjectImpl::releaseRef();
}

void PropertyConfigurator::doConfigure(const File& configFileName,
	spi::LoggerRepositoryPtr& hierarchy)
{
	hierarchy->setConfigured(true);

	Properties props;

	try
	{
		InputStreamPtr inputStream = new FileInputStream(configFileName);
		props.load(inputStream);
	}
	catch (const IOException&)
	{
		LogLog::error(((LogString) LOG4CXXNG_STR("Could not read configuration file ["))
			+ configFileName.getPath() + LOG4CXXNG_STR("]."));
		return;
	}

	try
	{
		doConfigure(props, hierarchy);
	}
	catch (const std::exception& ex)
	{
		LogLog::error(((LogString) LOG4CXXNG_STR("Could not parse configuration file ["))
			+ configFileName.getPath() + LOG4CXXNG_STR("]."), ex);
	}
}

void PropertyConfigurator::configure(const File& configFilename)
{
	PropertyConfigurator().doConfigure(configFilename, LogManager::getLoggerRepository());
}

void PropertyConfigurator::configure(helpers::Properties& properties)
{
	PropertyConfigurator().doConfigure(properties, LogManager::getLoggerRepository());
}

#if APR_HAS_THREADS
void PropertyConfigurator::configureAndWatch(const File& configFilename)
{
	configureAndWatch(configFilename, FileWatchdog::DEFAULT_DELAY);
}



void PropertyConfigurator::configureAndWatch(
	const File& configFilename, long delay)
{
	if (pdog)
	{
		APRInitializer::unregisterCleanup(pdog);
		delete pdog;
	}

	pdog = new PropertyWatchdog(configFilename);
	APRInitializer::registerCleanup(pdog);
	pdog->setDelay(delay);
	pdog->start();
}
#endif

void PropertyConfigurator::doConfigure(helpers::Properties& properties,
	spi::LoggerRepositoryPtr& hierarchy)
{
	hierarchy->setConfigured(true);

	static const LogString DEBUG_KEY(LOG4CXXNG_STR("log4j.debug"));
	LogString value(properties.getProperty(DEBUG_KEY));

	if (!value.empty())
	{
		LogLog::setInternalDebugging(OptionConverter::toBoolean(value, true));
	}

	static const LogString THRESHOLD_PREFIX(LOG4CXXNG_STR("log4j.threshold"));
	LogString thresholdStr =
		OptionConverter::findAndSubst(THRESHOLD_PREFIX, properties);

	if (!thresholdStr.empty())
	{
		hierarchy->setThreshold(OptionConverter::toLevel(thresholdStr, Level::getAll()));
		LogLog::debug(((LogString) LOG4CXXNG_STR("Hierarchy threshold set to ["))
			+ hierarchy->getThreshold()->toString()
			+ LOG4CXXNG_STR("]."));
	}

	static const LogString STRINGSTREAM_KEY(LOG4CXXNG_STR("log4j.stringstream"));
	LogString strstrValue(properties.getProperty(STRINGSTREAM_KEY));

	if (strstrValue == LOG4CXXNG_STR("static"))
	{
		MessageBufferUseStaticStream();
	}

	configureRootLogger(properties, hierarchy);
	configureLoggerFactory(properties);
	parseCatsAndRenderers(properties, hierarchy);

	LogLog::debug(LOG4CXXNG_STR("Finished configuring."));

	// We don't want to hold references to appenders preventing their
	// destruction.
	registry->clear();
}

void PropertyConfigurator::configureLoggerFactory(helpers::Properties& props)
{
	static const LogString LOGGER_FACTORY_KEY(LOG4CXXNG_STR("log4j.loggerFactory"));

	LogString factoryClassName =
		OptionConverter::findAndSubst(LOGGER_FACTORY_KEY, props);

	if (!factoryClassName.empty())
	{
		LogString msg(LOG4CXXNG_STR("Setting logger factory to ["));
		msg += factoryClassName;
		msg += LOG4CXXNG_STR("].");
		LogLog::debug(msg);
		loggerFactory =
			OptionConverter::instantiateByClassName(
				factoryClassName, LoggerFactory::getStaticClass(), loggerFactory);
		static const LogString FACTORY_PREFIX(LOG4CXXNG_STR("log4j.factory."));
		Pool p;
		PropertySetter::setProperties(loggerFactory, props, FACTORY_PREFIX, p);
	}
}

void PropertyConfigurator::configureRootLogger(helpers::Properties& props,
	spi::LoggerRepositoryPtr& hierarchy)
{
	static const LogString ROOT_CATEGORY_PREFIX(LOG4CXXNG_STR("log4j.rootCategory"));
	static const LogString ROOT_LOGGER_PREFIX(LOG4CXXNG_STR("log4j.rootLogger"));



	LogString effectiveFrefix(ROOT_LOGGER_PREFIX);
	LogString value = OptionConverter::findAndSubst(ROOT_LOGGER_PREFIX, props);

	if (value.empty())
	{
		value = OptionConverter::findAndSubst(ROOT_CATEGORY_PREFIX, props);
		effectiveFrefix = ROOT_CATEGORY_PREFIX;
	}

	if (value.empty())
	{
		LogLog::debug(LOG4CXXNG_STR("Could not find root logger information. Is this OK?"));
	}
	else
	{
		LoggerPtr root = hierarchy->getRootLogger();

		LOCK_W sync(root->getMutex());
		static const LogString INTERNAL_ROOT_NAME(LOG4CXXNG_STR("root"));
		parseLogger(props, root, effectiveFrefix, INTERNAL_ROOT_NAME, value);
	}
}

void PropertyConfigurator::parseCatsAndRenderers(helpers::Properties& props,
	spi::LoggerRepositoryPtr& hierarchy)
{
	static const LogString CATEGORY_PREFIX(LOG4CXXNG_STR("log4j.category."));
	static const LogString LOGGER_PREFIX(LOG4CXXNG_STR("log4j.logger."));

	std::vector<LogString> names = props.propertyNames();

	std::vector<LogString>::iterator it = names.begin();
	std::vector<LogString>::iterator itEnd = names.end();

	while (it != itEnd)
	{
		LogString key = *it++;

		if (key.find(CATEGORY_PREFIX) == 0 || key.find(LOGGER_PREFIX) == 0)
		{
			LogString loggerName;

			if (key.find(CATEGORY_PREFIX) == 0)
			{
				loggerName = key.substr(CATEGORY_PREFIX.length());
			}
			else if (key.find(LOGGER_PREFIX) == 0)
			{
				loggerName = key.substr(LOGGER_PREFIX.length());
			}

			LogString value = OptionConverter::findAndSubst(key, props);
			LoggerPtr logger = hierarchy->getLogger(loggerName, loggerFactory);

			LOCK_W sync(logger->getMutex());
			parseLogger(props, logger, key, loggerName, value);
			parseAdditivityForLogger(props, logger, loggerName);
		}
	}
}

void PropertyConfigurator::parseAdditivityForLogger(helpers::Properties& props,
	LoggerPtr& cat, const LogString& loggerName)
{

	static const LogString ADDITIVITY_PREFIX(LOG4CXXNG_STR("log4j.additivity."));



	LogString value(OptionConverter::findAndSubst(ADDITIVITY_PREFIX + loggerName, props));
	LogLog::debug((LogString) LOG4CXXNG_STR("Handling ") + ADDITIVITY_PREFIX
		+ loggerName + LOG4CXXNG_STR("=[") +  value + LOG4CXXNG_STR("]"));

	// touch additivity only if necessary
	if (!value.empty())
	{
		bool additivity = OptionConverter::toBoolean(value, true);
		LogLog::debug(((LogString) LOG4CXXNG_STR("Setting additivity for \""))
			+ loggerName
			+ ((additivity) ?  LOG4CXXNG_STR("\" to true") :
				LOG4CXXNG_STR("\" to false")));
		cat->setAdditivity(additivity);
	}
}

/**
        This method must work for the root logger as well.
*/
void PropertyConfigurator::parseLogger(
	helpers::Properties& props, LoggerPtr& logger, const LogString& /* optionKey */,
	const LogString& loggerName, const LogString& value)
{
	LogLog::debug(((LogString) LOG4CXXNG_STR("Parsing for ["))
		+ loggerName
		+ LOG4CXXNG_STR("] with value=[")
		+ value + LOG4CXXNG_STR("]."));

	// We must skip over ',' but not white space
	StringTokenizer st(value, LOG4CXXNG_STR(","));

	// If value is not in the form ", appender.." or "", then we should set
	// the level of the logger.
	if (!(value.find(LOG4CXXNG_STR(",")) == 0 || value.empty()))
	{
		// just to be on the safe side...
		if (!st.hasMoreTokens())
		{
			return;
		}

		LogString levelStr = st.nextToken();
		LogLog::debug((LogString) LOG4CXXNG_STR("Level token is [")
			+ levelStr +  LOG4CXXNG_STR("]."));


		// If the level value is inherited, set logger level value to
		// null. We also check that the user has not specified inherited for the
		// root logger.
		if (StringHelper::equalsIgnoreCase(levelStr, LOG4CXXNG_STR("INHERITED"), LOG4CXXNG_STR("inherited"))
			|| StringHelper::equalsIgnoreCase(levelStr, LOG4CXXNG_STR("NULL"), LOG4CXXNG_STR("null")))
		{
			static const LogString INTERNAL_ROOT_NAME(LOG4CXXNG_STR("root"));

			if (loggerName == INTERNAL_ROOT_NAME)
			{
				LogLog::warn(LOG4CXXNG_STR("The root logger cannot be set to null."));
			}
			else
			{
				logger->setLevel(0);
				LogLog::debug((LogString) LOG4CXXNG_STR("Logger ")
					+ loggerName + LOG4CXXNG_STR(" set to null"));
			}
		}
		else
		{
			logger->setLevel(OptionConverter::toLevel(levelStr, Level::getDebug()));

			LogLog::debug((LogString) LOG4CXXNG_STR("Logger ")
				+ loggerName + LOG4CXXNG_STR(" set to ")
				+ logger->getLevel()->toString());
		}

	}

	// Begin by removing all existing appenders.
	logger->removeAllAppenders();

	AppenderPtr appender;
	LogString appenderName;

	while (st.hasMoreTokens())
	{
		appenderName = StringHelper::trim(st.nextToken());

		if (appenderName.empty() || appenderName == LOG4CXXNG_STR(","))
		{
			continue;
		}

		LogLog::debug(LOG4CXXNG_STR("Parsing appender named ")
			+ appenderName + LOG4CXXNG_STR("\"."));
		appender = parseAppender(props, appenderName);

		if (appender != 0)
		{
			logger->addAppender(appender);
		}
	}
}

AppenderPtr PropertyConfigurator::parseAppender(
	helpers::Properties& props, const LogString& appenderName)
{
	AppenderPtr appender = registryGet(appenderName);

	if (appender != 0)
	{
		LogLog::debug((LogString) LOG4CXXNG_STR("Appender \"")
			+ appenderName + LOG4CXXNG_STR("\" was already parsed."));

		return appender;
	}

	static const LogString APPENDER_PREFIX(LOG4CXXNG_STR("log4j.appender."));

	// Appender was not previously initialized.
	LogString prefix = APPENDER_PREFIX + appenderName;
	LogString layoutPrefix = prefix + LOG4CXXNG_STR(".layout");

	appender =
		OptionConverter::instantiateByKey(
			props, prefix, Appender::getStaticClass(), 0);

	if (appender == 0)
	{
		LogLog::error((LogString) LOG4CXXNG_STR("Could not instantiate appender named \"")
			+ appenderName + LOG4CXXNG_STR("\"."));
		return 0;
	}

	appender->setName(appenderName);

	if (appender->instanceof(OptionHandler::getStaticClass()))
	{
		Pool p;

		if (appender->requiresLayout())
		{
			LayoutPtr layout =
				OptionConverter::instantiateByKey(
					props, layoutPrefix, Layout::getStaticClass(), 0);

			if (layout != 0)
			{
				appender->setLayout(layout);
				LogLog::debug((LogString) LOG4CXXNG_STR("Parsing layout options for \"")
					+ appenderName + LOG4CXXNG_STR("\"."));

				//configureOptionHandler(layout, layoutPrefix + ".", props);
				PropertySetter::setProperties(layout, props, layoutPrefix + LOG4CXXNG_STR("."), p);
				LogLog::debug((LogString) LOG4CXXNG_STR("End of parsing for \"")
					+ appenderName +  LOG4CXXNG_STR("\"."));
			}
		}

		//configureOptionHandler((OptionHandler) appender, prefix + _T("."), props);
		PropertySetter::setProperties(appender, props, prefix + LOG4CXXNG_STR("."), p);
		LogLog::debug((LogString) LOG4CXXNG_STR("Parsed \"")
			+ appenderName + LOG4CXXNG_STR("\" options."));
	}

	registryPut(appender);

	return appender;
}

void PropertyConfigurator::registryPut(const AppenderPtr& appender)
{
	(*registry)[appender->getName()] = appender;
}

AppenderPtr PropertyConfigurator::registryGet(const LogString& name)
{
	return (*registry)[name];
}
