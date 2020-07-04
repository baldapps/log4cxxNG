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

#ifndef _LOG4CXXNG_NET_SOCKET_HUB_APPENDER_H
#define _LOG4CXXNG_NET_SOCKET_HUB_APPENDER_H

#if defined(_MSC_VER)
	#pragma warning ( push )
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif


#include <log4cxxNG/appenderskeleton.h>
#include <vector>
#include <log4cxxNG/helpers/thread.h>
#include <log4cxxNG/helpers/objectoutputstream.h>


namespace log4cxxng
{
namespace helpers
{
class ObjectOutputStream;
typedef ObjectPtrT<ObjectOutputStream> ObjectOutputStreamPtr;
}
namespace net
{
LOG4CXXNG_LIST_DEF(ObjectOutputStreamList, log4cxxng::helpers::ObjectOutputStreamPtr);

/**
Sends {@link log4cxxng::spi::LoggingEvent LoggingEvent} objects to a set of remote log
servers, usually a SocketNode.

<p>Acts just like SocketAppender except that instead of
connecting to a given remote log server,
<code>SocketHubAppender</code> accepts connections from the remote
log servers as clients.  It can accept more than one connection.
When a log event is received, the event is sent to the set of
currently connected remote log servers. Implemented this way it does
not require any update to the configuration file to send data to
another remote log server. The remote log server simply connects to
the host and port the <code>SocketHubAppender</code> is running on.

<p>The <code>SocketHubAppender</code> does not store events such
that the remote side will events that arrived after the
establishment of its connection. Once connected, events arrive in
order as guaranteed by the TCP protocol.

<p>This implementation borrows heavily from the SocketAppender.

<p>The SocketHubAppender has the following characteristics:

- If sent to a SocketNode, logging is non-intrusive as
far as the log event is concerned. In other words, the event will be
logged with the same time stamp, NDC,
location info as if it were logged locally.

- <code>SocketHubAppender</code> does not use a layout. It
ships a serialized spi::LoggingEvent object to the remote side.

- <code>SocketHubAppender</code> relies on the TCP
protocol. Consequently, if the remote side is reachable, then log
events will eventually arrive at remote client.

- If no remote clients are attached, the logging requests are
simply dropped.

- Logging events are automatically <em>buffered</em> by the
native TCP implementation. This means that if the link to remote
client is slow but still faster than the rate of (log) event
production, the application will not be affected by the slow network
connection. However, if the network connection is slower then the
rate of event production, then the local application can only
progress at the network rate. In particular, if the network link to
the the remote client is down, the application will be blocked.
@n @n On the other hand, if the network link is up, but the remote
client is down, the client will not be blocked when making log
requests but the log events will be lost due to client
unavailability.
@n @n The single remote client case extends to multiple clients
connections. The rate of logging will be determined by the slowest
link.

- If the application hosting the <code>SocketHubAppender</code>
exits before the <code>SocketHubAppender</code> is closed either
explicitly or subsequent to garbage collection, then there might
be untransmitted data in the pipe which might be lost. This is a
common problem on Windows based systems.
@n @n To avoid lost data, it is usually sufficient to #close
the <code>SocketHubAppender</code> either explicitly or by calling
the LogManager#shutdown method before
exiting the application.
*/

class LOG4CXXNG_EXPORT SocketHubAppender : public AppenderSkeleton
{
	private:
		/**
		The default port number of the ServerSocket will be created on.
		*/
		static int DEFAULT_PORT;

		int port;
		ObjectOutputStreamList streams;
		bool locationInfo;

	public:
		DECLARE_LOG4CXXNG_OBJECT(SocketHubAppender)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(SocketHubAppender)
		LOG4CXXNG_CAST_ENTRY_CHAIN(AppenderSkeleton)
		END_LOG4CXXNG_CAST_MAP()

		SocketHubAppender();
		~SocketHubAppender();

		/**
		Connects to remote server at <code>address</code> and <code>port</code>.
		*/
		SocketHubAppender(int port) ;

		/**
		Set up the socket server on the specified port.
		*/
		virtual void activateOptions(log4cxxng::helpers::Pool& p);

		/**
		Set options
		*/
		virtual void setOption(const LogString& option, const LogString& value);

		virtual void close();

		/**
		Append an event to all of current connections. */
		virtual void append(const spi::LoggingEventPtr& event, log4cxxng::helpers::Pool& p);

		/**
		The SocketHubAppender does not use a layout. Hence, this method returns
		<code>false</code>. */
		virtual bool requiresLayout() const
		{
			return false;
		}

		/**
		The <b>Port</b> option takes a positive integer representing
		the port where the server is waiting for connections. */
		inline void setPort(int port1)
		{
			this->port = port1;
		}

		/**
		Returns value of the <b>Port</b> option. */
		inline int getPort() const
		{
			return port;
		}

		/**
		The <b>LocationInfo</b> option takes a boolean value. If true,
		the information sent to the remote host will include location
		information. By default no location information is sent to the server. */
		inline void setLocationInfo(bool locationInfo1)
		{
			this->locationInfo = locationInfo1;
		}

		/**
		Returns value of the <b>LocationInfo</b> option. */
		inline bool getLocationInfo() const
		{
			return locationInfo;
		}

		/**
		Start the ServerMonitor thread. */
	private:
		void startServer();

		helpers::Thread thread;
		static void* LOG4CXXNG_THREAD_FUNC monitor(apr_thread_t* thread, void* data);

}; // class SocketHubAppender
LOG4CXXNG_PTR_DEF(SocketHubAppender);
}  // namespace net
} // namespace log4cxxng


#if defined(_MSC_VER)
	#pragma warning ( pop )
#endif

#endif // _LOG4CXXNG_NET_SOCKET_HUB_APPENDER_H
