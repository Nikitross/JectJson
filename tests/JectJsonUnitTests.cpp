
#include <gtest/gtest.h>
#include <jectjson.h>
#include <string>
#include <filesystem>
#include <fstream>

static const std::string test_file_path = "json_test.json";

static bool fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

static bool fileContains(const std::string& filename, const std::string& content) {

    if (!fileExists(filename))
        return false;

    std::ifstream file(filename);
    std::string fileContent((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    return fileContent.find(content) != std::string::npos;
}

using namespace testing;

class JsonCppErrorControlTest : public ::testing::Test {
protected:
    JsonCpp json;

    void SetUp() override {
        json.ClearErrorState();
    }
};


TEST(JsonParserTest, BasicTest) {
   
    JsonCpp json;
	
    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("root");
    json.Key("root").AddValue("key", "value");

    EXPECT_EQ(json.Key("root").GetValue<std::string>("key"), "value");
	remove(test_file_path.c_str());
}

TEST(JsonParserTest, CreateAndReadBasicJson) {
    
    JsonCpp json;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER
    
    auto localErrorHandler = [](const std::string& func_name, const std::string& message, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << message << " Error code: " << static_cast<int>(error) << std::endl;
    };
#else
    auto localErrorHandler = [](const JsonCpp::ErrorCode error) {
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
	};

#endif

    json.SetErrorCallback(localErrorHandler);
    
    ASSERT_FALSE(json.Open(test_file_path, JsonCpp::FileAccess::R));
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
    json.ClearErrorState();

    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL));

    json.AddObject("root");
    json.Key("root").AddValue("name", "Test");
    json.Key("root").AddValue("value", 42);
     
    EXPECT_EQ(json.Key("root").GetValue<std::string>("name"), "Test");
    EXPECT_EQ(json.Key("root").GetValue<int>("value"), 42);

    const std::string temp_file_path = "json_temp_test.json";
    ASSERT_TRUE(json.Save(temp_file_path.c_str()));

    JsonCpp jsonLoad;
    ASSERT_TRUE(jsonLoad.Open(temp_file_path, JsonCpp::FileAccess::RW));
    EXPECT_EQ(jsonLoad.Key("root").GetValue<std::string>("name"), "Test");
    remove(temp_file_path.c_str());
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, NestedObjects) {
    
    JsonCpp json;
    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);

    json.AddObject("school");
    json.Key("school").AddObject("classA");
    json.Key("school").Key("classA").AddObjectsArray("students", 2);
    
    json.Key("school").Key("classA").Key("students", 0).AddValue("name", "Alice");
    json.Key("school").Key("classA").Key("students", 0).AddValue("age", 20);

    json.Key("school").Key("classA").Key("students", 1).AddValue("name", "Bob");
    json.Key("school").Key("classA").Key("students", 1).AddValue("age", 21);

    EXPECT_EQ(
        json.Key("school").Key("classA").Key("students", 0).GetValue<std::string>("name"),
        "Alice"
    );
    EXPECT_EQ(
        json.Key("school").Key("classA").Key("students", 1).GetValue<int>("age"),
        21
    );
}

TEST(JsonParserTest, ArrayOperations) {
    
    JsonCpp json;

    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);

    int numbers[] = { 1, 2, 3 };
    json.AddObject("data");
    json.Key("data").AddArray("numbers", numbers, 3);

    std::string strings[] = { "a", "b", "c" };
    json.Key("data").AddArray("strings", strings, 3);

    bool bools[] = { true, false };
    json.Key("data").AddArray("bools", bools, 2);

    EXPECT_EQ(json.Key("data").GetSizeArray("numbers"), 3);
    EXPECT_EQ(json.Key("data").GetValue<int>("numbers", 1), 2);

    EXPECT_EQ(json.Key("data").GetSizeArray("strings"), 3);
    EXPECT_EQ(json.Key("data").GetValue<std::string>("strings", 0), "a");

    EXPECT_EQ(json.Key("data").GetSizeArray("bools"), 2);
    EXPECT_EQ(json.Key("data").GetValue<bool>("bools", 1), false);

    int newNumbers[] = { 4, 5 };
    json.Key("data").ChangeArray("numbers", newNumbers, 2);
    
    EXPECT_EQ(json.Key("data").GetSizeArray("numbers"), 2);
}

