#!/bin/bash
# testing.sh - testing for indexer.c and indextest.c
# Sanjana Goli, April 2019
# 
#
# Description: Test cases for indexer.c and indextest.c

# INDEXER TESTING

# More than three arguments
./indexer testDir oldIndexFile.txt extraarg

# Not a crawler directory
./indexer ../common oldIndexFile.txt

# Not writable file
./indexer testDir nowrite

# Working test
./indexer testDir oldIndexFile.txt

# Working test2
./indexer testDir2 indexFile1.txt

# INDEXTEST TESTING

# first input file does not exist
./indextest water.txt new.txt

# second file (file that contains output) is not writable
./indextest oldIndexFile.txt nowrite

# actual case --> using input from correct indexer case above
./indextest oldIndexFile.txt newIndexFile.txt

# actual case2 --> using input from correct indexer case above
./indextest indexFile1.txt indexFile2.txt

# Confirm success of program

gawk -f indexsort.awk indexFile1.txt > indexFile1sorted.txt
gawk -f indexsort.awk indexFile2.txt > indexFile2sorted.txt

diff indexFile2sorted.txt indexFile1sorted.txt
if [ $? -eq 0 ]; then 
	echo success
fi
