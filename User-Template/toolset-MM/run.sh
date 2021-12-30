#!/bin/bash 

n='0000'$1
echo 'result of seed' $1
echo 'result of seed' $1 > ./result/temp/${n: -4:4}.txt
java -jar tester.jar -exec "./StopTheElves.exe ${@:2:($#-1)}" -pr -seed $1 -novis >> ./result/temp/${n: -4:4}.txt