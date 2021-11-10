cmake_minimum_required (VERSION 3.20.0)
project (GLEEMAIL_CLI)

set(CMAKE_BUILD_TYPE Debug)

add_executable (glEEmail-cli src/main.cpp)

if(WIN32)
	target_link_libraries(glEEmail-cli wsock32 ws2_32)
else()
	target_link_libraries(glEEmail-cli pthread)
endif()