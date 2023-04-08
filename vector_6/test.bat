@echo off
set PROGRAM="vector_6.exe"
set TEST_TGT="test_data"

echo TEST #1
%PROGRAM% < "%TEST_TGT%\test_1.txt" > "%TEMP%\test_1_r.txt" || goto RE
fc "%TEMP%\test_1_r.txt" "%TEST_TGT%\test_1_expected.txt" || goto WA

echo TEST #2
%PROGRAM% < "%TEST_TGT%\test_2_1num.txt" > "%TEMP%\test_2_1num_r.txt" || goto RE
fc "%TEMP%\test_2_1num_r.txt" "%TEST_TGT%\test_2_1num_expected.txt" || goto WA

echo TEST #3
%PROGRAM% < "%TEST_TGT%\test_3_empty.txt" > "%TEMP%\test_3_empty_r.txt" || goto RE
fc "%TEMP%\test_3_empty_r.txt" "%TEST_TGT%\test_3_empty_expected.txt" || goto WA

goto success

:WA
echo Test not passed (wrong answer)
exit -1

:RE
echo Test not passed (runtime error)
exit -2

:success
echo Test passed