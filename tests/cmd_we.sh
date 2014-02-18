#!/bin/sh

CMD_NAME="we"
CMD=$'we 0x80 0x30\x0a we 0x81 0x31\x0a we 0x82 0x32\x0a de 0x80 16'
TEST_OUTPUT="0080  30 31 32 00 00 00 00 00 00 00 00 00 00 00 00 00  012............."

. $srcdir/cmd.sh
