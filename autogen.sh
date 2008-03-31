#!/bin/sh
touch README
rm -f config.cache
echo "- aclocal."
aclocal -I m4
echo "- autoconf."
autoconf
echo "- autoheader."
autoheader
echo "- automake."
automake -a
echo "- configure"
if [ -z "$NOCONFIGURE" ]; then
	./configure "$@"
	./configure "$@"
fi
exit
