#!/bin/bash
cmake . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cp ./build/compile_commands.json ./
