SUBDIRS = cgre_array  \
	  cgre_hash_list  \
	  cgre_queue \
	  cgre_stack \
	  cgre_tree

AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_node_tests

check_PROGRAMS = cgre_node_tests

cgre_node_tests_SOURCES = cgre_node_tests.c
