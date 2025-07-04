file(GLOB EXAMPLE_SOURCES *.cpp)

foreach(example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    
    if(TARGET jectjson)
        target_link_libraries(${example_name} PRIVATE jectjson)
    endif()
endforeach()