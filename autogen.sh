#!/bin/sh
# autogen.sh -- Use this script to create generated files from the git distribution

set -e
autoreconf -vi
rm -rf autom4te.cache
