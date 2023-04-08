@echo off
set PROGRAM="radix"
set TEST_TGT="test_target_results"

echo TEST #1 (digit_out_of_range)
%PROGRAM% 10 2 ABCD > "%TEMP%\digit_out_of_range.txt"
fc "%TEMP%\digit_out_of_range.txt" "%TEST_TGT%\digit_out_of_range.txt" || goto err

echo TEST #2 (same_radix_pass)
%PROGRAM% 10 10 532 > "%TEMP%\same_radix_pass.txt"
fc "%TEMP%\same_radix_pass.txt" "%TEST_TGT%\same_radix_pass.txt" || goto err

echo TEST #3 (out_of_range)
%PROGRAM% 16 10 DEADBEEFDEADBEEF > "%TEMP%\out_of_range.txt"
fc "%TEMP%\out_of_range.txt" "%TEST_TGT%\out_of_range.txt" || goto err

echo TEST #4 (invalid_src_radix)
%PROGRAM% 105 5 1337228 > "%TEMP%\invalid_src_radix.txt"
fc "%TEMP%\invalid_src_radix.txt" "%TEST_TGT%\invalid_src_radix.txt" || goto err

echo TEST #5 (invalid_src_radix)
%PROGRAM% 16 666 1337228 > "%TEMP%\invalid_dst_radix.txt"
fc "%TEMP%\invalid_dst_radix.txt" "%TEST_TGT%\invalid_dst_radix.txt" || goto err

echo TEST #6 (invalid_char)
%PROGRAM% 16 8 "!153" > "%TEMP%\invalid_char.txt"
fc "%TEMP%\invalid_char.txt" "%TEST_TGT%\invalid_char.txt" || goto err

echo TEST #7 (signed_processing)
%PROGRAM% 10 16 -64 > "%TEMP%\signed_processing.txt"
fc "%TEMP%\signed_processing.txt" "%TEST_TGT%\signed_processing.txt" || goto err

goto success

:err
echo Test not passed 
exit 0

:success
echo Test passed
exit 0