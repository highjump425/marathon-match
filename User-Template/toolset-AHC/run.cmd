@echo off
set local
setlocal enabledelayedexpansion

for /l %%a in (%1,1,%2) do (
  set n=0000%%a
  set n=!n:~-4,4!
  echo result of seed %%a
  Source < ./in/!n!.txt > ./out/!n!.txt
)
