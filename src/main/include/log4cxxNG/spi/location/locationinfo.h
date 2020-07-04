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

#ifndef _LOG4CXXNG_SPI_LOCATION_LOCATIONINFO_H
#define _LOG4CXXNG_SPI_LOCATION_LOCATIONINFO_H

#include <log4cxxNG/log4cxxNG.h>
#include <string>
#include <log4cxxNG/helpers/objectoutputstream.h>

namespace log4cxxng
{
namespace spi
{
/**
 * This class represents the location of a logging statement.
 *
 */
class LOG4CXXNG_EXPORT LocationInfo
{
	public:



		/**
		  *   When location information is not available the constant
		  * <code>NA</code> is returned. Current value of this string constant is <b>?</b>.
		  */
		static const char* const NA;
		static const char* const NA_METHOD;

		static const LocationInfo& getLocationUnavailable();



		/**
		 *   Constructor.
		 *   @remarks Used by LOG4CXXNG_LOCATION to generate
		 *       location info for current code site
		 */
		LocationInfo( const char* const fileName,
			const char* const functionName,
			int lineNumber);

		/**
		 *   Default constructor.
		 */
		LocationInfo();

		/**
		 *   Copy constructor.
		 *   @param src source location
		 */
		LocationInfo( const LocationInfo& src );

		/**
		 *  Assignment operator.
		 * @param src source location
		 */
		LocationInfo& operator = ( const LocationInfo& src );

		/**
		 *   Resets location info to default state.
		 */
		void clear();


		/** Return the class name of the call site. */
		const std::string getClassName() const;

		/**
		 *   Return the file name of the caller.
		 *   @returns file name, may be null.
		 */
		const char* getFileName() const;

		/**
		  *   Returns the line number of the caller.
		  * @returns line number, -1 if not available.
		  */
		int getLineNumber() const;

		/** Returns the method name of the caller. */
		const std::string getMethodName() const;

		void write(log4cxxng::helpers::ObjectOutputStream& os, log4cxxng::helpers::Pool& p) const;


	private:
		/** Caller's line number. */
		int lineNumber;

		/** Caller's file name. */
		const char* fileName;

		/** Caller's method name. */
		const char* methodName;


};
}
}

#if !defined(LOG4CXXNG_LOCATION)
#if defined(_MSC_VER)
	#if _MSC_VER >= 1300
		#define __LOG4CXXNG_FUNC__ __FUNCSIG__
	#endif
#else
	#if defined(__GNUC__)
		#define __LOG4CXXNG_FUNC__ __PRETTY_FUNCTION__
	#else
		#if defined(__BORLANDC__)
			#define __LOG4CXXNG_FUNC__ __FUNC__
		#endif
	#endif
#endif
#if !defined(__LOG4CXXNG_FUNC__)
	#define __LOG4CXXNG_FUNC__ ""
#endif
#define LOG4CXXNG_LOCATION ::log4cxxng::spi::LocationInfo(__FILE__,         \
	__LOG4CXXNG_FUNC__, \
	__LINE__)
#endif

#endif //_LOG4CXXNG_SPI_LOCATION_LOCATIONINFO_H
