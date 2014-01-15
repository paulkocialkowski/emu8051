#!/bin/sh
# autogen.sh -- Use this script to create generated files from the git distribution

# ChangeLog is generated from git log output using "make changelog"
touch ChangeLog

set -e
autoreconf -vi
rm -rf autom4te.cache
