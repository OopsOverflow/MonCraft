#!/usr/bin/env sh

OUT=$PWD/iwyu.txt
MAP=$PWD/iwyu_mapping.json
echo $OUT

cd ../DebugOptimized

# -Xiwyu --verbose=3 \
iwyu_tool.py -j 6 -p . -- \
-Xiwyu --mapping_file="$MAP" \
-I/usr/lib64/clang/12.0.1/include > $OUT

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
