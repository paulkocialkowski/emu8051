#!/bin/sh

CMD_NAME="wr"
CMD=$'wr pc 0x5555 \x0a dr \x0a'
TEST_OUTPUT="| 5555 | 07 |"

. $srcdir/cmd.sh
