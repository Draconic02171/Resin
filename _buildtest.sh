#!/bin/bash

FILE="test/resin_test.c"
HEADER="test/stack_debug.h"

# 1. Increment BuildVersion
# Searches for 'BuildVersion = [number]' and increments it
current_version=$(grep -oP 'const int BuildVersion = \K\d+' "$FILE")
new_version=$((current_version + 1))
sed -i "s/const int BuildVersion = $current_version;/const int BuildVersion = $new_version;/" "$FILE"

# 2. Get current timestamp
now=$(date +"%Y-%m-%d %H:%M:%S")

# 3. Set StartTime only if it's still the placeholder
sed -i "s/const char StartTime\[\] = \"<set here>\";/const char StartTime[] = \"$now\";/" "$FILE"

# 4. Set TimeSpent every time
# We use the @ delimiter in sed because the date string contains slashes/spaces
sed -i "s@const char TimeSpent\[\] = \".*\";@const char TimeSpent[] = \"$now\";@" "$FILE"

gcc -pg $FILE -o test/_resin_test
test/_resin_test
# gprof test/_resin_test test/gmon.out > test/profiler.txt