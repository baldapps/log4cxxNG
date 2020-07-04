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

#ifndef _LOG4CXXNG_HELPERS_MUTEX_H
#define _LOG4CXXNG_HELPERS_MUTEX_H

#include <log4cxxNG/log4cxxNG.h>

#if defined(RW_MUTEX)
	#include <apr_portable.h>
	#include <atomic>
#endif

extern "C" {
	struct apr_thread_mutex_t;
	struct apr_pool_t;
	struct apr_thread_rwlock_t;
}


namespace log4cxxng
{
namespace helpers
{
class Pool;

class LOG4CXXNG_EXPORT Mutex
{
	public:
		Mutex(log4cxxng::helpers::Pool& p);
		Mutex(apr_pool_t* p);
		~Mutex();
		apr_thread_mutex_t* getAPRMutex() const;

	private:
		Mutex(const Mutex&);
		Mutex& operator=(const Mutex&);
		apr_thread_mutex_t* mutex;
};
} // namespace helpers
} // namespace log4cxxng


#if defined(RW_MUTEX)

namespace log4cxxng
{
namespace helpers
{
class Pool;

class LOG4CXXNG_EXPORT RWMutex
{
	public:
		RWMutex(log4cxxng::helpers::Pool& p);
		RWMutex(apr_pool_t* p);
		~RWMutex();

		void rdLock() const;
		void rdUnlock() const;

		void wrLock() const;
		void wrUnlock() const;

	private:
		mutable std::atomic<apr_os_thread_t> id;
		mutable unsigned count;
		RWMutex(const RWMutex&);
		RWMutex& operator=(const RWMutex&);
		apr_thread_rwlock_t* mutex;
};
} // namespace helpers
} // namespace log4cxxng

#define SHARED_MUTEX log4cxxng::helpers::RWMutex

#else // no RW_MUTEX

#define SHARED_MUTEX log4cxxng::helpers::Mutex

#endif // RW_MUTEX

#define SHARED_MUTEX_INIT(mutex, p) mutex(p)

#if defined(NON_BLOCKING)

namespace log4cxxng
{
namespace helpers
{
struct SemaphoreImpl;

class LOG4CXXNG_EXPORT Semaphore
{
	public:
		Semaphore(log4cxxng::helpers::Pool& p);
		~Semaphore();

		void await() const;
		void signalAll() const;

	private:
		Semaphore(const Semaphore&);
		Semaphore& operator=(const Semaphore&);

		SemaphoreImpl* impl;
};
} // namespace helpers
} // namespace log4cxxng

#endif // NON_BLOCKING

#endif //_LOG4CXXNG_HELPERS_MUTEX_H
