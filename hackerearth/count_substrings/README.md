
#PROBLEM STATEMENT

Points: 10

Given a string S of length N consisting of only lower-case English alphabets, you will be asked to process Q queries over it . In each query you will be given two lower case characters X and Y. Your task is to find out the number of such substrings of the the string S which have the characters X and Y on either of its end points, both X...Y and Y...X are considered to be valid.

Note : Substrings length should be greater than 1.

##Input

The first line of the input will contain N , the length of the string.
Next line will contain as string of length N. Next line will will contain Q , the number of queries. Then Q subsequent lines will contain two lowercase characters X and Y separated by a space.

##Output

For each query , output the answer in a separate line.

##Constraints

1 <= N <= 10^6
1 <= Q <= 10^3

##SAMPLE INPUT

5
aacbb
2
a c
a b

##SAMPLE OUTPUT

2
4

##Explanation

For the first query, the possible substrings are aac and ac. Hence the answer is 2.
For the second query, the possible substrings are aacbb, aacb, acbb, and acb, hence making a total of 4 substrings.

Time Limit: 1.0 sec(s) for each input file.
Memory Limit: 256 MB
Source Limit: 1024 KB
Marking Scheme: Marks are awarded if any testcase passes.
Allowed Languages: Bash, C, C++, C++14, Clojure, C#, D, Erlang, F#, Go, Groovy, Haskell, Java, Java 8, JavaScript(Rhino), JavaScript(Node.js), Julia, Kotlin, Lisp, Lisp (SBCL), Lua, Objective-C, OCaml, Octave, Pascal, Perl, PHP, Python, Python 3, R(RScript), Racket, Ruby, Rust, Scala, Swift, Swift-4.1, TypeScript, Visual Basic

