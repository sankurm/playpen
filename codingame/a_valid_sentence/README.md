# A Valid Sentence

## Problem Statement
You have to test if a sentence complies with our rule.

Our rule : a sentence is valid if by successively rotating its words to the right we can find a form of the sentence whose words' sizes make a increasing sequence of difference 1.

Example with the sentence "The grey Mo" and its two only possible rotated forms:
The grey Mo >>> Mo The grey >>> grey Mo The
It's a valid sentence because "Mo The grey" forms a sequence of words of size 2 3 4, and 2<3<4 and 3-2=1 and 4-3=1

## Input
Line 1: A sentence

## Output
Line1 :The rotated valid sentence if one exists else ERROR

## Constraints
Input contains only letters and spaces, two words are always separated by exactly one simple space.
There is no punctuation.
All sentences are at least 1 word long.

## Example
### Input

moon peaks to the

### Output

to the moon peaks
