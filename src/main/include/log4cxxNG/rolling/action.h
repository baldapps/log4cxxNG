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

#if !defined(_LOG4CXXNG_ROLLING_ACTION_H)
#define _LOG4CXXNG_ROLLING_ACTION_H

#include <log4cxxNG/portability.h>
#include <log4cxxNG/helpers/objectimpl.h>
#include <log4cxxNG/helpers/mutex.h>
#include <log4cxxNG/helpers/pool.h>

namespace log4cxxng
{
namespace rolling
{


/**
 *  A file system action performed as part of a rollover event.
 */
class Action : public virtual log4cxxng::helpers::ObjectImpl
{
		DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(Action)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(Action)
		END_LOG4CXXNG_CAST_MAP()
		/**
		 * Is action complete.
		 */
		bool complete;

		/**
		 * Is action interrupted.
		 */
		bool interrupted;

		log4cxxng::helpers::Pool pool;
		log4cxxng::helpers::Mutex mutex;


	protected:
		/**
		 * Constructor.
		 */
		Action();
		virtual ~Action();

	public:
		/**
		 * Perform action.
		 *
		 * @return true if successful.
		 */
		virtual bool execute(log4cxxng::helpers::Pool& pool) const = 0;

		void run(log4cxxng::helpers::Pool& pool);

		void close();

		/**
		 * Tests if the action is complete.
		 * @return true if action is complete.
		 */
		bool isComplete() const;

		void reportException(const std::exception&);


};

LOG4CXXNG_PTR_DEF(Action);

}
}
#endif

