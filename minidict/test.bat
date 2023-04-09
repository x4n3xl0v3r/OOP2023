@echo off
set PROGRAM="minidict"
set TEST_TGT="tests"

rem dict.txt
type nul > dict.txt

echo TEST #1
%PROGRAM% < "%TEST_TGT%\test_1.txt" > "%TEMP%\test_1r.txt" 2>&1 || goto RE
fc "%TEMP%\test_1r.txt" "%TEST_TGT%\test_1r.txt" || goto WA

echo TEST #2
%PROGRAM% < "%TEST_TGT%\test_2.txt" > "%TEMP%\test_2r.txt" 2>&1 || goto RE
fc "%TEMP%\test_2r.txt" "%TEST_TGT%\test_2r.txt" || goto WA

echo TEST #3
%PROGRAM% < "%TEST_TGT%\test_3.txt" > "%TEMP%\test_3r.txt" 2>&1 || goto RE
fc "%TEMP%\test_3r.txt" "%TEST_TGT%\test_3r.txt" || goto WA

echo TEST #4
%PROGRAM% < "%TEST_TGT%\test_4.txt" > "%TEMP%\test_4r.txt" 2>&1 || goto RE
fc "%TEMP%\test_4r.txt" "%TEST_TGT%\test_4r.txt" || goto WA

echo TEST #5
%PROGRAM% < "%TEST_TGT%\test_5.txt" > "%TEMP%\test_5r.txt" 2>&1 || goto RE
fc "%TEMP%\test_5r.txt" "%TEST_TGT%\test_5r.txt" || goto WA

goto success

:WA
echo Test not passed (wrong answer)
exit 0

:RE
echo Test not passed (runtime error)
exit 0 

:success
echo Test passed