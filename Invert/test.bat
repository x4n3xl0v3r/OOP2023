@echo off

set PROGRAM="invert"
set TEST_LOC="tests"
set TEST_TGT="tests"

:: переместить вход/выход в одну папку
:: проверять код возврата с помощью && / ||

echo "TEST #1 (invalid_format) ..."
%PROGRAM% "%TEST_LOC%\invalid_format.txt" > "%TEMP%\invalid_format.txt" || goto failed
fc "%TEMP%\invalid_format.txt" "%TEST_TGT%\invalid_format_needed.txt" || goto err
echo "TEST #1 PASSED"

echo "TEST #2 (inverse_matrix_not_exist) ..."
%PROGRAM% "%TEST_LOC%\not_exist.txt" > "%TEMP%\not_exist.txt" || goto failed
fc "%TEMP%\not_exist.txt" "%TEST_TGT%\not_exist_needed.txt" || goto err
echo "TEST #2 PASSED"

echo "TEST #3 (test) ..."
%PROGRAM% "%TEST_LOC%\test.txt" > "%TEMP%\test.txt" || goto failed
fc "%TEMP%\test.txt" "%TEST_TGT%\test_needed.txt" || goto err
echo "TEST #3 PASSED"

goto success

:err
echo Tests not passed (wrong answer)
exit 0

:failed
echo Tests not passed (returned invalid code)
exit 0

:success
echo Tests passed
exit 0 