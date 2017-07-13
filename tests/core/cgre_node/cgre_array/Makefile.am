AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_array_add_tests \
	cgre_array_delete_tests \
	cgre_array_get_tests \
	cgre_array_set_tests

check_PROGRAMS = cgre_array_add_tests \
		 cgre_array_delete_tests \
		 cgre_array_get_tests \
		 cgre_array_set_tests

cgre_array_add_tests_SOURCES = cgre_array_add_tests.c

cgre_array_delete_tests_SOURCES = cgre_array_delete_tests.c

cgre_array_get_tests_SOURCES = cgre_array_get_tests.c

cgre_array_set_tests_SOURCES = cgre_array_set_tests.c
