dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(YandexFotkiSaver, 1.0)

dnl AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_LANG([C++])

PKG_CHECK_MODULES([MAGICKPP], [Magick++ >= 6.6.0])
PKG_CHECK_MODULES([XML], [libxml-2.0 >= 2.7.0])
PKG_CHECK_MODULES([GL], [gl >= 7.10.0])
PKG_CHECK_MODULES([GLEW], [glew >= 1.5.0])
PKG_CHECK_MODULES([CURL], [libcurl >= 7.21.0])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
