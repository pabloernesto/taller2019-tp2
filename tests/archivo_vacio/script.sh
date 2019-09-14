#! /bin/bash

./tp 5 1 1 $folder/input $folder/output
ret=$?

if [ $ret -ne 0 ]; then
  fail "got a return of $ret"
fi

delta=`diff -q $folder/output $folder/expected_output`
if [ "$delta" != "" ]; then
  fail "got non-empty output"
fi

pass
