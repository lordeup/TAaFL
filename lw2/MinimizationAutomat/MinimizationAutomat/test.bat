@echo off

rem test.bat "$(TargetPath)"

set PROGRAM="%~1"

set OUT="%TEMP%\out.txt"

call %PROGRAM% tests\input1.txt> %OUT%
fc %OUT% tests\output1.txt
if errorlevel 1 goto err
echo Test: input1 PASSED

call %PROGRAM% tests\input2.txt> %OUT%
fc %OUT% tests\output2.txt
if errorlevel 1 goto err
echo Test: input2 PASSED

call %PROGRAM% tests\input3.txt> %OUT%
fc %OUT% tests\output3.txt
if errorlevel 1 goto err
echo Test: input3 PASSED

call %PROGRAM% tests\input4.txt> %OUT%
fc %OUT% tests\output4.txt
if errorlevel 1 goto err
echo Test: input4 PASSED

call %PROGRAM% tests\input5.txt> %OUT%
fc %OUT% tests\output5.txt
if errorlevel 1 goto err
echo Test: input5 PASSED

call %PROGRAM% tests\input6.txt> %OUT%
fc %OUT% tests\output6.txt
if errorlevel 1 goto err
echo Test: input6 PASSED

call %PROGRAM% tests\input7.txt> %OUT%
fc %OUT% tests\output7.txt
if errorlevel 1 goto err
echo Test: input7 PASSED

call %PROGRAM% tests\input8.txt> %OUT%
fc %OUT% tests\output8.txt
if errorlevel 1 goto err
echo Test: input8 PASSED

call %PROGRAM% tests\input9.txt> %OUT%
fc %OUT% tests\output9.txt
if errorlevel 1 goto err
echo Test: input9 PASSED

call %PROGRAM% tests\input10.txt> %OUT%
fc %OUT% tests\output10.txt
if errorlevel 1 goto err
echo Test: input10 PASSED

echo All tests passed
exit / B 0

:err
echo Program testing failed
exit / B 1