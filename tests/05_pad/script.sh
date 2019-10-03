# 05_pad
./tp 8 1 1 $folder/pad $folder/pad8.out
ret=$?

if [ $ret -ne 0 ]; then
  fail "got a return of $ret"
fi

delta=`diff -q $folder/pad8.out $folder/expected_output`
if [ "$delta" != "" ]; then
  fail "unexpected output"
fi

pass
