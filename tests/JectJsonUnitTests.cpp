
#include <gtest/gtest.h>
#include <jectjson.h>

static void ErrHandl2(const std::string& modul_name, const std::string& message, const std::string& details) {
    std::cout << modul_name << message << " " << details << std::endl;
}

TEST(JsonParserTest, BasicTest) {
    JsonCpp json;
    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("root");
    json.Key("root").AddValue("key", "value");

    EXPECT_EQ(json.Key("root").GetValue<std::string>("key"), "value");
}

TEST(JsonCppTest, CreateAndReadBasicJson) {
    JsonCpp json;
    json.RegisterCallback_onError(&ErrHandl2);

    ASSERT_TRUE(json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL));
    json.AddObject("root1");
    json.Key("root1").AddValue("name", "Test");
    json.Key("root1").AddValue("value", 42);
     
    EXPECT_EQ(json.Key("root1").GetValue<std::string>("name"), "Test");
    EXPECT_EQ(json.Key("root1").GetValue<int>("value"), 42);

    const std::string tmpFile = "test_temp.json";
    ASSERT_TRUE(json.Save(tmpFile.c_str()));

    JsonCpp jsonLoad;
    ASSERT_TRUE(jsonLoad.Open(tmpFile, JsonCpp::FileAccess::RW));
    EXPECT_EQ(jsonLoad.Key("root1").GetValue<std::string>("name"), "Test");
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
    bool errorCalled = false;

    auto errorHandler = [&errorCalled](const std::string&, const std::string&, const std::string&) {
        errorCalled = true;
    };

    json.RegisterCallback_onError(errorHandler);

    json.Open("unit_test.json", JsonCpp::FileAccess::VIRTUAL);
    json.AddObject("test");

    json.Key("test").GetValue<int>("invalid_key");
    //EXPECT_THROW(json.Key("test").GetValue<int>("invalid_key"), std::runtime_error);
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



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}