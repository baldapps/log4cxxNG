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

#ifndef _LOG4CXXNG_HELPERS_BYTEBUFFER_H
#define _LOG4CXXNG_HELPERS_BYTEBUFFER_H

#include <log4cxxNG/log4cxxNG.h>
#include <stdio.h>

namespace log4cxxng
{

namespace helpers
{

/**
* A byte buffer.
*/
class LOG4CXXNG_EXPORT ByteBuffer
{
	private:
		char* base;
		size_t pos;
		size_t lim;
		size_t cap;

	public:
		ByteBuffer(char* data, size_t capacity);
		~ByteBuffer();

		void clear();
		void flip();

		inline char* data()
		{
			return base;
		}
		inline const char* data() const
		{
			return base;
		}
		inline char* current()
		{
			return base + pos;
		}
		inline const char* current() const
		{
			return base + pos;
		}
		inline size_t limit() const
		{
			return lim;
		}
		void limit(size_t newLimit);
		inline size_t position() const
		{
			return pos;
		}
		inline size_t remaining() const
		{
			return lim - pos;
		}
		void position(size_t newPosition);

		bool put(char byte);


	private:
		ByteBuffer(const ByteBuffer&);
		ByteBuffer& operator=(const ByteBuffer&);
};

} // namespace helpers

}  //namespace log4cxxng

#endif //_LOG4CXXNG_HELPERS_BYTEBUFFER_H
