manbuild = $(top_srcdir)/doc/src/man

dist_man1_MANS = $(manbuild)/cgre-clockperf.man

$(manbuild)/cgre-clockperf.man: cgre-clockperf.stamp

cgre-clockperf.stamp:
	echo Timestamp > cgre-clockperf.stamp

CLEANFILES = cgre-clockperf.stamp

all-local: cgre-clockperf.stamp
clean-local:
	rm -rf cgre-clockperf.stamp

.PHONY: docs

docs:
	$(DOXYGEN)
