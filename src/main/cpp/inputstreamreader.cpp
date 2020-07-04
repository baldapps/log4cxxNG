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
#include <log4cxxNG/helpers/inputstreamreader.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/helpers/pool.h>
#include <log4cxxNG/helpers/bytebuffer.h>

#include <cstdio>
#include <cstring>

using namespace log4cxxng;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(InputStreamReader)

InputStreamReader::InputStreamReader(const InputStreamPtr& in1)
	: in(in1), dec(CharsetDecoder::getDefaultDecoder())
{
	if (in1 == 0)
	{
		throw NullPointerException(LOG4CXXNG_STR("in parameter may not be null."));
	}
}

InputStreamReader::InputStreamReader(const InputStreamPtr& in1, const CharsetDecoderPtr& dec1)
	: in(in1), dec(dec1)
{
	if (in1 == 0)
	{
		throw NullPointerException(LOG4CXXNG_STR("in parameter may not be null."));
	}

	if (dec1 == 0)
	{
		throw NullPointerException(LOG4CXXNG_STR("dec parameter may not be null."));
	}
}

InputStreamReader::~InputStreamReader()
{
}

void InputStreamReader::close(Pool& )
{
	in->close();
}

LogString InputStreamReader::read(Pool& p)
{
	const size_t BUFSIZE = 4096;
	ByteBuffer buf(p.pstralloc(BUFSIZE), BUFSIZE);
	LogString output;

	// read whole file
	while (in->read(buf) >= 0)
	{
		buf.flip();
		log4cxxng_status_t stat = dec->decode(buf, output);

		if (stat != 0)
		{
			throw IOException(stat);
		}

		if (buf.remaining() > 0)
		{
			memmove(buf.data(), buf.current(), buf.remaining());
			buf.limit(buf.remaining());
		}
		else
		{
			buf.clear();
		}
	}

	return output;
}
