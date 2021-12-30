#!/bin/bash 

n='0000'$1
echo 'result of seed' $1
echo 'result of seed' $1 > ./result/temp/${n: -4:4}.txt
./Source.exe ${@:2:($#-1)} < ./in/${n: -4:4}.txt > ./out/${n: -4:4}.txt 2>> ./result/temp/${n: -4:4}.txt