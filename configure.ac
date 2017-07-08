AC_PREREQ([2.69])
AC_INIT(cgre, 0.1)

AC_LANG([C])

AC_CONFIG_MACRO_DIRS([config])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/cgre.c])
AC_CONFIG_HEADERS([src/config.h])

AC_PROG_LIBTOOL
AC_PROG_CC

AC_TYPE_LONG_DOUBLE
AC_TYPE_LONG_LONG_INT
AC_PROG_CC_C99

LT_INIT

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Options
# TODO --with-max-tree-height=N := CGRE_RBTREE_MAX_HEIGHT default 18
AC_SUBST([RBTREE_MAX_HEIGHT], [18])

# Convenience defines
AC_DEFINE_UNQUOTED([CGRE_RBTREE_MAX_HEIGHT], $RBTREE_MAX_HEIGHT, [Node rebalance interval])
AC_SUBST([START_YEAR], [2016])
AC_DEFINE_UNQUOTED([START_YEAR], $START_YEAR, [Year of project inception])
AC_SUBST([BUILD_YEAR], `date +%Y`)
AC_DEFINE_UNQUOTED([BUILD_YEAR], $BUILD_YEAR, [Year of build])
AC_SUBST([RELEASE_BUILD], `date +%Y%m%d`)
AC_DEFINE_UNQUOTED([BUILD_RELEASE], $BUILD_RELEASE, [Date of build for release])

# Doxygen Check
AC_PATH_PROG([DOXYGEN], [doxygen], [no])
AM_CONDITIONAL([HAS_DOXYGEN], [test "x$DOXYGEN" != xno])

# math
AC_SEARCH_LIBS([atan2l], [m], [],
               [AC_MSG_FAILURE([Math library missing])])

# pthread
AC_SEARCH_LIBS([pthread_create], [pthread], [],
               [AC_MSG_FAILURE([Threads library missing])])

# Program Source
AC_CONFIG_FILES([Makefile src/Makefile tests/speed/Makefile])

# Program Tests
AC_CONFIG_FILES([tests/Makefile tests/cgre/Makefile tests/math/Makefile tests/core/Makefile])

# Program Docs
AC_CONFIG_FILES([doc/Makefile doc/Doxyfile doc/src/man/cgre-clockperf.man])

AC_OUTPUT
