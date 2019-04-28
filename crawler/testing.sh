#!/bin/bash
# testing.sh - testing for crawler.c
# Sanjana Goli, April 2019
# 
#
# Description: Testing normal and boundary cases for program (invalid parameters -- non-internal URL, too many args, incorrect depth, etc)


# examples given on lecture demonstration

./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html testDir 1

# need to remove files between test runs because assuming no files with integer names upon run
make removeTestFiles

./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html testDir 1

make removeTestFiles

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html testDir 1

make removeTestFiles

# example with depth 2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testDir 2

make removeTestFiles

# example with depth 0
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html testDir 0

make removeTestFiles

# BOUNDARY CASES 

# directory that does not exist
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html doesnotexist 1

# max depth is less than 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html testDir -1

# max depth is greater than 10
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html testDir 11

# URL is not internal
./crawler http://wikipedia.com testDir 1

# seedURL that points to a non-existent server.
./crawler abcdef testDir 1

# seedURL that points to a valid server but non-existent page
./crawler http://old-www.cs.dartmouth.edu/~campbell/indexes.html testDir 1

# extra arguments
./crawler http://old-www.cs.dartmouth.edu/~campbell/index.html testDir 0 extraargument
