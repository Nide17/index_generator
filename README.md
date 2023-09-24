## index_generator

__INTRODUCTION__

This repo contains a program that is a C-based word indexer that reads text input line-by-line and parses out words, recording their occurrences and the lines they appear on. It handles common words, ignores case, and produces a sorted index of words and their occurrences.

__DESCRIPTION__

The program performs the following tasks:

1. Reads input line-by-line, assigning line numbers starting at 1 for the first line.
2. Parses each line into words, considering alphabetic characters, apostrophes, and hyphens within words.
3. Ignores common words as defined in common_words.h.
4. Records non-common words, keeping track of their occurrences and the lines they appear on.
5. Sorts the recorded words by frequency (descending) and alphabetically within each frequency.
6. Outputs a sorted index showing words and their occurrences.


__IMPORTANCE__

This program is important because it can be used to implement a search engine. It can be used to index words in a document and then search for a word in the index. It can also be used to implement a spell checker. It can be used to index words in a dictionary and then search for a word in the index, and if the word is not found, it can be suggested to the user as a possible misspelling.


__GETTING STARTED__

* Clone this repository to get the index_generator.c program.
* In the terminal, navigate to the program's directory.
* Compile the program by running the command:
```
gcc -Wall -Werror index_generator.c -o index_generator
```
* Test the program using the command:
```
assignment4_tester.py ./index_generator
```
  
__TESTING__

This program is implemented and tested against 25 input implemented by the python script with test cases that has known outputs. A test suite is provided inside the file 'assignment4_tester.py' to ensure correctness.
  
 __KEYWORDS__

<mark>ISSE</mark>     <mark>CMU</mark>     <mark>Assignment4</mark>     <mark>index_generator</mark>     <mark>C Programming</mark>     <mark>Strings</mark>

  __AUTHOR__

 Written by parmenin (Niyomwungeri Parmenide ISHIMWE) at CMU-Africa - MSIT

 __DATE__

 September 24, 2023
