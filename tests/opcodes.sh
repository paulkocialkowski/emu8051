#!/bin/sh

# Get the symlink name (how we were called):
test_name=`basename $0`

lf=${test_name}.log

STOP_ADDRESS="0xFFF0"
XRAM_SIZE="256"

name=$(basename ${test_name} .sh)
hexfile=${name}.hex

echo "Testing ${name}.hex" > ${lf}

../src/cli/emu8051-cli -d 2 --xram=${XRAM_SIZE} -s ${STOP_ADDRESS} ${hexfile} >> ${lf}
if test $? -ne 0 ; then
    return 1
fi

test_output_found=0

while read line; do
    if echo ${line} | grep -q "; Test output"; then
        test_output_found=1

        test_str=$(echo ${line} | sed "s/^; Test output: //")

        if ! grep -q "${test_str}" ${lf}; then
            echo "Failed test: ${test_str}" >> ${lf}
            exit 1
        fi
    fi
done < $srcdir/${name}.asm

if [ x"${test_output_found}" == x0 ]; then
    # Need at least one test output condition to verify correct operation
    exit 1
fi

exit 0
