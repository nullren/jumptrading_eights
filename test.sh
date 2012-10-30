#!/bin/bash -e

g++ main.cpp -Wall -msse4.2 -o compute -O2
time ./compute

