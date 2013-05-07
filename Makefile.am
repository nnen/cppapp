lib_LIBRARIES = libcppapp.a
libcppapp_a_SOURCES = src/AppBase.cpp src/Config.cpp src/Input.cpp src/Logger.cpp src/Object.cpp src/Options.cpp src/Output.cpp src/Stopwatch.cpp src/string_utils.cpp src/utils.cpp

library_includedir=$(includedir)/cppapp-@PACKAGE_VERSION@/cppapp
library_include_HEADERS = src/AppBase.h src/Config.h src/cppapp.h src/Input.h src/Logger.h src/Object.h src/Options.h src/Output.h src/Stopwatch.h src/string_utils.h src/utils.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cppapp-@PACKAGE_VERSION@.pc

