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

#include <log4cxxNG/net/syslogappender.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/datagramsocket.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/level.h>
#include <log4cxxNG/helpers/transcoder.h>
#if !defined(LOG4CXXNG)
	#define LOG4CXXNG 1
#endif
#include <log4cxxNG/private/log4cxxNG_private.h>

#if LOG4CXXNG_HAVE_SYSLOG
	#include <syslog.h>
#else
	/* facility codes */
	#define   LOG_KERN (0<<3)   /* kernel messages */
	#define   LOG_USER (1<<3)   /* random user-level messages */
	#define   LOG_MAIL (2<<3)   /* mail system */
	#define   LOG_DAEMON  (3<<3)   /* system daemons */
	#define   LOG_AUTH (4<<3)   /* security/authorization messages */
	#define   LOG_SYSLOG  (5<<3)   /* messages generated internally by syslogd */
	#define   LOG_LPR     (6<<3)   /* line printer subsystem */
	#define   LOG_NEWS (7<<3)   /* network news subsystem */
	#define   LOG_UUCP (8<<3)   /* UUCP subsystem */
	#define   LOG_CRON (9<<3)   /* clock daemon */
	#define   LOG_AUTHPRIV   (10<<3)  /* security/authorization messages (private) */
	#define   LOG_FTP     (11<<3)  /* ftp daemon */

	/* other codes through 15 reserved for system use */
	#define   LOG_LOCAL0  (16<<3)  /* reserved for local use */
	#define   LOG_LOCAL1  (17<<3)  /* reserved for local use */
	#define   LOG_LOCAL2  (18<<3)  /* reserved for local use */
	#define   LOG_LOCAL3  (19<<3)  /* reserved for local use */
	#define   LOG_LOCAL4  (20<<3)  /* reserved for local use */
	#define   LOG_LOCAL5  (21<<3)  /* reserved for local use */
	#define   LOG_LOCAL6  (22<<3)  /* reserved for local use */
	#define   LOG_LOCAL7  (23<<3)  /* reserved for local use */
#endif

#define LOG_UNDEF -1

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::net;

IMPLEMENT_LOG4CXXNG_OBJECT(SyslogAppender)

SyslogAppender::SyslogAppender()
	: syslogFacility(LOG_USER), facilityPrinting(false), sw(0)
{
	this->initSyslogFacilityStr();

}

SyslogAppender::SyslogAppender(const LayoutPtr& layout1,
	int syslogFacility1)
	: syslogFacility(syslogFacility1), facilityPrinting(false), sw(0)
{
	this->layout = layout1;
	this->initSyslogFacilityStr();
}

SyslogAppender::SyslogAppender(const LayoutPtr& layout1,
	const LogString& syslogHost1, int syslogFacility1)
	: syslogFacility(syslogFacility1), facilityPrinting(false), sw(0)
{
	this->layout = layout1;
	this->initSyslogFacilityStr();
	setSyslogHost(syslogHost1);
}

SyslogAppender::~SyslogAppender()
{
	finalize();
}

/** Release any resources held by this SyslogAppender.*/
void SyslogAppender::close()
{
	closed = true;

	if (sw != 0)
	{
		delete sw;
		sw = 0;
	}
}

void SyslogAppender::initSyslogFacilityStr()
{
	facilityStr = getFacilityString(this->syslogFacility);

	if (facilityStr.empty())
	{
		Pool p;
		LogString msg(LOG4CXXNG_STR("\""));
		StringHelper::toString(syslogFacility, p, msg);
		msg.append(LOG4CXXNG_STR("\" is an unknown syslog facility. Defaulting to \"USER\"."));
		LogLog::error(msg);
		this->syslogFacility = LOG_USER;
		facilityStr = LOG4CXXNG_STR("user:");
	}
	else
	{
		facilityStr += LOG4CXXNG_STR(":");
	}
}

