#!/usr/bin/env bash

# Get the script directory
script_dir=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

python ${script_dir}/benchmark_list.py > ${script_dir}/benchmark_list.txt
cd ${script_dir}/../build
./benchmark_list >> ${script_dir}/benchmark_list.txt
cd ${script_dir}
