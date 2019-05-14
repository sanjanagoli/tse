#!/bin/bash
# testing.sh - testing for querier.c
# Sanjana Goli, April 2019
# 

# Creating inputs
mkdir testDir
../crawler/crawler http://old-www.cs.dartmouth.edu/~campbell/page-4/ testDir 1
../indexer/indexer testDir oldIndexFile.txt

# COMMANDLINE INPUT TESTING

## non-crawler directory
echo deanna | ./querier notcrawler oldIndexFile.txt

## non-existent inputfile
echo deanna | ./querier testDir old.txt

## simple working case
echo deanna | ./querier testDir oldIndexFile.txt

## simple working case (with valgrind)
echo understanding | valgrind ./querier testDir oldIndexFile.txt

# create file for conjuctions testing (boundary cases)
echo and deanna > tester
echo or deanna >> tester
echo deanna home or >> tester
echo deanna home and >> tester
echo deanna and or home >> tester
echo deanna or and home >> tester

./querier testDir oldIndexFile.txt < tester

# bad characters in file
echo "deanna2 home" > tester
echo "the!y" >> tester
echo "-the(y" >> tester

./querier testDir oldIndexFile.txt < tester

# word does not exist in file
echo dog | ./querier testDir oldIndexFile.txt

# several spaces in front
echo      health | ./querier testDir oldIndexFile.txt

# capitalized word that is in file
echo DARTMouth | ./querier testDir oldIndexFile.txt

# AND tests

## no intersection
echo visions and silvia | ./querier testDir oldIndexFile.txt

## with intersections
echo nimh and understanding | ./querier testDir oldIndexFile.txt

## without 'and'
echo nimh understanding | ./querier testDir oldIndexFile.txt

## several words with intersections
echo nimh understanding department | ./querier testDir oldIndexFile.txt

## one word does not exist in file
echo dog and deanna | ./querier testDir oldIndexFile.txt

## one word does not exist in file
echo deanna and dog | ./querier testDir oldIndexFile.txt

## spaces between words
echo     nimh      understanding   department | ./querier testDir oldIndexFile.txt

# OR tests

## no intersection
echo visions or silvia | ./querier testDir oldIndexFile.txt

## with intersections
echo nimh or understanding | ./querier testDir oldIndexFile.txt

## several words with intersections
echo nimh or understanding or department | ./querier testDir oldIndexFile.txt

## one word does not exist in file
echo dog or deanna | ./querier testDir oldIndexFile.txt

# AND OR combinations

## combinations of provided example
echo dartmouth college or computer science | ./querier testDir oldIndexFile.txt

echo dartmouth or college and computer science | ./querier testDir oldIndexFile.txt

echo dartmouth and college computer or science | ./querier testDir oldIndexFile.txt


# cleaning files
rm -rf testDir
rm -f tester
rm -f oldIndexFile.txt
