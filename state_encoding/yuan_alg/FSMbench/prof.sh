#!/bin/bash

# compile weights profiler
#gcc -Wall prof.c ../../benchmarks/clarissa/clarissa.c -o prof 

# run
input=../../benchmarks/clarissa/data/chunks/chunk-100/xab

./prof ../../benchmarks/clarissa/commadot/dfa-131-12-51.table 131 12 51 $input
mv ${input}w_edge chunk-100 
mv ${input}w_node chunk-100

