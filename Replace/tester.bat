@echo off

set PROGRAM="%~1"
set TEST_LOC="tests"
set TEST_TGT="test_target_results"

%PROGRAM% "%TEST_LOC%\all_replaced.txt" "%TEMP%\all_replaced_output.txt" abc replaced
echo TEST #1 (all_replaced) ... 
fc.exe "%TEMP%\all_replaced_output.txt" "%TEST_TGT%\all_replaced_needed.txt"  goto err
echo TEST #1 PASSED

%PROGRAM% "%TEST_LOC%\empty_input.txt" "%TEMP%\empty_input_output.txt" some_text_1 some_text_2
echo TEST #2 (empty_input) ...
fc.exe "%TEMP%\empty_input_output.txt" "%TEST_TGT%\empty_input_needed.txt"  goto err
echo TEST #2 PASSED

%PROGRAM% "%TEST_LOC%\no_replacements.txt" "%TEMP%\no_replacements_output.txt" xx why_it_replaced
echo TEST #3 (no_replacements) ...
fc.exe "%TEMP%\no_replacements_output.txt" "%TEST_TGT%\no_replacements_needed.txt"  goto err
echo TEST #3 PASSED

%PROGRAM% "%TEST_LOC%\overlap.txt" "%TEMP%\overlap_output.txt" 1234 12345
echo TEST #4 (overlap) ...
fc.exe "%TEMP%\overlap_output.txt" "%TEST_TGT%\overlap_output_needed.txt"  goto err
echo TEST #4 PASSED

echo TEST #6 (large_input) ...
%PROGRAM% "%TEST_LOC%\large_input.txt" "%TEMP%\large_input.txt" abcdefghklm x
fc "%TEMP%\large_input.txt" "%TEST_TGT%\large_input_needed.txt"  goto err

echo TEST #5 (invalid_search_pattern) ...
%PROGRAM% "%TEST_LOC%\large_input.txt" "%TEMP%\invalid_search_pattern_output.txt" "" heyy_dude
echo TEST #5 PASSED

:okay
echo OK
exit 0

:err
echo Program testing failed
exit 1