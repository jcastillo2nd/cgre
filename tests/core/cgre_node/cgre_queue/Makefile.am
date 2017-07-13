AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_queue_push_tests \
	cgre_queue_pop_tests \
	cgre_queue_peek_tests

check_PROGRAMS = cgre_queue_push_tests \
		 cgre_queue_pop_tests \
		 cgre_queue_peek_tests

cgre_queue_push_tests_SOURCES = cgre_queue_push_tests.c

cgre_queue_pop_tests_SOURCES = cgre_queue_pop_tests.c

cgre_queue_peek_tests_SOURCES = cgre_queue_peek_tests.c
