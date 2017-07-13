AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_tree_delete_tests \
	cgre_tree_insert_tests \
	cgre_tree_replace_tests \
	cgre_tree_search_tests

check_PROGRAMS = cgre_tree_delete_tests \
		 cgre_tree_insert_tests \
		 cgre_tree_replace_tests \
		 cgre_tree_search_tests

cgre_tree_delete_tests_SOURCES = cgre_tree_delete_tests.c

cgre_tree_insert_tests_SOURCES = cgre_tree_insert_tests.c

cgre_tree_replace_tests_SOURCES = cgre_tree_replace_tests.c

cgre_tree_search_tests_SOURCES = cgre_tree_search_tests.c
