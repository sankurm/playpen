# Problem Statement
You are given N elements of an array. You need to find number of trailing zeros in the product of all these N elements.

Trailing zeros are a sequence of 0 in the decimal representation (or more generally, in any positional representation) of a number, after which no other digits (except 0) follow.

Example. Trailing zeros : 3 in 101000 , 2 in 10200, 0 in 2132, 0 in 10011.

## Input Format:

First line contains N, total number of elements.

Second line contains N space separated integers A[i]s.

## Output Format:

Print the only integer indicating number of trailing zeros in the product of all elements.

## Constraints:

1<=N<=100000

1<=A[i]<=10^9
### SAMPLE INPUT

3
4 5 6

### SAMPLE OUTPUT

1

## Explanation

Product of all elements is : 4 x 5 x 6 = 120.

Total number of trailing zeros in 120 is 1.
Time Limit: 1.0 sec(s) for each input file.
Memory Limit: 256 MB
Source Limit: 1024 KB
Marking Scheme: Marks are awarded when all the testcases pass.
Allowed Languages: Bash, C, C++, C++14, Clojure, C#, D, Erlang, F#, Go, Groovy, Haskell, Java, Java 8, JavaScript(Rhino), JavaScript(Node.js), Julia, Kotlin, Lisp, Lisp (SBCL), Lua, Objective-C, OCaml, Octave, Pascal, Perl, PHP, Python, Python 3, R(RScript), Racket, Ruby, Rust, Scala, Swift, Swift-4.1, TypeScript, Visual Basic
