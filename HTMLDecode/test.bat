@echo off
set PROGRAM="htmldecode"
set TEST_TGT="tests"

echo TEST #1
%PROGRAM% < "%TEST_TGT%\test_1.txt" > "%TEMP%\test_1r.txt" 2>&1 && goto RE
fc "%TEMP%\test_1r.txt" "%TEST_TGT%\test_1r.txt" || goto WA

echo TEST #2
%PROGRAM% < "%TEST_TGT%\test_2.txt" > "%TEMP%\test_2r.txt" 2>&1 && goto RE
fc "%TEMP%\test_2r.txt" "%TEST_TGT%\test_2r.txt" || goto WA

echo TEST #3
%PROGRAM% < "%TEST_TGT%\test_3.txt" > "%TEMP%\test_3r.txt" 2>&1 || goto RE
fc "%TEMP%\test_3r.txt" "%TEST_TGT%\test_3r.txt" || goto WA

echo TEST #4
%PROGRAM% < "%TEST_TGT%\test_4.txt" > "%TEMP%\test_4r.txt" 2>&1 || goto RE
fc "%TEMP%\test_4r.txt" "%TEST_TGT%\test_4r.txt" || goto WA

goto success

:WA
echo Test not passed (wrong answer)
exit -1

:RE
echo Test not passed (runtime error)
exit -2

:success
echo Test passed