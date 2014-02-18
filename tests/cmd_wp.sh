#!/bin/sh

CMD_NAME="wp"
CMD=$'wp 0x1000 0x41\x0a wp 0x1001 0x42\x0a wp 0x1002 0x43\x0a dp 0x1000 16'
TEST_OUTPUT="1000  41 42 43 00 00 00 00 00 00 00 00 00 00 00 00 00  ABC............."

. $srcdir/cmd.sh
