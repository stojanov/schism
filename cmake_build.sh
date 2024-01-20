#!/bin/bash

cmake --build build  
cd build/schism
./schism
cd ../..
