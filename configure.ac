# AC_PREREQ([2.68])
AC_INIT([cppapp], [0.2.1dev], [milikjan@fit.cvut.cz])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB(pthread, pthread_create, [], [ echo "ERROR!: pthread_create in libpthread not found!"; exit -1], [])
AC_CHECK_LIB(pthread, pthread_mutex_init, [], [ echo "ERROR!: pthread_mutex_init in libpthread not found!"; exit -1], [])
AC_CHECK_LIB(pthread, pthread_cond_init, [], [ echo "ERROR!: pthread_cond_init in libpthread not found!"; exit -1], [])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([gettimeofday strerror strtol])

AC_CONFIG_FILES([Makefile cppapp-0.2.1.pc])
AC_OUTPUT

