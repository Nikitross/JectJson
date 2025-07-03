/*-- File description -------------------------------------------------------*/
/**
 *   @file:    main.h
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

	static void ErrHandl(const char *message, const char *details) {
		std::cout << message << " " << details << std::endl;
	}

	static void ErrHandl2(const std::string& modul_name,const std::string& message,const std::string& details) {
		std::cout << modul_name << message << " " << details << std::endl;
	}

	static void TEST() {

		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl2);
						
			if (!json.Open("fileTest.json", JsonCpp::FileAccess::RW)) {
				fprintf(stderr, "File: fileTest.json is not found!\n");
			}
		} 

		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl2);

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

			json.PrintIt();
			json.Save();

            //================================

            std::string DataJsonBuff = json.GetStringDataBuffer();

            std::cout<<"\n\n\n"<<"TEST GET STRING DATA FROM JSON --  START \n"<<std::endl;
            std::cout<<DataJsonBuff<<std::endl;
            std::cout << "\n\n\n" << "TEST GET STRING DATA FROM JSON -- END \n" << std::endl;

            //================================

            std::cout << "\n\n\n" << "TEST SET STRING DATA FROM JSON --  START \n" << std::endl;
            JsonCpp jsonStrSet;
            jsonStrSet.SetSringDataBuffer(DataJsonBuff.c_str());
            jsonStrSet.PrintIt();
            std::cout << "\n\n\n" << "TEST SET STRING DATA FROM JSON -- END \n" << std::endl;

            //================================
		}
		
		printf("\n\n\n");

		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl);

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
			std::string *strarr = new std::string[size_arr];

			for (int i = 0; i < size_arr; i++) {
				strarr[i] = json.Key("root").Key("student_list").Key("student", 2).GetValue<std::string>("Children", i);
			}

			for (int i = 0; i < size_arr; i++) {
				std::cout << strarr[i] << std::endl;
			}
			
			delete[] strarr;
			printf("\n");

			int size_arr1 = json.Key("root").Key("student_list").Key("student", 2).GetSizeArray("Child");
			int *strarr1 = new int[size_arr1];
			
			for (int i = 0; i < size_arr1; i++) {
				strarr1[i] = json.Key("root").Key("student_list").Key("student", 2).GetValue<int>("Child", i);
			}
			
			for (int i = 0; i < size_arr1; i++) {
				std::cout << strarr1[i] << std::endl;
			}
			delete[] strarr1;
			printf("\n");
		}
		

		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl2);

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

			//================= 1 способ

			if (json.Open("test.json", JsonCpp::FileAccess::R)) {
				remove("test.json");
				json.Open("test.json", JsonCpp::FileAccess::RWA);
			} else {
				json.Open("test.json", JsonCpp::FileAccess::RWA);
			}
			json.AddObject("NIKITROSS");
			json.PrintIt();
			json.Save();

			//================= 2 способ.
			
			if (!json.Open("test.json", JsonCpp::FileAccess::RWAS)) {
				printf("ERROR RWAS\n");
			} 
			json.AddObject("NIKITROSS");
			json.PrintIt();
			json.Save();

			//=================
		}


		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl2);

			if (!json.Open("test.json", JsonCpp::FileAccess::RWAS)) {
				fprintf(stderr, "file is not found!\n");
				exit(EXIT_FAILURE);
			}
			json.AddObject("root");
			json.Key("root").AddValue("val", "hello!");
			json.Save();
		}

		{
			JsonCpp json;
			json.RegisterCallback_onError(&ErrHandl2);

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
	}
		
	class ErrorTestCall {
	public:
		ErrorTestCall() = default;
		~ErrorTestCall() = default;

		void TEST() {			
			using namespace std::placeholders;			
			JsonCpp json;
			json.RegisterCallback_onError(std::bind(&ErrorTestCall::ErrHandl2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			
			if (!json.Open("ErrorTest.json", JsonCpp::FileAccess::RW)) {
				fprintf(stderr, "File: ErrorTest.json is not found!\n");
			}
			else {
				std::cout << json.Key("root").Key("student_list").Key("student", 0).GetValue<std::string>("name") << std::endl;
			}
		}
		
		void ErrHandl1(const char *message, const char* details) {
			std::cout << message << " " << details << std::endl;
		}

		void ErrHandl2(const std::string& modul_name,const std::string& message,const std::string& details) {
			std::cout << modul_name << message << " " << details << std::endl;
		}

	};
	
}

int main() {
	 
	json_parser_exaple::TEST();

	json_parser_exaple::ErrorTestCall obj;

	obj.TEST();
	
	return EXIT_SUCCESS;
}

/*-- EOF --------------------------------------------------------------------*/
