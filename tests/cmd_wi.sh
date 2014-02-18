#!/bin/sh

CMD_NAME="wi"
CMD=$'wi 0x40 0x30\x0a wi 0x41 0x31\x0a wi 0x42 0x32\x0a di 0x40 16'
TEST_OUTPUT="0040  30 31 32 00 00 00 00 00 00 00 00 00 00 00 00 00  012............."

. $srcdir/cmd.sh
