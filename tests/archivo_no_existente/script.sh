#! /bin/bash

./tp 5 1 1 $folder/input $folder/output
ret=$?

if [ $ret -ne 1 ]; then
  fail "got a return of $ret"
fi

pass
