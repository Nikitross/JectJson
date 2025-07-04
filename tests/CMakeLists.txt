
find_package(GTest REQUIRED)

add_executable(JectJsonTests "JectJsonUnitTests.cpp")

target_link_libraries(JectJsonTests 
    PRIVATE 
    jectjson 
    GTest::GTest 
    GTest::Main
)

add_test(NAME JectJsonUnitTests COMMAND JectJsonTests)

enable_testing()