/**
Returns the specified syslog facility as a lower-case String,
e.g. "kern", "user", etc.
*/
LogString SyslogAppender::getFacilityString(
	int syslogFacility)
{
	switch (syslogFacility)
	{
		case LOG_KERN:
			return LOG4CXXNG_STR("kern");

		case LOG_USER:
			return LOG4CXXNG_STR("user");

		case LOG_MAIL:
			return LOG4CXXNG_STR("mail");

		case LOG_DAEMON:
			return LOG4CXXNG_STR("daemon");

		case LOG_AUTH:
			return LOG4CXXNG_STR("auth");

		case LOG_SYSLOG:
			return LOG4CXXNG_STR("syslog");

		case LOG_LPR:
			return LOG4CXXNG_STR("lpr");

		case LOG_NEWS:
			return LOG4CXXNG_STR("news");

		case LOG_UUCP:
			return LOG4CXXNG_STR("uucp");

		case LOG_CRON:
			return LOG4CXXNG_STR("cron");
#ifdef LOG_AUTHPRIV

		case LOG_AUTHPRIV:
			return LOG4CXXNG_STR("authpriv");
#endif
#ifdef LOG_FTP

		case LOG_FTP:
			return LOG4CXXNG_STR("ftp");
#endif

		case LOG_LOCAL0:
			return LOG4CXXNG_STR("local0");

		case LOG_LOCAL1:
			return LOG4CXXNG_STR("local1");

		case LOG_LOCAL2:
			return LOG4CXXNG_STR("local2");

		case LOG_LOCAL3:
			return LOG4CXXNG_STR("local3");

		case LOG_LOCAL4:
			return LOG4CXXNG_STR("local4");

		case LOG_LOCAL5:
			return LOG4CXXNG_STR("local5");

		case LOG_LOCAL6:
			return LOG4CXXNG_STR("local6");

		case LOG_LOCAL7:
			return LOG4CXXNG_STR("local7");

		default:
			return LogString();
	}
}

int SyslogAppender::getFacility(
	const LogString& s)
{
	if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("KERN"), LOG4CXXNG_STR("kern")))
	{
		return LOG_KERN;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("USER"), LOG4CXXNG_STR("user")))
	{
		return LOG_USER;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("MAIL"), LOG4CXXNG_STR("mail")))
	{
		return LOG_MAIL;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("DAEMON"), LOG4CXXNG_STR("daemon")))
	{
		return LOG_DAEMON;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("AUTH"), LOG4CXXNG_STR("auth")))
	{
		return LOG_AUTH;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("SYSLOG"), LOG4CXXNG_STR("syslog")))
	{
		return LOG_SYSLOG;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LPR"), LOG4CXXNG_STR("lpr")))
	{
		return LOG_LPR;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("NEWS"), LOG4CXXNG_STR("news")))
	{
		return LOG_NEWS;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("UUCP"), LOG4CXXNG_STR("uucp")))
	{
		return LOG_UUCP;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("CRON"), LOG4CXXNG_STR("cron")))
	{
		return LOG_CRON;
	}

#ifdef LOG_AUTHPRIV
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("AUTHPRIV"), LOG4CXXNG_STR("authpriv")))
	{
		return LOG_AUTHPRIV;
	}

#endif
#ifdef LOG_FTP
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("FTP"), LOG4CXXNG_STR("ftp")))
	{
		return LOG_FTP;
	}

