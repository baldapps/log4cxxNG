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

#ifndef _LOG4CXXNG_HELPERS_SYSTEMERRWRITER_H
#define _LOG4CXXNG_HELPERS_SYSTEMERRWRITER_H

#include <log4cxxNG/helpers/writer.h>

namespace log4cxxng
{
namespace helpers
{

/**
*   Abstract class for writing to character streams.
*/
class LOG4CXXNG_EXPORT SystemErrWriter : public Writer
{
	public:
		DECLARE_LOG4CXXNG_OBJECT(SystemErrWriter)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(SystemErrWriter)
		LOG4CXXNG_CAST_ENTRY_CHAIN(Writer)
		END_LOG4CXXNG_CAST_MAP()

		SystemErrWriter();
		virtual ~SystemErrWriter();

		virtual void close(Pool& p);
		virtual void flush(Pool& p);
		virtual void write(const LogString& str, Pool& p);

		static void write(const LogString& str);
		static void flush();

	private:
		SystemErrWriter(const SystemErrWriter&);
		SystemErrWriter& operator=(const SystemErrWriter&);
		static bool isWide();
};

} // namespace helpers

}  //namespace log4cxxng

#endif //_LOG4CXXNG_HELPERS_SYSTEMERRWRITER_H
