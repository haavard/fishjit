#!/usr/bin/env bash

set -o nounset

target=$1
testdir=$2

c_red='\e[31m'
c_green='\e[32m'
bold='\e[1m'
c_clear='\e[0m'

stdout=$(mktemp)
stderr=$(mktemp)
empty=$(mktemp)

test_count=0
pass_count=0

# Iterate over every *.fish, executing with input from *.stdin (if it exists).
# The standard output and error of the script are then compared to *.stdout
# and *.stderr, and if differences are found, that script fails the test.

for script in $testdir/*.fish; do
    (( test_count++ ))

    base=${script%.*}
    name=$(basename $base)
    stdin=$base.stdin
    expected_stdout=$base.stdout
    expected_stderr=$base.stderr

    echo "Testing $name..."

    if [ -f $stdin ]; then
        $target $script <$stdin >$stdout 2>$stderr
    else
        $target $script >$stdout 2>$stderr
    fi

    # if there's are no files, assume they should be empty
    if ! [ -f $expected_stdout ]; then
        expected_stdout=$empty
    fi
    if ! [ -f $expected_stderr ]; then
        expected_stderr=$empty
    fi

    fail=no

    cmp $stdout $expected_stdout &>/dev/null
    if [ $? -ne 0 ]; then
        echo -ne "\033[F\033[2K"

        echo -e "${c_red}[Fail]${c_clear} wrong stdout for $name"
        echo "--- Expected: ---"
        cat $expected_stdout
        echo "--- Actual: -----"
        cat $stdout
        echo "-----------------"

        fail=yes
    fi

    cmp $stderr $expected_stderr &>/dev/null
    if [ $? -ne 0 ]; then
        if ! [ "$fail" = yes ]; then
            echo -ne "\033[F\033[2K"
        fi

        echo -e "${c_red}[Fail]${c_clear} wrong stderr for $name"
        echo "--- Expected: ---"
        cat $expected_stderr
        echo "--- Actual: -----"
        cat $stderr
        echo "-----------------"

        fail=yes
    fi

    if ! [ "$fail" = yes ]; then
        (( pass_count++ ))
        echo -ne "\033[F\033[2K" # Back up and erase one line
        echo -e "${c_green}[Pass]${c_clear} $name"
    fi
done

rm $stdout $stderr $empty

if [ $test_count = $pass_count ]; then
    echo -e "${bold}${c_green}${pass_count}/${test_count} tests passed${c_clear}"
    exit 0
else
    echo -e "${bold}${c_red}${pass_count}/${test_count} tests passed${c_clear}"
    exit 1
fi
