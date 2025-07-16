/*-- File description -------------------------------------------------------*/
/**
 *   @file:    example_base.cpp
 *
 *   @author:  nikita.lutsenko
 *
 */

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/

#include <jectjson.h>

/*-- Standard C/C++ Libraries -----------------------------------------------*/

#include <string>
#include <iostream>

/*-- Imported functions -----------------------------------------------------*/
/*-- Local namespace --------------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/

class ErrorTestCall {
public:
	ErrorTestCall() = default;
	~ErrorTestCall() = default;

	void TEST() {

		JsonCpp json;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
		json.SetErrorCallback(std::bind(&ErrorTestCall::ErrorHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
#else
		json.SetErrorCallback(std::bind(&ErrorTestCall::ErrorHandler, std::placeholders::_1));
#endif // ENABLE_JSONCPP_DETAILS_ERROR_HANDLER


		if (!json.Open("ErrorTest.json", JsonCpp::FileAccess::RW)) {
			fprintf(stderr, "File: ErrorTest.json is not found!\n");
		}
		else {
			std::cout << json.Key("root").Key("student_list").Key("student", 0).GetValue<std::string>("name") << std::endl;
		}
	}

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER	

	void ErrorHandler(const std::string& func_name, const std::string& msg, JsonCpp::ErrorCode error) {
		std::cout << func_name << " " << msg << " Error id: " << static_cast<int>(error) << std::endl;
		std::cout << "Class name: " << this->name << std::endl;
	}

#else

	static void ErrorHandler(const JsonCpp::ErrorCode error) {
		std::cout << "Error id: " << static_cast<int>(error) << std::endl;
	}

#endif
private:

	const char* name = "ErrorTestCall";

};

/*-- Local Variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER	

static void ErrorHandler(const std::string& func_name, const std::string& msg, JsonCpp::ErrorCode error) {
	std::cout << func_name << " " << msg << " Error id: " << static_cast<int>(error) << std::endl;
}

#else

static void ErrorHandler(const JsonCpp::ErrorCode error) {
	std::cout << "Error id: " << static_cast<int>(error) << std::endl;
}

#endif

static void example_000(void) {

	remove("jectjson_example.json");

	JsonCpp json;
	json.SetErrorCallback(ErrorHandler);

	// READ + WRITE + CREATE
	if (!json.Open("jectjson_example.json", JsonCpp::FileAccess::R)) {
		fprintf(stderr, "Error - Open json file.");
	}

	// READ + WRITE + CREATE
	if (!json.Open("jectjson_example.json", JsonCpp::FileAccess::RWA)) {
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

	std::string data_string_array_new[] = { "1","2","3" };
	json.Key("projects").Key("configs").Key("config", 0).ChangeArray("data", data_string_array_new, 3);

	std::cout << "get data value index = 0: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 0) << std::endl;
	std::cout << "get data value index = 1: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 1) << std::endl;
	std::cout << "get data value index = 2: " << json.Key("projects").Key("configs").Key("config", 0).GetValue<std::string>("data", 2) << std::endl;

	json.PrintIt();

	json.Save();
}

static void example_001(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("fileTest.json", JsonCpp::FileAccess::RW)) {
		fprintf(stderr, "File: fileTest.json is not found!\n");
	}
}

static void example_002(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("test.json", JsonCpp::FileAccess::RWA)) {
		fprintf(stderr, "file is not found!");
		exit(EXIT_FAILURE);
	}

	json.AddObject("root");
	json.Key("root").AddObject("student_list");
	json.Key("root").Key("student_list").AddObjectsArray("student", 3);

	json.Key("root").Key("student_list").Key("student", 0).AddValue("name", "Mikle");
	json.Key("root").Key("student_list").Key("student", 0).AddValue("age", 34);
	json.Key("root").Key("student_list").Key("student", 0).AddValue("sexM", true);

	json.Key("root").Key("student_list").Key("student", 1).AddValue("name", "Andry");
	json.Key("root").Key("student_list").Key("student", 1).AddValue("age", 12);
	json.Key("root").Key("student_list").Key("student", 1).AddValue("sexM", true);

	std::string child[] = { "Nina","Pety","Liza" };
	json.Key("root").Key("student_list").Key("student", 2).AddArray("Children", child, 3);

	int agechild[4] = { 19,8,5,88 };
	json.Key("root").Key("student_list").Key("student", 2).AddArray("Child", agechild, 4);

	uint8_t agechild1[3] = { 19,8,5 };
	json.Key("root").Key("student_list").Key("student", 2).AddArray("Children1", agechild1, 3);

	uint32_t agechild2[3] = { 19,8,5 };
	json.Key("root").Key("student_list").Key("student", 2).AddArray("Children2", agechild2, 3);

	bool sexChild4[] = { false,true,false };
	json.Key("root").Key("student_list").Key("student", 2).AddArray("sexChild4", sexChild4, sizeof(sexChild4) / sizeof(bool));

	json.Key("root").Key("student_list").Key("student", 0).ChangeValue("name", "Roma");
	json.Key("root").Key("student_list").Key("student", 0).ChangeValue("age", 31);

	json.PrintIt();
	json.Save();

	//================================

	std::string DataJsonBuff = json.GetStringDataBuffer();

	std::cout << "\n\n\n" << "TEST GET STRING DATA FROM JSON --  START \n" << std::endl;
	std::cout << DataJsonBuff << std::endl;
	std::cout << "\n\n\n" << "TEST GET STRING DATA FROM JSON -- END \n" << std::endl;

	//================================

	std::cout << "\n\n\n" << "TEST SET STRING DATA FROM JSON --  START \n" << std::endl;
	JsonCpp jsonStrSet;
	jsonStrSet.SetSringDataBuffer(DataJsonBuff.c_str());
	jsonStrSet.PrintIt();
	std::cout << "\n\n\n" << "TEST SET STRING DATA FROM JSON -- END \n" << std::endl;


}

static void example_003(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("test.json", JsonCpp::FileAccess::RWA)) {
		fprintf(stderr, "file is found!");
		exit(EXIT_FAILURE);
	}

	std::cout << json.Key("root").Key("student_list").Key("student", 0).GetValue<std::string>("name") << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 0).GetValue<int>("age") << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 0).GetValue<bool>("sexM") << std::endl;
	printf("\n");

	std::cout << json.Key("root").Key("student_list").Key("student", 1).GetValue<std::string>("name") << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 1).GetValue<int>("age") << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 1).GetValue<bool>("sexM") << std::endl;
	printf("\n");

	std::cout << json.Key("root").Key("student_list").Key("student", 2).GetValue<std::string>("Children", 0) << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 2).GetValue<std::string>("Children", 1) << std::endl;
	std::cout << json.Key("root").Key("student_list").Key("student", 2).GetValue<std::string>("Children", 2) << std::endl;
	printf("\n");

	int size_arr = json.Key("root").Key("student_list").Key("student", 2).GetSizeArray("Children");
	std::string* strarr = new std::string[size_arr];

	for (int i = 0; i < size_arr; i++) {
		strarr[i] = json.Key("root").Key("student_list").Key("student", 2).GetValue<std::string>("Children", i);
	}

	for (int i = 0; i < size_arr; i++) {
		std::cout << strarr[i] << std::endl;
	}

	delete[] strarr;
	printf("\n");

	int size_arr1 = json.Key("root").Key("student_list").Key("student", 2).GetSizeArray("Child");
	int* strarr1 = new int[size_arr1];

	for (int i = 0; i < size_arr1; i++) {
		strarr1[i] = json.Key("root").Key("student_list").Key("student", 2).GetValue<int>("Child", i);
	}

	for (int i = 0; i < size_arr1; i++) {
		std::cout << strarr1[i] << std::endl;
	}
	delete[] strarr1;
	printf("\n");

}

static void example_004(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("test.json", JsonCpp::FileAccess::RW)) {
		fprintf(stderr, "file is not found!\n");
		exit(EXIT_FAILURE);
	}
	json.Key("root").Key("student_list").Key("jdkfjkd").GetValue<int>("hello");

	json.Key("root").Key("student_list").Key("student", 0).ChangeValue("name", "Roma");
	json.Key("root").Key("student_list").Key("student", 0).ChangeValue("age", 31);
	json.Key("root").Key("student_list").Key("student", 0).ChangeValue("sexM", true);

	json.Key("root").Key("student_list").Key("student", 1).ChangeValue("name", "Lena");
	json.Key("root").Key("student_list").Key("student", 1).ChangeValue("age", 20);
	json.Key("root").Key("student_list").Key("student", 1).ChangeValue("sexM", false);

	std::string child[] = { "COCA","COLA","Liza" };
	json.Key("root").Key("student_list").Key("student", 2).ChangeArray("Children", child, 3);

	int agechild[4] = { 10,9,8,7 };
	json.Key("root").Key("student_list").Key("student", 2).ChangeArray("Child", agechild, 4);

	json.PrintIt();
	json.Save();

	//================= 

	if (json.Open("test.json", JsonCpp::FileAccess::R)) {
		remove("test.json");
		json.Open("test.json", JsonCpp::FileAccess::RWA);
	}
	else {
		json.Open("test.json", JsonCpp::FileAccess::RWA);
	}
	json.AddObject("NIKITROSS");
	json.PrintIt();
	json.Save();

	//================= 

	if (!json.Open("test.json", JsonCpp::FileAccess::RWAS)) {
		printf("ERROR RWAS\n");
	}
	json.AddObject("NIKITROSS");
	json.PrintIt();
	json.Save();

}

static void example_005(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("test.json", JsonCpp::FileAccess::RWAS)) {
		fprintf(stderr, "file is not found!\n");
		exit(EXIT_FAILURE);
	}
	json.AddObject("root");
	json.Key("root").AddValue("val", "hello!");
	json.Save();

}

static void example_006(void) {
	JsonCpp json;
	json.SetErrorCallback(&ErrorHandler);

	if (!json.Open("", JsonCpp::FileAccess::VIRTUAL)) {
		fprintf(stderr, "file is not found!\n");
		exit(EXIT_FAILURE);
	}
	json.AddObject("root");
	json.Key("root").AddValue("val", 44);
	json.Key("root").AddValue("age", 4);
	json.Key("root").AddValue("name", "bayby");
	json.PrintIt();

	if (!json.Save()) {
		std::cout << "\nNOT HAS PATH! add path save!!!" << std::endl;
	}
	json.Save("virtual.json");

}

static void example_007(void) {
	ErrorTestCall obj;
	obj.TEST();
}

static void run_examples(void) {

	example_000();
	example_001();
	example_002();
	example_003();
	example_004();
	example_005();
	example_006();
	example_007();
}

/*-- Main function ----------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/

int main() {

	run_examples();

	return EXIT_SUCCESS;
}

/*-- EOF --------------------------------------------------------------------*/
