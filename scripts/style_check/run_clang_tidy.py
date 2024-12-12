#!/usr/bin/python3

import os

clang_tidy_default_path="/usr/bin/clang-tidy"
clang_tidy_default_file_hint_name=".clang-tidy-hint"
cur_path = os.path.abspath(__file__)
project_dir = os.path.abspath(cur_path + "/../../../")
sources_map={}

for root, dirs, files in os.walk(project_dir):
    if files.count(clang_tidy_default_file_hint_name):
        with open(os.path.abspath(root + "/" + clang_tidy_default_file_hint_name)) as f:
            # first line contains info should we check or not
            flag = f.readline().split()[1]
            if (flag == "false"):
                continue
        # Collect all *.cpp files in this directory
        sources = ""
        for file in files:
            if file.endswith(".cpp"):
                sources += os.path.abspath(root + "/" + file) + " "
        sources_map[root] = sources

print(sources_map)
for key, value in sources_map.items():
    # run clang-tidy
    os.system(f"{clang_tidy_default_path} {value} -header-filter=.* --config-file={project_dir}/.clang-tidy")