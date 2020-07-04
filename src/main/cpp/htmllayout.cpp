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
#include <log4cxxNG/htmllayout.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/helpers/optionconverter.h>
#include <log4cxxNG/level.h>
#include <log4cxxNG/helpers/transform.h>
#include <log4cxxNG/helpers/iso8601dateformat.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/transcoder.h>

#include <apr_time.h>
#include <apr_strings.h>
#include <string.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;
using namespace log4cxxng::spi;

IMPLEMENT_LOG4CXXNG_OBJECT(HTMLLayout)


HTMLLayout::HTMLLayout()
	: locationInfo(false), title(LOG4CXXNG_STR("Log4cxx Log Messages")),
	  dateFormat()
{
	dateFormat.setTimeZone(TimeZone::getGMT());
}


void HTMLLayout::setOption(const LogString& option,
	const LogString& value)
{

	if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("TITLE"), LOG4CXXNG_STR("title")))
	{
		setTitle(value);
	}
	else if (StringHelper::equalsIgnoreCase(option,
			LOG4CXXNG_STR("LOCATIONINFO"), LOG4CXXNG_STR("locationinfo")))
	{
		setLocationInfo(OptionConverter::toBoolean(value, false));
	}
}

void HTMLLayout::format(LogString& output,
	const spi::LoggingEventPtr& event,
	Pool& p) const
{
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<tr>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<td>"));

	dateFormat.format(output, event->getTimeStamp(), p);


	output.append(LOG4CXXNG_STR("</td>"));
	output.append(LOG4CXXNG_EOL);

	output.append(LOG4CXXNG_STR("<td title=\""));
	LogString threadName(event->getThreadName());
	output.append(threadName);
	output.append(LOG4CXXNG_STR(" thread\">"));
	output.append(threadName);
	output.append(LOG4CXXNG_STR("</td>"));
	output.append(LOG4CXXNG_EOL);

	output.append(LOG4CXXNG_STR("<td title=\"Level\">"));

	if (event->getLevel()->equals(Level::getDebug()))
	{
		output.append(LOG4CXXNG_STR("<font color=\"#339933\">"));
		output.append(event->getLevel()->toString());
		output.append(LOG4CXXNG_STR("</font>"));
	}
	else if (event->getLevel()->isGreaterOrEqual(Level::getWarn()))
	{
		output.append(LOG4CXXNG_STR("<font color=\"#993300\"><strong>"));
		output.append(event->getLevel()->toString());
		output.append(LOG4CXXNG_STR("</strong></font>"));
	}
	else
	{
		output.append(event->getLevel()->toString());
	}

	output.append(LOG4CXXNG_STR("</td>"));
	output.append(LOG4CXXNG_EOL);

	output.append(LOG4CXXNG_STR("<td title=\""));
	output.append(event->getLoggerName());
	output.append(LOG4CXXNG_STR(" logger\">"));
	Transform::appendEscapingTags(output, event->getLoggerName());
	output.append(LOG4CXXNG_STR("</td>"));
	output.append(LOG4CXXNG_EOL);

	if (locationInfo)
	{
		output.append(LOG4CXXNG_STR("<td>"));
		const LocationInfo& locInfo = event->getLocationInformation();
		LOG4CXXNG_DECODE_CHAR(fileName, locInfo.getFileName());
		Transform::appendEscapingTags(output, fileName);
		output.append(1, (logchar) 0x3A /* ':' */);
		int line = event->getLocationInformation().getLineNumber();

		if (line != 0)
		{
			StringHelper::toString(line, p, output);
		}

		output.append(LOG4CXXNG_STR("</td>"));
		output.append(LOG4CXXNG_EOL);
	}

	output.append(LOG4CXXNG_STR("<td title=\"Message\">"));
	Transform::appendEscapingTags(output, event->getRenderedMessage());
	output.append(LOG4CXXNG_STR("</td>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("</tr>"));
	output.append(LOG4CXXNG_EOL);

	LogString ndcVal;

	if (event->getNDC(ndcVal))
	{
		output.append(LOG4CXXNG_STR("<tr><td bgcolor=\"#EEEEEE\" "));
		output.append(LOG4CXXNG_STR("style=\"font-size : xx-small;\" colspan=\"6\" "));
		output.append(LOG4CXXNG_STR("title=\"Nested Diagnostic Context\">"));
		output.append(LOG4CXXNG_STR("NDC: "));
		Transform::appendEscapingTags(output, ndcVal);
		output.append(LOG4CXXNG_STR("</td></tr>"));
		output.append(LOG4CXXNG_EOL);
	}
}

void HTMLLayout::appendHeader(LogString& output, Pool& p)
{
	output.append(LOG4CXXNG_STR("<!DOCTYPE HTML PUBLIC "));
	output.append(LOG4CXXNG_STR("\"-//W3C//DTD HTML 4.01 Transitional//EN\" "));
	output.append(LOG4CXXNG_STR("\"http://www.w3.org/TR/html4/loose.dtd\">"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<html>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<head>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<title>"));
	output.append(title);
	output.append(LOG4CXXNG_STR("</title>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<style type=\"text/css\">"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<!--"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("body, table {font-family: arial,sans-serif; font-size: x-small;}"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("th {background: #336699; color: #FFFFFF; text-align: left;}"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("-->"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("</style>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("</head>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<body bgcolor=\"#FFFFFF\" topmargin=\"6\" leftmargin=\"6\">"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<hr size=\"1\" noshade>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("Log session start time "));

	dateFormat.format(output, apr_time_now(), p);

	output.append(LOG4CXXNG_STR("<br>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<br>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<table cellspacing=\"0\" cellpadding=\"4\" border=\"1\" bordercolor=\"#224466\" width=\"100%\">"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<tr>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<th>Time</th>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<th>Thread</th>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<th>Level</th>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<th>Logger</th>"));
	output.append(LOG4CXXNG_EOL);

	if (locationInfo)
	{
		output.append(LOG4CXXNG_STR("<th>File:Line</th>"));
		output.append(LOG4CXXNG_EOL);
	}

	output.append(LOG4CXXNG_STR("<th>Message</th>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("</tr>"));
	output.append(LOG4CXXNG_EOL);
}

void HTMLLayout::appendFooter(LogString& output, Pool& /* pool */ )
{
	output.append(LOG4CXXNG_STR("</table>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("<br>"));
	output.append(LOG4CXXNG_EOL);
	output.append(LOG4CXXNG_STR("</body></html>"));
}
