#!/bin/sh

CMD_NAME="di"
CMD=$'di 0x80 16\x0a'
TEST_OUTPUT="0080  FF 07 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................"

. $srcdir/cmd.sh
