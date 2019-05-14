# Sanjana Goli
## CS50 Spring 2019, Tiny Search Engine, Querier

GitHub username: sanjanagoli

To build, run `make`.

To clean up, run `make clean`.

For running `testing.sh`:  
To build, `cd ..` (to tse-sanjanagoli) and run `make`, `cd querier` so that all programs are made when running testing.sh.

Note to the Grader - Program fulfills all the following functionality (everything required by the spec):

* 10 points if your querier prints the set of documents that contain all the words in the query; you may treat operators (‘and’, ‘or’) as regular words.
* 20 points if your querier also supports ‘and’ and ‘or’ operators, but without precedence (in mathematical terms, it treats them as left associative, equal precedence operators).
* 30 points if your querier also supports ‘and’ precedence over ‘or’.
* 40 points if your querier also prints the document set in decreasing order by score, thus meeting the full specs.

*Details of program are included in DESIGN.md and IMPLEMENTATION.md*  
*Testing output from `testing.sh` is included in `testing.out` -- runs through several input boundary cases and normal uses*  
*Created `testing.out` by running `make test`*  

