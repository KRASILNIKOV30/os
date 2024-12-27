#!/usr/bin/env bash
set -e

g++ -c -fPIC ./lib/src/equationsolver.cpp -o equationsolver.o

g++ -shared -o libequationsolver.so equationsolver.o

g++ -I ./lib/include -L . ./app/main.cpp -lequationsolver -o myapp

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./myapp