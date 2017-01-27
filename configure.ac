AC_INIT([pulsar], [0.0.1], [cyrille.github@youplaboum.fr])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
				 Makefile
				 src/Makefile
				 ])
AC_CONFIG_MACRO_DIR([m4])
PKG_CHECK_MODULES([PULSE], [libpulse >= 5],
				  [AC_SUBST(PULSE_CFLAGS) AC_SUBST(PULSE_LIBS)])

AX_BOOST_BASE
AX_BOOST_PROGRAM_OPTIONS

AC_OUTPUT
