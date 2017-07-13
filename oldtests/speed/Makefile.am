AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/tests/speed

LDADD = $(top_builddir)/src/libcgre.la

bin_PROGRAMS = cgre-clockperf

cgre_clockperf_SOURCES = cgre-clockperf.c \
			 math/cgre_base.c \
			 math/cgre_real_clamp.c \
			 math/cgre_vec2_angle_between.c \
			 math/cgre_vec2_oriented_angle_between.c \
			 core/cgre_tree_insert.c
