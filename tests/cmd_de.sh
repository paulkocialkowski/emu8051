#!/bin/sh

CMD_NAME="de"
CMD=$'de 0x00 16\x0a'
TEST_OUTPUT="0000  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................"

. $srcdir/cmd.sh
