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

#ifndef _LOG4CXXNG_LAYOUT_H
#define _LOG4CXXNG_LAYOUT_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif


#include <log4cxxNG/helpers/objectimpl.h>
#include <log4cxxNG/helpers/objectptr.h>
#include <log4cxxNG/spi/optionhandler.h>
#include <log4cxxNG/spi/loggingevent.h>


namespace log4cxxng
{
/**
Extend this abstract class to create your own log layout format.
*/
class LOG4CXXNG_EXPORT Layout :
	public virtual spi::OptionHandler,
	public virtual helpers::ObjectImpl
{
	public:
		DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(Layout)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(Layout)
		LOG4CXXNG_CAST_ENTRY(spi::OptionHandler)
		END_LOG4CXXNG_CAST_MAP()

		virtual ~Layout();
		void addRef() const;
		void releaseRef() const;


		/**
		Implement this method to create your own layout format.
		*/
		virtual void format(LogString& output,
			const spi::LoggingEventPtr& event, log4cxxng::helpers::Pool& pool) const = 0;

		/**
		Returns the content type output by this layout. The base class
		returns "text/plain".
		*/
		virtual LogString getContentType() const;

		/**
		Append the header for the layout format. The base class does
		nothing.
		*/
		virtual void appendHeader(LogString& output, log4cxxng::helpers::Pool& p);

		/**
		Append the footer for the layout format. The base class does
		nothing.
		*/
		virtual void appendFooter(LogString& output, log4cxxng::helpers::Pool& p);

		/**
		If the layout handles the throwable object contained within
		{@link spi::LoggingEvent LoggingEvent}, then the layout should return
		<code>false</code>. Otherwise, if the layout ignores throwable
		object, then the layout should return <code>true</code>.

		<p>The SimpleLayout, TTCCLayout,
		PatternLayout all return <code>true</code>. The {@link
		xml::XMLLayout XMLLayout} returns <code>false</code>.
		*/
		virtual bool ignoresThrowable() const = 0;
};
LOG4CXXNG_PTR_DEF(Layout);
}

#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif

#endif // _LOG4CXXNG_LAYOUT_H
