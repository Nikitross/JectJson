/*-- File description -------------------------------------------------------*/
/**
 *   @file:    example_base.cpp
 *
 *   @author:  nikita.lutsenko
 *
 */

 /*-- Other libraries --------------------------------------------------------*/

#include <jectjson.h>

/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Standard C/C++ Libraries -----------------------------------------------*/

#include <string>
#include <iostream>

/*-- Imported functions -----------------------------------------------------*/
/*-- Local namespace --------------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local Variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Main function ----------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/

namespace json_parser_exaple {

	static void ErrHandl(const char* message, const char* details) {
		std::cout << message << " " << details << std::endl;
	}

	static void example_jectjson() {

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

		std::string data_string_array[] = { "data1", "data2", "data3" };
		json.Key("projects").Key("configs").Key("config", 0).AddArray("data", data_string_array, 3);

		json.PrintIt();

		std::cout << "get int id: "    << json.Key("projects").Key("configs").Key("config", 0).GetValue<int>("id") << std::endl;
		std::cout << "get string id: " << json.Key("projects").Key("configs").Key("config", 1).GetValue<std::string>("id") << std::endl;
		std::cout << "get float id:"   << json.Key("projects").Key("configs").Key("config", 2).GetValue<float>("id") << std::endl;

		std::cout << "get data value index = 0: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 0) << std::endl;
		std::cout << "get data value index = 1: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 1) << std::endl;
		std::cout << "get data value index = 2: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 2) << std::endl;

		int size_arr = json.Key("projects").Key("configs").Key("config", 0).GetSizeArray("data");
		std::cout << "get size array of data: " << size_arr << std::endl;

		json.Key("projects").Key("configs").Key("config", 0).ChangeValue("name", "JectJson config");
		std::cout << "get config name: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("name") << std::endl;

		std::string data_string_array_new[] = { "1","2","3" };
		json.Key("projects").Key("configs").Key("config", 0).ChangeArray("data", data_string_array_new, 3);

		std::cout << "get data value index = 0: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 0) << std::endl;
		std::cout << "get data value index = 1: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 1) << std::endl;
		std::cout << "get data value index = 2: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 2) << std::endl;

		json.PrintIt();

		json.Save(); 
	}
}

int main() {

	json_parser_exaple::example_jectjson();

	return EXIT_SUCCESS;
}

/*-- EOF --------------------------------------------------------------------*/
