## What is it?

Log4cxxNG is a C++ logging library based on Apache log4cxx project.

Requirments:
* Apache Portable Runtime
* CMake 3.11
* Compiler with C++11 support

## Features

* User can select different LogLevels – TRACE, DEBUG, INFO, WARN, ERROR, and FATAL
* Hierarchical Loggers
* It is possible to log asynchronously
* Supports multiple appenders
* User can select to enabled or disabled the logger
* Log can be sent to different and multiple output targets
* User selected output formats
* Really similar to Log4j project
* Cross-platform

For additional information see Log4cxx Apache project web page [here](https://logging.apache.org/log4cxx/latest_stable/)

## HowTo

Log4cxxNG use the same methods of Log4cxx so if you are already using Log4cxx
the porting is really easy. You need to be careful only to init and shutdown phases.
Here a basic example:

```cpp
#include "com/foo/bar.h"
using namespace com::foo;
    
// include log4cxxng header files.
#include "log4cxxNG/logger.h"
#include "log4cxxNG/logmanager.h"
#include "log4cxxNG/basicconfigurator.h"
#include "log4cxxNG/helpers/exception.h"
    
using namespace log4cxxng;
using namespace log4cxxng::helpers;
    
int main(int argc, char **argv)
{
    int result = EXIT_SUCCESS;
    try
    {
        //first method to call in your main
        LogManager::init();
        // Set up a simple configuration that logs on the console.
        BasicConfigurator::configure();

        LoggerPtr logger(Logger::getLogger("MyApp"));
        LOG4CXXNG_INFO(logger, "Entering application.")
        Bar bar;
        bar.doIt();
        LOG4CXXNG_INFO(logger, "Exiting application.")
    }
    catch(Exception&)
    {
        result = EXIT_FAILURE;
    }
    
    //call before exit
    LogManager::shutdown();
    return result;
}
```

## Bugs

Please report bugs in the GitHub issue tracker.

## License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License. You can
see the LICENSE files for any detail. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
