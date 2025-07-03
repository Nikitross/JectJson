
add_library(jectjson STATIC src/jectjson.cpp)


target_include_directories(jectjson 
    PUBLIC 
	    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(jectjson PUBLIC rapidjson)

