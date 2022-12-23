cmake_minimum_required(VERSION 3.25)	
project(server)
set(SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/server/Command.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/server/LED.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/server/server.cpp)
add_executable(LEDServer ${SOURCE_FILES})
set(CMAKE_CXX_STANDARD 17)
set(BOOST_DIR F:\\cpp\\boost\\boost_1_81_0)
set(INCLUDE_DIRS ${BOOST_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server)
target_include_directories(LEDServer PRIVATE ${INCLUDE_DIRS} )