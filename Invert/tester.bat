@echo off

set PROGRAM="%~1"
set TEST_LOC="tests"
set TEST_TGT="test_target_results"

echo "TEST #1 (invalid_format) ..."
%PROGRAM% "%TEST_LOC%\invalid_format.txt" > "%TEMP%\invalid_format.txt"
fc "%TEMP%\invalid_format.txt" "%TEST_TGT%\invalid_format_needed.txt"  goto err
echo "TEST #1 PASSED"

echo "TEST #2 (inverse_matrix_not_exist) ..."
%PROGRAM% "%TEST_LOC%\not_exist.txt" > "%TEMP%\not_exist.txt"
fc "%TEMP%\not_exist.txt" "%TEST_TGT%\not_exist_needed.txt"  goto err
echo "TEST #2 PASSED"

echo "TEST #3 (test) ..."
%PROGRAM% "%TEST_LOC%\test.txt" > "%TEMP%\test.txt"
fc "%TEMP%\test.txt" "%TEST_TGT%\test_needed.txt"  goto err
echo "TEST #3 PASSED"

