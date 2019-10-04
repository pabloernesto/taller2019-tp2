# 06_zero_block
./tp 3 1 1 $folder/zeroblk $folder/zeroblk3.out
ret=$?

if [ $ret -ne 0 ]; then
  fail "got a return of $ret"
fi

delta=`diff -q $folder/zeroblk3.out $folder/expected_output`
if [ "$delta" != "" ]; then
  fail "unexpected output"
fi

pass
