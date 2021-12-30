@echo off
set local
setlocal enabledelayedexpansion

type nul > ./result/result.txt
for /l %%a in (%1,1,%2) do (
  echo result of seed %%a
  echo result of seed %%a >> ./result/result.txt
  java -jar tester.jar -exec ChickenRun.exe -pr -seed %%a -novis >> ./result/result.txt
)
