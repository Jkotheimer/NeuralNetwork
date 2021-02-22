#!/usr/bin/bash

g++ -o main main.cpp -lpng -fno-stack-protector -std=c++17
[ $? -eq 0 ] && ./main $* || echo "Something went wrong :/"
