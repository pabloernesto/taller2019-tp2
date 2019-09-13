#! /bin/bash

./tp
ret=$?

if [ $ret -ne 1 ]; then
  fail "got a return of $ret"
fi

pass
