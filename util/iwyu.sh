#!/usr/bin/env sh

OUT=$PWD/iwyu.txt
MAP=$PWD/iwyu_mapping.json
CLANG_INC=$(clang++ -print-resource-dir)/include

echo "output: $OUT"
echo "clang includes: $CLANG_INC"

cd ../DebugOptimized

# -Xiwyu --verbose=3 \
iwyu_tool.py -j 10 -p . -- \
-Xiwyu --mapping_file="$MAP" \
-I$CLANG_INC > $OUT

cd ../DebugOptimized && fix_includes.py \
-b -m \
--nosafe_headers \
--reorder \
--separate_project_includes="../OpenGL/src" \
--ignore_re="debug/Debug.cpp" \
< $OUT

cd -


# ^(\(.+\)|---)$\n\n^In([\s\S]+?)^(\(.+\)|.+should add these lines:)$ -> $1\n\n$3
# ^\(.+\)$\n\n
# \n\nthe full[\s\S]+?---
# .+should .+\n\n
