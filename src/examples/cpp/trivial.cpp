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
#include <stdlib.h>
#include <log4cxxNG/logger.h>
#include <log4cxxNG/basicconfigurator.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/ndc.h>
#include <locale.h>

using namespace log4cxxng;
using namespace log4cxxng::helpers;

int main()
{
    setlocale(LC_ALL, "");
    int result = EXIT_SUCCESS;
    try
    {
                BasicConfigurator::configure();
                LoggerPtr rootLogger = Logger::getRootLogger();

                NDC::push("trivial context");

                LOG4CXXNG_DEBUG(rootLogger, "debug message");
                LOG4CXXNG_INFO(rootLogger, "info message");
                LOG4CXXNG_WARN(rootLogger, "warn message");
                LOG4CXXNG_ERROR(rootLogger, "error message");
                LOG4CXXNG_FATAL(rootLogger, "fatal message");

                NDC::pop();
        }
        catch(std::exception&)
        {
                result = EXIT_FAILURE;
        }

    return result;
}
