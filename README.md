![JectJson logo](doc/logo/jectjson.png)

# JectJson - Modern C++ JSON Facade, or JSON PARSER

JectJson is a lightweight wrapper library built atop RapidJSON, 
designed to provide Python/JavaScript-like simplicity for JSON handling in C++. 
It combines RapidJSON's performance with intuitive API design for seamless integration.

Copyright (c) 2024 Nikitross

## Why JectJson?

- Compared to raw RapidJSON:
- 70% less boilerplate
- Type-safe by design
- Predictable error handling
- Fluent method chaining

## Key Features

- **Fluent interface**      - Chainable methods for intuitive JSON construction
- **Type-safe operations**  - Automatic type conversion with error handling
- **Fault-tolerant design** - Graceful handling of malformed JSON
- **Full CRUD support**     - Create, Read, Update, and Delete JSON elements with ease
- **Memory-safe**           - Automatic resource management

### Workflow

In general, we follow the "fork-and-pull" Git workflow.

 1. **Fork** the repo on GitHub
 2. **Clone** the project to your own machine
 3. **Checkout** a new branch on your fork, start developing on the branch
 4. **Test** the change before commit, Make sure the changes pass all the tests, including `unittest` and `preftest`, please add test case for each new feature or bug-fix if needed.
 5. **Commit** changes to your own branch
 6. **Push** your work back up to your fork
 7. Submit a **Pull request** so that we can review your changes

NOTE: Be sure to merge the latest from "upstream" before making a pull request!

## Build

mkdir build && cd build
cmake .. 
make 

## Example

~~~~~~~~~~cpp

#include <string>
#include <iostream>
#include <jectjson.h>

static void ErrHandl(const char *message, const char *details) {
	std::cout << message << " " << details << std::endl;
}

int main() {
    
    	JsonCpp json;
		json.RegisterCallback_onError(&ErrHandl);
		
		// READ + WRITE + CREATE
		if (!json.Open("jectjson_examle.json", JsonCpp::FileAccess::RWA)) {
			fprintf(stderr, "Error - Open json file.");
		}

		json.AddObject("projects");
		json.Key("projects").AddObject("configs");
		json.Key("projects").Key("configs").AddObjectsArray("config", 3);

		json.Key("projects").Key("configs").Key("config", 0).AddValue("id", 1);
		json.Key("projects").Key("configs").Key("config", 0).AddValue("name", "config name");
		json.Key("projects").Key("configs").Key("config", 0).AddValue("dev", "usb");

		json.Key("projects").Key("configs").Key("config", 1).AddValue("id", "2");
		json.Key("projects").Key("configs").Key("config", 2).AddValue("id", 3.433333333);

		std::string data1[] = { "data1","data2","data3" };
		json.Key("projects").Key("configs").Key("config", 0).AddArray("data", data1, 3);

		json.PrintIt();

		std::cout << "get int id: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<int>("id") << std::endl;
		std::cout << "get string id: " << json.Key("projects").Key("configs").Key("config", 1).GetValue<std::string>("id") << std::endl;
		std::cout << "get float id:" << json.Key("projects").Key("configs").Key("config", 2).GetValue<float>("id") << std::endl;

		std::cout << "get data value index = 0: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 0) << std::endl;
		std::cout << "get data value index = 1: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 1) << std::endl;
		std::cout << "get data value index = 2: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 2) << std::endl;

		int size_arr = json.Key("projects").Key("configs").Key("config", 0).GetSizeArray("data");
		std::cout << "get size array of data: " << size_arr << std::endl;

		json.Key("projects").Key("configs").Key("config", 0).ChangeValue("name", "JectJson config");
		std::cout << "get config name: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("name") << std::endl;

		std::string data2[] = { "1","2","3" };
		json.Key("projects").Key("configs").Key("config", 0).ChangeArray("data", data2, 3);

		std::cout << "get data value index = 0: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 0) << std::endl;
		std::cout << "get data value index = 1: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 1) << std::endl;
		std::cout << "get data value index = 2: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 2) << std::endl;

		json.PrintIt();

		json.Save();

    return 0;
}
~~~~~~~~~~

More [examples](https://github.com/Nikitross/JectJson/tree/main/example) are available:

### Copyright and Licensing

This project uses RapidJSON (https://github.com/Tencent/rapidjson),
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

JectJson license 
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
