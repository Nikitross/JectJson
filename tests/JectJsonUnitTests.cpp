
#include <gtest/gtest.h>
#include <jectjson.h>

#include <filesystem>
#include <fstream>

static void ErrHandl(const std::string& modul_name, const std::string& message, const std::string& details) {
    std::cout << modul_name << message << " " << details << std::endl;
}

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

TEST(JsonCppTest, BasicTest) {
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("root");
    json.Key("root").AddValue("key", "value");

    EXPECT_EQ(json.Key("root").GetValue<std::string>("key"), "value");
}

TEST(JsonCppTest, CreateAndReadBasicJson) {
    
    JsonCpp json;
    json.RegisterCallback_onError(&ErrHandl);

    ASSERT_TRUE(json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL));
    json.AddObject("root");
    json.Key("root").AddValue("name", "Test");
    json.Key("root").AddValue("value", 42);
     
    EXPECT_EQ(json.Key("root").GetValue<std::string>("name"), "Test");
    EXPECT_EQ(json.Key("root").GetValue<int>("value"), 42);

    const std::string tmpFile = "test_temp.json";
    ASSERT_TRUE(json.Save(tmpFile.c_str()));

    JsonCpp jsonLoad;
    ASSERT_TRUE(jsonLoad.Open(tmpFile, JsonCpp::FileAccess::RW));
    EXPECT_EQ(jsonLoad.Key("root").GetValue<std::string>("name"), "Test");
    remove(tmpFile.c_str());
}

TEST(JsonCppTest, NestedObjects) {
    
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);

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

TEST(JsonCppTest, ArrayOperations) {
    
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);

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

TEST(JsonCppTest, ErrorHandling) {
    JsonCpp json;

    bool errorCalled  = false;
    auto errorHandler = [&errorCalled](const std::string&, const std::string&, const std::string&) {
        errorCalled = true;
    };

    json.RegisterCallback_onError(errorHandler);

    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("test");

    json.Key("test").GetValue<int>("invalid_key");
    EXPECT_TRUE(errorCalled);

    errorCalled = false;
    EXPECT_FALSE(json.Open("nonexistent.json", JsonCpp::FileAccess::RW));
    EXPECT_TRUE(errorCalled);
}

TEST(JsonCppTest, StringBufferOperations) {
    
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);

    json.AddObject("data");
    json.Key("data").AddValue("key", "value");

    std::string buffer = json.GetStringDataBuffer();
    EXPECT_FALSE(buffer.empty());

    JsonCpp json2;   
    json2.SetSringDataBuffer(buffer.c_str());
    EXPECT_EQ(json2.Key("data").GetValue<std::string>("key"), "value");
}

TEST(JsonCppTest, DataModification) {
    
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);

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

TEST(JsonCppTest, FileOperations) {
    
    const std::string testFile = "test_operations.json";

    {
        JsonCpp json;
        json.Open(testFile, JsonCpp::FileAccess::RWA);
        json.AddObject("data");
        json.Key("data").AddValue("created", true);
        ASSERT_TRUE(json.Save());
    }

    {
        JsonCpp json;
        ASSERT_TRUE(json.Open(testFile, JsonCpp::FileAccess::RW));
        EXPECT_TRUE(json.Key("data").GetValue<bool>("created"));
    }

    remove(testFile.c_str());
}

TEST(JsonCppTest, SpecialCases) {
   
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    EXPECT_NO_THROW(json.PrintIt());

    json.AddObject("root");
    json.Key("root").AddObjectsArray("items", 2);
    json.Key("root").Key("items", 0).AddValue("id", 1);
    json.Key("root").Key("items", 1).AddValue("id", 2);
    EXPECT_EQ(json.Key("root").GetSizeArray("items"), 2);

    json.Key("root").AddValue("flag", false);
    EXPECT_FALSE(json.Key("root").GetValue<bool>("flag"));
}

TEST(JsonCppTest, OpenJsonFile) {

    remove("unit_test.json");
    JsonCpp json;

    bool res = json.Open("unit_test.json", JsonCpp::FileAccess::R);
    ASSERT_FALSE(res);

    res = json.Open("unit_test.json", JsonCpp::FileAccess::RW);
    ASSERT_FALSE(res);

    res = json.Open("unit_test.json", JsonCpp::FileAccess::RWA);
    json.AddObject("data");
    json.Key("data").AddValue("created", true);
    ASSERT_TRUE(json.Save());
    ASSERT_TRUE(res);

    res = json.Open("unit_test.json", JsonCpp::FileAccess::R);
    ASSERT_TRUE(res);

    res = json.Open("unit_test.json", JsonCpp::FileAccess::RW);
    ASSERT_TRUE(res);

    remove("unit_test.json");
    res = json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    ASSERT_TRUE(json.Save());
    ASSERT_TRUE(res);

    remove("unit_test.json");
}

TEST(JsonParserTest, OpenWithReadAccessOnExistingFile) {

    const std::string TEST_FILE = "test_file.json";
    std::ofstream out(TEST_FILE);
    out << "{\"key\":\"value\"}";
    out.close();

    JsonCpp json;
    json.RegisterCallback_onError([](const char* msg, const char* details) {
        FAIL() << "Error callback should not be called";
    });

    EXPECT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::R));
}

