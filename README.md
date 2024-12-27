# CrackingTheCodingInterview
Coding Practice for my Job interviews and personal learnings

### OS concepts
 - mmap()
 - page allocation (multiple subsequent calls to malloc())
 - mutexes
 - processes (fork())
### Hardware concepts
 - False Sharing
 - Cache hits/misses
 - TLB
 - Memory Orderings
 - Bus Faults (unaligned accesses)
### Algorithms, and Data Structures
 - Compression and Decompression using Huffman Coding
 - Nqueens problem
 - Number of possible paths from top left to bottom right in a grid
 - AVL tree
 - Maximum SubArray sum


### C++ Software Engineering
#### Use Warnings
gcc -Wall -Wextra -Wpedantic main.cpp -o ./main <br>
-Wpendatic compiler flag disables all the language extensions
#### Use Sanitisers
gcc -fsanitize=address,undefined -c main.cpp -o ./main.o <br>
Sanitizers must also be enbaled when linking, gcc -fsanitize=address,undefined main.o -o ./main
#### Use Static Analysis
$ clang-tidy ... <br>
$ cppcheck ...
