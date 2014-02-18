#!/bin/sh

CMD_NAME="dp"
CMD=$'dp 0x00 16\x0a'
TEST_OUTPUT="0000  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................"

. $srcdir/cmd.sh
