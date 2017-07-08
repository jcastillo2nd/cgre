AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS = cgre_hash_tests cgre_node_tests cgre_tree_tests

LDADD = $(top_builddir)/src/libcgre.la

check_PROGRAMS = cgre_hash_tests \
		 cgre_node_tests \
		 cgre_tree_tests

cgre_hash_tests_SOURCES = cgre_hash_tests.c

cgre_node_tests_SOURCES = cgre_node_tests.c

cgre_tree_tests_SOURCES = cgre_tree_tests.c
