#!/bin/bash

clear

C_FILE="src/main.c"
SRC_DIRS=("src/compile" "src/runtime" "src/utills")
INC_DIR="src/header"
OUTPUT="resin"

FILES=$(find "${SRC_DIRS[@]}" -name "*.c")
CURRENT_VAL=$(grep "const unsigned long BuildVersion =" $C_FILE | grep -oE '[0-9]+')
NEW_VAL=$((CURRENT_VAL + 1))

sed -i "s/const unsigned long BuildVersion = [0-9]*;/const unsigned long BuildVersion = $NEW_VAL;/" "$C_FILE"
gcc -I"$INC_DIR" -o "$OUTPUT" "$C_FILE" $FILES
