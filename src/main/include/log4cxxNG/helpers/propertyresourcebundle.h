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

#ifndef _LOG4CXXNG_HELPERS_PROPERTY_RESOURCE_BUNDLE_H
#define _LOG4CXXNG_HELPERS_PROPERTY_RESOURCE_BUNDLE_H

#include <log4cxxNG/helpers/resourcebundle.h>
#include <log4cxxNG/helpers/properties.h>
#include <log4cxxNG/helpers/inputstream.h>

namespace log4cxxng
{
namespace helpers
{

/**
PropertyResourceBundle is a concrete subclass of ResourceBundle that
manages resources for a locale using a set of static strings from a
property file.
*/
class LOG4CXXNG_EXPORT PropertyResourceBundle : public ResourceBundle
{
	public:
		DECLARE_ABSTRACT_LOG4CXXNG_OBJECT(PropertyResourceBundle)
		BEGIN_LOG4CXXNG_CAST_MAP()
		LOG4CXXNG_CAST_ENTRY(PropertyResourceBundle)
		LOG4CXXNG_CAST_ENTRY_CHAIN(ResourceBundle)
		END_LOG4CXXNG_CAST_MAP()

		/**
		Creates a property resource bundle.
		@param inStream property file to read from.
		@throw IOException if an error occurred when reading from the
		input stream.
		*/
		PropertyResourceBundle(InputStreamPtr inStream);

		virtual LogString getString(const LogString& key) const;

	protected:
		Properties properties;
}; // class PropertyResourceBundle
LOG4CXXNG_PTR_DEF(PropertyResourceBundle);
}  // namespace helpers
} // namespace log4cxxng

#endif // _LOG4CXXNG_HELPERS_PROPERTY_RESOURCE_BUNDLE_H