TEST(JsonParserTest, ErrorHandling) {
    
    JsonCpp json;

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif


    json.SetErrorCallback(localErrorHandler);

    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("test");

    json.Key("test").GetValue<int>("invalid_key");
    EXPECT_TRUE(errorCalled);
	json.ClearErrorState();

    errorCalled = false;
    EXPECT_FALSE(json.Open("nonexistent.json", JsonCpp::FileAccess::RW));
    EXPECT_TRUE(errorCalled);
}

TEST(JsonParserTest, StringBufferOperations) {
    
    JsonCpp json;

    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);

    json.AddObject("data");
    json.Key("data").AddValue("key", "value");

    std::string buffer = json.GetStringDataBuffer();
    EXPECT_FALSE(buffer.empty());

    JsonCpp json2;   
    json2.SetSringDataBuffer(buffer.c_str());
    EXPECT_EQ(json2.Key("data").GetValue<std::string>("key"), "value");
}

TEST(JsonParserTest, DataModification) {
    
    JsonCpp json;
	
    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);

    json.AddObject("test");
    json.Key("test").AddValue("value", 100);
    EXPECT_EQ(json.Key("test").GetValue<int>("value"), 100);

    json.Key("test").ChangeValue("value", 200);
    EXPECT_EQ(json.Key("test").GetValue<int>("value"), 200);

    std::string oldArr[] = { "a", "b" };
    std::string newArr[] = { "x", "y", "z" };
    json.Key("test").AddArray("items", oldArr, 2);
    json.Key("test").ChangeArray("items", newArr, 3);
    EXPECT_EQ(json.Key("test").GetSizeArray("items"), 3);
}

TEST(JsonParserTest, FileOperations) {

    {
        JsonCpp json;
        json.Open(test_file_path, JsonCpp::FileAccess::RWA);
        json.AddObject("data");
        json.Key("data").AddValue("created", true);
        ASSERT_TRUE(json.Save());
    }

    {
        JsonCpp json;
        ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RW));
        EXPECT_TRUE(json.Key("data").GetValue<bool>("created"));
    }

    remove(test_file_path.c_str());
}

TEST(JsonParserTest, SpecialCases) {
   
    JsonCpp json;

    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);
    EXPECT_NO_THROW(json.PrintIt());

    json.AddObject("root");
    json.Key("root").AddObjectsArray("items", 2);
    json.Key("root").Key("items", 0).AddValue("id", 1);
    json.Key("root").Key("items", 1).AddValue("id", 2);
    EXPECT_EQ(json.Key("root").GetSizeArray("items"), 2);

    json.Key("root").AddValue("flag", false);
    EXPECT_FALSE(json.Key("root").GetValue<bool>("flag"));
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, OpenJsonFile) {

   
    JsonCpp json;

    ASSERT_FALSE(json.Open(test_file_path, JsonCpp::FileAccess::R));
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
    json.ClearErrorState();

    ASSERT_FALSE(json.Open(test_file_path, JsonCpp::FileAccess::RW));
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
	json.ClearErrorState();

    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));
    EXPECT_EQ(json.ObjectIsEmpty(), false);

    json.AddObject("data");
    json.Key("data").AddValue("created", true);
    ASSERT_TRUE(json.Save());
 
	ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::R)); // TODO Check if this file olready opened.
    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RW));
    json.ClearObject();

    remove(test_file_path.c_str());

	ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL)); // TODO Check save wen file olready exists.
    ASSERT_TRUE(json.Save());
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, OpenWithReadAccessOnExistingFile) {
    
    {
        std::ofstream out(test_file_path);
        out << "{\"key\":\"value\"}";
        out.close();
    }

    JsonCpp json;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        FAIL() << "Error callback should not be called";
    };

