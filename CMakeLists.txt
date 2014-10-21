cmake_minimum_required (VERSION 2.6)

project (cppapp)

# include_directories("src")

#set(CMAKE_CXX_COMPILER    "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS_INIT  "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gtest-1.7.0)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest-1.7.0/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cppapp)

#add_executable(littlevm
#	src/main.cpp
#	)

add_library(cppapp STATIC 
	cppapp/AppBase.cpp
	cppapp/Channel.cpp
	cppapp/Config.cpp
	cppapp/Debug.cpp
	cppapp/DynObject.cpp
	cppapp/Exception.cpp
	cppapp/Injector.cpp
	cppapp/Input.cpp
	cppapp/json.cpp
	cppapp/Lexer.cpp
	cppapp/Logger.cpp
	cppapp/Mutex.cpp
	cppapp/Object.cpp
	cppapp/Options.cpp
	cppapp/Output.cpp
	cppapp/Path.cpp
	cppapp/PrettyPrinter.cpp
	cppapp/Stopwatch.cpp
	cppapp/string_utils.cpp
	cppapp/TestApp.cpp
	cppapp/Test.cpp
	cppapp/TextLoc.cpp
	cppapp/Thread.cpp
	cppapp/Time.cpp
	cppapp/utils.cpp
	)

#target_link_libraries(littlevm littlelang ${CMAKE_DL_LIBS})
#target_link_libraries(littlelang ${CMAKE_DL_LIBS})
#target_link_libraries(tests littlelang gtest gtest_main)

