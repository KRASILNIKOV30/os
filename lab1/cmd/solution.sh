#!/usr/bin/env bash
set -e

test out && rm -rf out
mkdir out

cd out
whoami > me.txt

cp me.txt metoo.txt

man wc > wchelp.txt

wc -l wchelp.txt | cut -d ' ' -f 1 > wchelp-lines.txt

tac wchelp.txt > wchelp-reversed.txt

cat wchelp.txt wchelp-reversed.txt me.txt metoo.txt wchelp-lines.txt > all.txt

tar -c -f result.tar *.txt

gzip result.tar

cd ..
test result.tar.gz && rm result.tar.gz
mv out/result.tar.gz ./

rm -rf out
