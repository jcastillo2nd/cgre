ACLOCAL_AMFLAGS = -I config

SUBDIRS = src tests doc

EXTRA_DIST = include tests autogen.sh

.PHONY: list

list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | xargs

.PHONY: docs

docs:
	cd doc && make docs