#else

    auto localErrorHandler = [](const JsonCpp::ErrorCode error) {
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
        FAIL() << "Error callback should not be called";
    };

#endif

    json.SetErrorCallback(localErrorHandler);

    EXPECT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::R));
	remove(test_file_path.c_str());
}

TEST(JsonParserTest, OpenWithReadAccessOnNonExistingFile) {
   
    JsonCpp json;

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif

	json.SetErrorCallback(localErrorHandler);

    EXPECT_FALSE(json.Open("nonexistent.json", JsonCpp::FileAccess::R));
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
    EXPECT_TRUE(errorCalled);
}

TEST(JsonParserTest, OpenWithRWAAccessCreatesNewFile) {
   
    JsonCpp json;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        FAIL() << "Error callback should not be called";
    };

#else

    auto localErrorHandler = [](const JsonCpp::ErrorCode error) {
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
        FAIL() << "Error callback should not be called";
    };

#endif

    json.SetErrorCallback(localErrorHandler);

    EXPECT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));
    EXPECT_TRUE(fileExists(test_file_path));
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, VirtualFileOperations) {

    JsonCpp json;

    EXPECT_TRUE(json.Open("", JsonCpp::FileAccess::VIRTUAL));

    json.AddObject("root");
    json.Key("root").AddValue("test", 123);

    EXPECT_EQ(json.Key("root").GetValue<int>("test"), 123);

    EXPECT_TRUE(json.Save(test_file_path.c_str()));
    EXPECT_TRUE(fileContains(test_file_path, "test"));
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, AddObjectAndKeyOperations) {

    JsonCpp json;

    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));

    json.AddObject("root");
    json.Key("root").AddObject("nested");

    json.Key("root").Key("nested").AddValue("int_val", 42);
    json.Key("root").Key("nested").AddValue("str_val", "hello");
    json.Key("root").Key("nested").AddValue("bool_val", true);

    EXPECT_EQ(json.Key("root").Key("nested").GetValue<int>("int_val"), 42);
    EXPECT_EQ(json.Key("root").Key("nested").GetValue<std::string>("str_val"), "hello");
    EXPECT_EQ(json.Key("root").Key("nested").GetValue<bool>("bool_val"), true);

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif


    json.SetErrorCallback(localErrorHandler);

    json.Key("root").Key("nonexistent");
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    EXPECT_TRUE(errorCalled);
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, ArrayOperations1) {
   
    JsonCpp json;
 
    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));

    json.AddObject("root");
    json.Key("root").AddObject("arrays");

    int intArr[] = { 1, 2, 3 };
    json.Key("root").Key("arrays").AddArray("int_array", intArr, 3);

    std::string strArr[] = { "a", "b", "c" };
    json.Key("root").Key("arrays").AddArray("str_array", strArr, 3);

    bool boolArr[] = { true, false, true };
    json.Key("root").Key("arrays").AddArray("bool_array", boolArr, 3);

    EXPECT_EQ(json.Key("root").Key("arrays").GetSizeArray("int_array"), 3);
    EXPECT_EQ(json.Key("root").Key("arrays").GetValue<int>("int_array", 1), 2);

    EXPECT_EQ(json.Key("root").Key("arrays").GetSizeArray("str_array"), 3);
    EXPECT_EQ(json.Key("root").Key("arrays").GetValue<std::string>("str_array", 2), "c");

    EXPECT_EQ(json.Key("root").Key("arrays").GetSizeArray("bool_array"), 3);
    EXPECT_FALSE(json.Key("root").Key("arrays").GetValue<bool>("bool_array", 1));

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif

    json.SetErrorCallback(localErrorHandler);

    json.Key("root").Key("arrays").GetValue<int>("nonexistent_array", 0);
    EXPECT_TRUE(errorCalled);
	EXPECT_TRUE(json.HasErrorCode());
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, ModifyOperations) {
    
    JsonCpp json;

    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));

    json.AddObject("root");
    json.Key("root").AddValue("value", "old");
    json.Key("root").AddArray("array", std::vector<int>{1, 2, 3}.data(), 3);

	EXPECT_EQ(json.Key("root").GetValue<std::string>("value"), "old");

    json.Key("root").ChangeValue("value", "new");

    EXPECT_EQ(json.Key("root").GetValue<std::string>("value"), "new");

    int newArr[] = { 4, 5, 6 };
    
    json.Key("root").ChangeArray("array", newArr, 3);
    
    EXPECT_EQ(json.Key("root").GetValue<int>("array", 0), 4);

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif

    json.SetErrorCallback(localErrorHandler);

    json.Key("root").ChangeValue("nonexistent", 123);
	EXPECT_TRUE(json.HasErrorCode());
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    EXPECT_TRUE(errorCalled);
    json.ClearErrorState();
    errorCalled = false;

    json.Key("root").ChangeArray("nonexistent", newArr, 3);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    EXPECT_TRUE(errorCalled);
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, StringBufferOperations1) {

    JsonCpp json;
    ASSERT_TRUE(json.Open("", JsonCpp::FileAccess::VIRTUAL));

    json.AddObject("root");
    json.Key("root").AddValue("test", "value");

    std::string jsonStr = json.GetStringDataBuffer();

    EXPECT_NE(jsonStr.find("test"), std::string::npos);
    EXPECT_NE(jsonStr.find("value"), std::string::npos);

    JsonCpp json2;
    ASSERT_TRUE(json2.Open("", JsonCpp::FileAccess::VIRTUAL));
    json2.SetSringDataBuffer(jsonStr.c_str());

    EXPECT_EQ(json2.Key("root").GetValue<std::string>("test"), "value");
}

