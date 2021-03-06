# Linear System of Equations Solver

Solve any system of linear equations using the method of Gauss-Jordan elimination and determine if there's unique, infinite or no solutions. If there are infinite solutions, null space is determined using Reduced Row Echelon Form Method.

## Instructions

Enter command 'make' to compile.

There are four main files:
1. **matrix.h** - Header file containing function declarations.

2. **runner.c** - Accepts input in a file "input.txt" by default, can be changed by commenting FILE_IN directive in matrix.h. Output is given in "output.txt"
    Input format:
```
  t - Number of test cases
  m n - Dimensions
  A - m x n matrix
  b = m x 1 matrix
```

  Output format:
```
  Test x
  Nature of solution
  Particular solution (if exists)
  Dimensions of null space (if exists)
  Null space solution (if exists)
```
3. **tester.c** - Checks the solutions generated by the program. It checks if Axp = b and Axn = 0 and writes the verdict in "results.txt"

4. **generator.m** - Matlab script to generate test cases in a format acceptable by the program. It can be used to create "input.txt" with the desired number of cases.
    "input.txt" has 100 randomly generated test cases from this script for now.

Some trivial/tricky test cases are placed in a file "manualtests.txt", which were used by me for testing. They are guaranteed to work, too.

C programs are written as per C89 standard. Can be verified by running:

```sh
gcc runner.c -o runner.o -std=c89 -pedantic
gcc tester.c -o tester.o -std=c89 -pedantic
```

'-pedantic' flag forces the compiler to check for C89 standards.

