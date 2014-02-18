#!/bin/sh

# Get the symlink name (how we were called):
test_name=`basename $0`

lf=${test_name}.log

name=$(basename ${test_name} .sh)

echo "Testing command ${CMD_NAME}" > ${lf}

echo "${CMD}" | ../src/cli/emu8051-cli -d 2 >> ${lf}
if test $? -ne 0 ; then
    return 1
fi

# Verify syntax errors
if grep -q "syntax" ${lf}; then
    echo "Failed test: ${CMD_NAME} (syntax error)" >> ${lf}
    exit 1
fi

# Verify for valid test values
if ! grep -q "${TEST_OUTPUT}" ${lf}; then
    echo "Failed test: ${CMD_NAME} (values not found)" >> ${lf}
    exit 1
fi

exit 0
