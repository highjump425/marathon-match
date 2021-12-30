#!/bin/bash 

mkdir ./result/temp
path="./result/log"
if [ ! -e $path ];then
  mkdir ./result/log
fi
fname=$(date "+%m-%d-%H-%M")
seq $1 $2 | xargs -P 4 -I {} ./run.sh {} ${@:3:($#-2)}
cat ./result/temp/*.txt > ./result/log/$fname.txt
rm -r ./result/temp
./summary.exe log/$fname.txt
./summary.exe log/$fname.txt > ./result/summary.txt
