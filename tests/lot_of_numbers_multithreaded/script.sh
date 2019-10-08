# lot_of_numbers_multithreaded
./tp 4 2 1 $folder/alot $folder/alot4.out
ret=$?

if [ $ret -ne 0 ]; then
  fail "got a return of $ret"
fi

delta=`diff -q $folder/alot4.out $folder/expected_output`
if [ "$delta" != "" ]; then
  fail "unexpected output"
fi

pass