TEST(JsonParserTest, ErrorHandling1) {
    
    JsonCpp json;
 
    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&](const JsonCpp::ErrorCode error) {
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
		errorCalled = true;
    };

#endif

    json.SetErrorCallback(localErrorHandler);

    json.Open("nonexistent.json", JsonCpp::FileAccess::R);

    ASSERT_TRUE(errorCalled);
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
    
    json.ClearErrorState();
	errorCalled = false;
    
    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWA));

    json.Key("nonexistent_key");

	EXPECT_TRUE(json.HasErrorCode());
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
	ASSERT_TRUE(errorCalled);
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, RWASModeRecreatesFile) {

    {
        std::ofstream out(test_file_path);
        out << "{\"old\":\"data\"}";
        out.close();
    }

    JsonCpp json;
    ASSERT_TRUE(json.Open(test_file_path, JsonCpp::FileAccess::RWAS));

    json.AddObject("root");
    json.Key("root").AddValue("new", "data");
    json.Save();

    std::ifstream in(test_file_path);
    std::string content((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());
    
    EXPECT_EQ(content.find("old"), std::string::npos);
    EXPECT_NE(content.find("new"), std::string::npos);
    remove(test_file_path.c_str());
}

TEST(JsonParserTest, UseNotInitObject) {

    JsonCpp json;

    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif


    json.SetErrorCallback(localErrorHandler);

    json.AddObject("root");

    EXPECT_TRUE(errorCalled);
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    json.ClearErrorState();

    errorCalled = false;
    json.Key("root").AddValue("key", "value");
    EXPECT_TRUE(errorCalled);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    json.ClearErrorState();

    errorCalled = false;
    std::string val = json.Key("root").GetValue<std::string>("key");
    EXPECT_TRUE(errorCalled);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
}

TEST(JsonParserTest, FailInitTypeGetValue) {
    
    JsonCpp json;
	
    bool errorCalled = false;

#ifdef ENABLE_JSONCPP_DETAILS_ERROR_HANDLER

    auto localErrorHandler = [&errorCalled](const std::string& func_name, const std::string& msg, const JsonCpp::ErrorCode error) {
        std::cout << func_name << " " << msg << " Error code: " << static_cast<int>(error) << std::endl;
        errorCalled = true;
    };

#else

    auto localErrorHandler = [&errorCalled](const JsonCpp::ErrorCode error) {
        errorCalled = true;
        std::cout << "Error code: " << static_cast<int>(error) << std::endl;
    };

#endif


    json.SetErrorCallback(localErrorHandler);

    json.Open(test_file_path, JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("root");
    json.Key("root").AddValue("key", "value");

    int val = json.Key("root").GetValue<int>("key");

    EXPECT_TRUE(errorCalled);
    EXPECT_TRUE(json.HasErrorCode());
	EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyTypeMismatch));
	remove(test_file_path.c_str());
}

