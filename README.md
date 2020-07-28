# Tiny Search Engine

## Setup
To build, run `make`.

To clean up, run `make clean`.

## Description
Tiny Search Engine is a simple search engine, which includes a crawler, indexer, and querier. The crawler crawls the web from a given seed to specified maxDepth and caches the content of the pages it finds, one page per file, in a given directory. Indexer creates an index (ranking system) based on the results produced by the crawler and writes this index to a file. Querier is a program to read the index file produced by Indexer, and page files produced by the Crawler to respond to queries entered through stdin.

## Notes
*Details of implementation and design are included in respective directories*  
*common directory includes `pagedir.c` and `pagedir.h` that hold a library of functions used for `crawler.c` and `index.c`*  
*common directory also includes `index.c`, `index.h`, `word.c`, and `word.h` that hold a library of functions used in `indexer.c`, `indextest.c`, and `querier.c`*
