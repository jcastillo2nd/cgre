AM_CPPFLAGS = -I$(top_srcdir)/include

LDADD = $(top_builddir)/src/libcgre.la

TESTS = cgre_hash_list_delete_tests \
	cgre_hash_list_insert_tests \
	cgre_hash_list_replace_tests \
	cgre_hash_list_search_tests

check_PROGRAMS = cgre_hash_list_delete_tests \
		 cgre_hash_list_insert_tests \
		 cgre_hash_list_replace_tests \
		 cgre_hash_list_search_tests

cgre_hash_list_delete_tests_SOURCES = cgre_hash_list_delete_tests.c

cgre_hash_list_insert_tests_SOURCES = cgre_hash_list_insert_tests.c

cgre_hash_list_replace_tests_SOURCES = cgre_hash_list_replace_tests.c

cgre_hash_list_search_tests_SOURCES = cgre_hash_list_search_tests.c
