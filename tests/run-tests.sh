#!/usr/bin/env bash

set -o nounset
shopt -s nullglob
shopt -s globstar

# command-line arguments
test_target=$1
tests_dir=$2

# ANSI escape strings
c_red='\e[31m'
c_green='\e[32m'
c_bold='\e[1m'
c_clear='\e[0m'

# files used to capture output
stdout=$(mktemp)
stderr=$(mktemp)

# the time to run a test before killing it
timeout_after=0.5

# helper for comparing stdout/stderr data
function compare_output() {
    test_name="$1"
    out_name="$2"
    output_file="$3"
    expected_file="$4"

    diff_out="$(diff --unified=2 --new-file "$expected_file" "$output_file")"
    if [ $? -ne 0 ]; then
        echo -e "${c_red}[Fail]${c_clear} wrong $out_name for $test_name"
        echo "$diff_out"

        return 1
    fi

    return 0
}

# keep track of executed and passed tests
test_count=0
pass_count=0

# Iterate over every *.fish, executing with input from *.stdin (if it exists).
# The standard output and error of the script are then compared to *.stdout
# and *.stderr, and if differences are found, that script fails the test.
for script in "$tests_dir"/**/*.fish
do
    (( test_count++ ))

    base=${script%.*}
    name=$(basename "$base")
    stdin="$base.stdin"
    expected_stdout="$base.stdout"
    expected_stderr="$base.stderr"

    echo "Testing $name..."

    if [ -f $stdin ]; then
        timeout $timeout_after $test_target $script <$stdin >$stdout 2>$stderr
    else
        timeout $timeout_after $test_target $script </dev/null >$stdout 2>$stderr
    fi

    echo -ne "\e[F\e[2K"

    if [ $? -eq 124 ]; then
        echo -e "${c_red}[Fail]${c_clear} timeout for $name"
        continue
    fi

    fail=no
    compare_output "$name" stdout "$stdout" "$expected_stdout" || fail=yes
    compare_output "$name" stderr "$stderr" "$expected_stderr" || fail=yes

    if [ "$fail" = no ]; then
        (( pass_count++ ))
        echo -e "${c_green}[Pass]${c_clear} $name"
    fi
done

rm $stdout $stderr

if [ $test_count = $pass_count ]; then
    echo -e "${c_bold}${c_green}${pass_count}/${test_count} tests passed${c_clear}"
    exit 0
else
    echo -e "${c_bold}${c_red}${pass_count}/${test_count} tests passed${c_clear}"
    exit 1
fi
