@echo off
set local
setlocal enabledelayedexpansion

type nul > ./result/result.txt
for /l %%a in (%1,1,%2) do (
  set n=0000%%a
  set n=!n:~-4,4!
  echo result of seed %%a
  echo result of seed %%a >> ./result/result.txt
  Source < ./in/!n!.txt > ./out/!n!.txt 2>> ./result/result.txt
)