#endif
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL0"), LOG4CXXNG_STR("local0")))
	{
		return LOG_LOCAL0;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL1"), LOG4CXXNG_STR("local1")))
	{
		return LOG_LOCAL1;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL2"), LOG4CXXNG_STR("local2")))
	{
		return LOG_LOCAL2;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL3"), LOG4CXXNG_STR("local3")))
	{
		return LOG_LOCAL3;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL4"), LOG4CXXNG_STR("local4")))
	{
		return LOG_LOCAL4;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL5"), LOG4CXXNG_STR("local5")))
	{
		return LOG_LOCAL5;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL6"), LOG4CXXNG_STR("local6")))
	{
		return LOG_LOCAL6;
	}
	else if (StringHelper::equalsIgnoreCase(s, LOG4CXXNG_STR("LOCAL7"), LOG4CXXNG_STR("local7")))
	{
		return LOG_LOCAL7;
	}
	else
	{
		return LOG_UNDEF;
	}
}

void SyslogAppender::append(const spi::LoggingEventPtr& event, Pool& p)
{
	if  (!isAsSevereAsThreshold(event->getLevel()))
	{
		return;
	}

	LogString msg;
	layout->format(msg, event, p);

	// On the local host, we can directly use the system function 'syslog'
	// if it is available
#if LOG4CXXNG_HAVE_SYSLOG

	if (sw == 0)
	{
		std::string sbuf;
		Transcoder::encode(msg, sbuf);

		// use of "%s" to avoid a security hole
		::syslog(syslogFacility | event->getLevel()->getSyslogEquivalent(),
			"%s", sbuf.c_str());

		return;
	}

#endif

	// We must not attempt to append if sw is null.
	if (sw == 0)
	{
		errorHandler->error(LOG4CXXNG_STR("No syslog host is set for SyslogAppedender named \"") +
			this->name + LOG4CXXNG_STR("\"."));
		return;
	}

	LogString sbuf(1, 0x3C /* '<' */);
	StringHelper::toString((syslogFacility | event->getLevel()->getSyslogEquivalent()), p, sbuf);
	sbuf.append(1, (logchar) 0x3E /* '>' */);

	if (facilityPrinting)
	{
		sbuf.append(facilityStr);
	}

	sbuf.append(msg);
	sw->write(sbuf);
}

void SyslogAppender::activateOptions(Pool&)
{
}

void SyslogAppender::setOption(const LogString& option, const LogString& value)
{
	if (StringHelper::equalsIgnoreCase(option, LOG4CXXNG_STR("SYSLOGHOST"), LOG4CXXNG_STR("sysloghost")))
	{
		setSyslogHost(value);
	}
	else if (StringHelper::equalsIgnoreCase(option, LOG4CXXNG_STR("FACILITY"), LOG4CXXNG_STR("facility")))
	{
		setFacility(value);
	}
	else
	{
		AppenderSkeleton::setOption(option, value);
	}
}

void SyslogAppender::setSyslogHost(const LogString& syslogHost1)
{
	if (this->sw != 0)
	{
		delete this->sw;
		this->sw = 0;
	}

	LogString slHost = syslogHost1;
	int slHostPort = -1;

	LogString::size_type colonPos = 0;
	colonPos = slHost.rfind(':');

	if (colonPos != LogString::npos)
	{
		slHostPort = StringHelper::toInt(slHost.substr(colonPos + 1));
		// Erase the :port part of the host name
		slHost.erase( colonPos );
	}

	// On the local host, we can directly use the system function 'syslog'
	// if it is available (cf. append)
#if LOG4CXXNG_HAVE_SYSLOG

	if (syslogHost1 != LOG4CXXNG_STR("localhost") && syslogHost1 != LOG4CXXNG_STR("127.0.0.1")
		&& !syslogHost1.empty())
#endif
		if (slHostPort >= 0)
		{
			this->sw = new SyslogWriter(slHost, slHostPort);
		}
		else
		{
			this->sw = new SyslogWriter(slHost);
		}

	this->syslogHost = slHost;
	this->syslogHostPort = slHostPort;
}


void SyslogAppender::setFacility(const LogString& facilityName)
{
	if (facilityName.empty())
	{
		return;
	}

	syslogFacility = getFacility(facilityName);

	if (syslogFacility == LOG_UNDEF)
	{
		LogLog::error(LOG4CXXNG_STR("[") + facilityName +
			LOG4CXXNG_STR("] is an unknown syslog facility. Defaulting to [USER]."));
		syslogFacility = LOG_USER;
	}

	this->initSyslogFacilityStr();
}

