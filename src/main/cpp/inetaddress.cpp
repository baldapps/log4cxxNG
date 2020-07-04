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
#if defined(_MSC_VER)
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/helpers/inetaddress.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/transcoder.h>
#include <log4cxxNG/helpers/pool.h>

#include "apr_network_io.h"

using namespace log4cxxng;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(InetAddress)

UnknownHostException::UnknownHostException(const LogString& msg1)
	: Exception(msg1)
{
}

UnknownHostException::UnknownHostException(const UnknownHostException& src)
	: Exception(src)
{
}

UnknownHostException& UnknownHostException::operator=(const UnknownHostException& src)
{
	Exception::operator=(src);
	return *this;
}


InetAddress::InetAddress(const LogString& hostName, const LogString& hostAddr)
	: ipAddrString(hostAddr), hostNameString(hostName)
{
}


/** Determines all the IP addresses of a host, given the host's name.
*/
std::vector<InetAddressPtr> InetAddress::getAllByName(const LogString& host)
{
	LOG4CXXNG_ENCODE_CHAR(encodedHost, host);

	// retrieve information about the given host
	Pool addrPool;

	apr_sockaddr_t* address = 0;
	apr_status_t status =
		apr_sockaddr_info_get(&address, encodedHost.c_str(),
			APR_INET, 0, 0, addrPool.getAPRPool());

	if (status != APR_SUCCESS)
	{
		LogString msg(LOG4CXXNG_STR("Cannot get information about host: "));
		msg.append(host);
		LogLog::error(msg);
		throw UnknownHostException(msg);
	}

	std::vector<InetAddressPtr> result;
	apr_sockaddr_t* currentAddr = address;

	while (currentAddr != NULL)
	{
		// retrieve the IP address of this InetAddress.
		LogString ipAddrString;
		char* ipAddr;
		status = apr_sockaddr_ip_get(&ipAddr, currentAddr);

		if (status == APR_SUCCESS)
		{
			std::string ip(ipAddr);
			Transcoder::decode(ip, ipAddrString);
		}

		// retrieve the host name of this InetAddress.
		LogString hostNameString;
		char* hostName;
		status = apr_getnameinfo(&hostName, currentAddr, 0);

		if (status == APR_SUCCESS)
		{
			std::string host(hostName);
			Transcoder::decode(host, hostNameString);
		}

		result.push_back(new InetAddress(hostNameString, ipAddrString));
		currentAddr = currentAddr->next;
	}

	return result;
}


/** Determines the IP address of a host, given the host's name.
*/
InetAddressPtr InetAddress::getByName(const LogString& host)
{
	return getAllByName(host)[0];
}

/** Returns the IP address string "%d.%d.%d.%d".
*/
LogString InetAddress::getHostAddress() const
{
	return ipAddrString;
}

/** Gets the host name for this IP address.
*/
LogString InetAddress::getHostName() const
{
	return hostNameString;
}

/** Returns the local host.
*/
InetAddressPtr InetAddress::getLocalHost()
{
	return getByName(LOG4CXXNG_STR("127.0.0.1"));
}


InetAddressPtr InetAddress::anyAddress()
{
	// APR_ANYADDR does not work with the LOG4CXXNG_STR macro
	return getByName(LOG4CXXNG_STR("0.0.0.0"));
}


/** Converts this IP address to a String.
*/
LogString InetAddress::toString() const
{
	LogString rv(getHostName());
	rv.append(LOG4CXXNG_STR("/"));
	rv.append(getHostAddress());
	return rv;
}

