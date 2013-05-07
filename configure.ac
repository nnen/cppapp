# AC_PREREQ([2.68])
AC_INIT([cppapp], [0.2.1], [milikjan@fit.cvut.cz])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for header files.
AC_CHECK_HEADERS([stdint.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([gettimeofday strtol])

AC_CONFIG_FILES([Makefile cppapp-0.2.1.pc])
AC_OUTPUT

