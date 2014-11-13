cmake_minimum_required (VERSION 2.6)

project (cppapp)

set(cppapp_VERSION 0.2.2dev)
set(cppapp_VERSION_MAJOR 0)
set(cppapp_VERSION_MINOR 2)

configure_file (
	"${PROJECT_SOURCE_DIR}/cppapp_config.h.in"
	"${PROJECT_BINARY_DIR}/cppapp_config.h"
)
configure_file(
	"${PROJECT_SOURCE_DIR}/cppapp.pc.in"
	"${PROJECT_BINARY_DIR}/cppapp.in"
)

file(GLOB CPPAPP_CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cppapp/*.cpp)
file(GLOB CPPAPP_H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cppapp/*.h)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cppapp")

#set(CMAKE_CXX_COMPILER    "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS_INIT  "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

add_library(cppapp STATIC ${CPPAPP_CPP_FILES})

install(TARGETS cppapp DESTINATION "lib")
install(FILES ${CPPAPP_H_FILES} DESTINATION "include/cppapp")
install(FILES ${PROJECT_BINARY_DIR}/cppapp_config.h DESTINATION "include/cppapp")

set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "milikjan@fit.cvut.cz")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2dev")
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README.md)
include(CPack)