TEST_F(JsonCppErrorControlTest, AllErrorCodesAndNoOverwrite) {

	// ErrorCode::None
    EXPECT_FALSE(json.HasErrorCode());
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::None));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::None), "None");

    //ErrorCode::InvalidJsonFile
    {
        std::ofstream f("invalid.json");
        f << "{ invalid json }";
        f.close();
    }

    json.Open("invalid.json", JsonCpp::FileAccess::R);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::InvalidJsonFile));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::InvalidJsonFile), "InvalidJsonFile");
    json.ClearObject();
    remove("invalid.json");

    // FileNotFound
    json.Open("definitely_not_exist_file.json", JsonCpp::FileAccess::R);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::FileNotFound));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::FileNotFound), "FileNotFound");
    json.ClearObject();

    // SavePathIsEmpty

    json.Open("", JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("level1");
    json.Key("level1").AddObject("level2");
    json.Key("level1").Key("level2").AddValue("key", "value");
    json.Save();

    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::SavePathIsEmpty));
    json.ClearErrorState();
    json.Save(test_file_path.c_str());
    EXPECT_FALSE(json.HasErrorCode());
    json.ClearObject();

	// ErrorCode::ObjectIsEmpty
    (void)json.GetValue<int>("any");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.AddValue("key", 34);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.AddObject("root");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.AddArray("array", std::vector<int>{1, 2, 3}.data(), 3);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

	(void)json.AddObjectsArray("objects", 2);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    bool boolArr[] = { true, false, true };
    (void)json.ChangeArray("array", boolArr, 3);
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

	(void)json.ChangeValue("key", "value");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.GetChildObjectCount();
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.GetSizeArray("root");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.RemoveKey("key");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.GetStringDataBuffer();
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");
    json.ClearErrorState();

    (void)json.Save();
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::ObjectIsEmpty));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::ObjectIsEmpty), "ObjectIsEmpty");

    json.ClearObject();

    // KeyNotFound
    json.Open("", JsonCpp::FileAccess::VIRTUAL);
	json.AddObject("level1");
	json.Key("level1").AddObject("level2");
	json.Key("level1").Key("level2").AddValue("key", "value");
    
    (void)json.Key("level1").Key("level5").GetValue<std::string>("key");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    json.ClearErrorState();

    // KeyNotFound
    json.SetSringDataBuffer("{\"foo\":123}");
    (void)json.GetValue<int>("bar");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotFound));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::KeyNotFound), "KeyNotFound");
    json.ClearErrorState();

    // KeyNotObject
    json.SetSringDataBuffer("{\"foo\":[1,2,3]}");
    json.Key("foo").GetValue<int>("foo");
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotObject));
    json.ClearErrorState();

    // KeyNotArray
    json.SetSringDataBuffer("{\"foo\":123}");
    json.Key("foo", 0); // foo is not Array.
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyNotArray));
    json.ClearErrorState();

    // KeyTypeMismatch
    json.SetSringDataBuffer("{\"foo\":123}");
    (void)json.GetValue<std::string>("foo"); // int -> string
    EXPECT_EQ(json.GetErrorCode(), static_cast<int>(JsonCpp::ErrorCode::KeyTypeMismatch));
    EXPECT_EQ(json.GetErrorCodeStr(JsonCpp::ErrorCode::KeyTypeMismatch), "KeyTypeMismatch");
    json.ClearErrorState();

    //// SaveError
    //// Уже проверено выше через Save() без пути

    //// AccessError  
    //// Нет прямого публичного способа вызвать, обычно это ошибка доступа к файлу

}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}