#!/bin/sh

# Tested on Ubuntu 18.04

export CXX=g++
./basic_dependencies.sh
pwd
ls
bash ./clean.sh
bash ./build.sh
