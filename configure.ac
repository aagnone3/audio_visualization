# configure.ac
AC_INIT([audio_visualization], [0.1], [anthonyagnone@gmail.com])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/main.cpp])
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# tell `autoconf` about any depdencies the configure script itself has.
# ex: C compiler --> AC_PROG_CC
# ex: program on ${PATH} --> AC_PATH_PROG
# http://www.gnu.org/software/autoconf/manual
AC_PROG_CC
AC_PROG_CXX

# tell `autoconf` to use the Makefile.am template to make a Makefile
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([bzero floor gettimeofday pow])

# output the script
AC_OUTPUT
