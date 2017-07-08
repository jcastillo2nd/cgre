#!/bin/sh
libtoolize -iqf
aclocal -I config --install
automake --foreign --add-missing --copy --force-missing
autoconf -f