TEST(JsonParserTest, OpenWithReadAccessOnNonExistingFile) {
   
    JsonCpp json;
    bool callbackCalled = false;

    json.RegisterCallback_onError([&](const char* msg, const char* details) {
        callbackCalled = true;
    });

    EXPECT_FALSE(json.Open("nonexistent.json", JsonCpp::FileAccess::R));
    EXPECT_TRUE(callbackCalled);
}

TEST(JsonParserTest, OpenWithRWAAccessCreatesNewFile) {
   
    JsonCpp json;
    const std::string TEST_FILE = "test_file.json";
    json.RegisterCallback_onError([](const char* msg, const char* details) {
        FAIL() << "Error callback should not be called";
    });

    EXPECT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWA));
    EXPECT_TRUE(fileExists(TEST_FILE));
}

TEST(JsonParserTest, VirtualFileOperations) {

    JsonCpp json;
    const std::string TEMP_FILE = "temp_file.json";
    json.RegisterCallback_onError([](const char* msg, const char* details) {
        FAIL() << "Error callback should not be called";
    });

    EXPECT_TRUE(json.Open("", JsonCpp::FileAccess::VIRTUAL));

    json.AddObject("root");
    json.Key("root").AddValue("test", 123);

    EXPECT_EQ(json.Key("root").GetValue<int>("test"), 123);

    EXPECT_TRUE(json.Save(TEMP_FILE.c_str()));
    EXPECT_TRUE(fileContains(TEMP_FILE, "test"));
}

TEST(JsonParserTest, AddObjectAndKeyOperations) {

    JsonCpp json;
    const std::string TEST_FILE = "test_file.json";
    ASSERT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWA));

    json.AddObject("root");
    json.Key("root").AddObject("nested");

    json.Key("root").Key("nested").AddValue("int_val", 42);
    json.Key("root").Key("nested").AddValue("str_val", "hello");
    json.Key("root").Key("nested").AddValue("bool_val", true);

    EXPECT_EQ(json.Key("root").Key("nested").GetValue<int>("int_val"), 42);
    EXPECT_EQ(json.Key("root").Key("nested").GetValue<std::string>("str_val"), "hello");
    EXPECT_EQ(json.Key("root").Key("nested").GetValue<bool>("bool_val"), true);

    bool errorCalled = false;
    json.RegisterCallback_onError([&](const char* msg, const char* details) {
        errorCalled = true;
    });

    json.Key("root").Key("nonexistent");
    EXPECT_TRUE(errorCalled);
}

TEST(JsonParserTest, ArrayOperations) {
   
    JsonCpp json;
    const std::string TEST_FILE = "test_file.json";
    ASSERT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWA));

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
    json.RegisterCallback_onError([&](const char* msg, const char* details) {
        errorCalled = true;
    });

    json.Key("root").Key("arrays").GetValue<int>("nonexistent_array", 0);
    EXPECT_TRUE(errorCalled);
}

TEST(JsonParserTest, ModifyOperations) {
    
    JsonCpp json;
    const std::string TEST_FILE = "test_file.json";
    ASSERT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWA));

    json.AddObject("root");
    json.Key("root").AddValue("value", "old");
    json.Key("root").AddArray("array", std::vector<int>{1, 2, 3}.data(), 3);

    json.Key("root").ChangeValue("value", "new");
    EXPECT_EQ(json.Key("root").GetValue<std::string>("value"), "new");

    int newArr[] = { 4, 5, 6 };
    json.Key("root").ChangeArray("array", newArr, 3);
    EXPECT_EQ(json.Key("root").GetValue<int>("array", 0), 4);

    bool errorCalled = false;
    json.RegisterCallback_onError([&](const char* msg, const char* details) {
        errorCalled = true;
    });

    json.Key("root").ChangeValue("nonexistent", 123);
    EXPECT_TRUE(errorCalled);

    errorCalled = false;
    json.Key("root").ChangeArray("nonexistent", newArr, 3);
    EXPECT_TRUE(errorCalled);
}

TEST(JsonParserTest, StringBufferOperations) {

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

TEST(JsonParserTest, ErrorHandling) {
    
    JsonCpp json;
    const std::string TEST_FILE = "test_file.json";
    std::string lastError;
    json.RegisterCallback_onError([&](const char* msg, const char* details) {
        lastError = std::string(msg) + " " + std::string(details);
    });

    json.Open("nonexistent.json", JsonCpp::FileAccess::R);
    EXPECT_NE(lastError.find("nonexistent.json"), std::string::npos);

    lastError.clear();
    ASSERT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWA));
    json.Key("nonexistent_key");
    EXPECT_NE(lastError.find("nonexistent_key"), std::string::npos);
}

TEST(JsonParserTest, RWASModeRecreatesFile) {

    const std::string TEST_FILE = "test_file.json";

    {
        std::ofstream out(TEST_FILE);
        out << "{\"old\":\"data\"}";
        out.close();
    }

    JsonCpp json;
    ASSERT_TRUE(json.Open(TEST_FILE, JsonCpp::FileAccess::RWAS));

    json.AddObject("root");
    json.Key("root").AddValue("new", "data");
    json.Save();

    std::ifstream in(TEST_FILE);
    std::string content((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());
    
    EXPECT_EQ(content.find("old"), std::string::npos);
    EXPECT_NE(content.find("new"), std::string::npos);
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}