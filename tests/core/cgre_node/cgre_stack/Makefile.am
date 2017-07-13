AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_stack_push_tests \
	cgre_stack_pop_tests \
	cgre_stack_peek_tests

check_PROGRAMS = cgre_stack_push_tests \
		 cgre_stack_pop_tests \
		 cgre_stack_peek_tests

cgre_stack_push_tests_SOURCES = cgre_stack_push_tests.c

cgre_stack_pop_tests_SOURCES = cgre_stack_pop_tests.c

cgre_stack_peek_tests_SOURCES = cgre_stack_peek_tests.c
