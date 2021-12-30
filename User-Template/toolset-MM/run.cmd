@echo off
set local
setlocal enabledelayedexpansion

for /l %%a in (%1,1,%2) do (
  echo result of seed %%a
  java -jar tester.jar -exec ChickenRun.exe -pr -seed %%a
)
