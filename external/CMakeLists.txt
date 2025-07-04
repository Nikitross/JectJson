
if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/external/rapidjson")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/external/rapidjson")
else()
    
    include(FetchContent)

    FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG master 
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/rapidjson
    )

    FetchContent_GetProperties(rapidjson)
    
    if(NOT rapidjson_POPULATED)
        FetchContent_Populate(rapidjson)
    endif()

endif()

add_library(rapidjson INTERFACE)

target_include_directories(rapidjson INTERFACE
     ${CMAKE_SOURCE_DIR}/external/rapidjson/include
)