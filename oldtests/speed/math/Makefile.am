AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS = cgre_real_clamp \
	cgre_vec2_angle_between \
	cgre_vec2_oriented_angle_between

LDADD = $(top_builddir)/src/libcgre.la

check_PROGRAMS = cgre_real_clamp \
		 cgre_vec2_angle_between \
		 cgre_vec2_oriented_angle_between

cgre_real_clamp_SOURCES = cgre_real_clamp.c

cgre_vec2_angle_between_SOURCES = cgre_vec2_angle_between.c

cgre_vec2_oriented_angle_between_SOURCES = cgre_vec2_oriented_angle_between.c
