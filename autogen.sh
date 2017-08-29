#! /bin/sh
set -e

# $Id: autogen.sh,v 1.4 2007/03/24 03:05:33 jjongsma Exp $
#
# Copyright (c) 2002  Daniel Elstner  <daniel.elstner@gmx.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License VERSION 2 as
# published by the Free Software Foundation.  You are not allowed to
# use any other version of the license; unless you got the explicit
# permission from the author to do so.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.


dir=`echo "$0" | sed 's,[^/]*$,,'`
test "x${dir}" = "x" && dir='.'

if test "x`cd "${dir}" 2>/dev/null && pwd`" != "x`pwd`"
then
    echo "This script must be executed directly from the source directory."
    exit 1
fi

LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}
AUTOCONF=${AUTOCONF:-autoconf}
AUTOMAKE=${AUTOMAKE:-automake}

#ACLOCAL_FLAGS="-I m4"
ACLOCAL=`echo $AUTOMAKE | sed s/automake/aclocal/`

rm -f config.cache acconfig.h

do_cmd() {
    echo "- $@"
    $@
}

do_cmd $ACLOCAL $ACLOCAL_FLAGS
do_cmd $LIBTOOLIZE --force
do_cmd $AUTOCONF
do_cmd $AUTOMAKE --add-missing --gnu
do_cmd ./configure "$@"

