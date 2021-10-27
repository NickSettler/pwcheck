# Rules

## First rule

In this description we'll look through this rule with such parameters:

* `str = "Password"`

1. Run a loop from the begging through the end of the string
2. Check if the character in the current position is big or small
    1. Compare it using character's ASCII number
3. Using two separate variables detect that the `str` has a small one and a big one letter
4. Check if both variables from the 3rd step are truthy - the rule is succeeded. 
   Otherwise, the rule failed. 
   In our case the rule succeeded, because the `str` has a big `P` and a small `a`

## Second rule

In this description we'll look through this rule with such parameters:

* `str = "Password123"`
* `level = 3`

1. Store all four checking function in and array
2. Store results of the functions is the array of length `level`
3. Run a loop from the beginning through the end of the string
    1. Check if the current character succeeds in at least one of checking functions
    2. If it is - save this success to the results array from the 2nd step
4. Check if all results from array in the 2nd step are truthy. 
   If they are - the rule is succeeded.
   Otherwise, the rule failed.
   In our case the rule is succeeded, because we have a big `P`, a small `a` and a number `1`

## Third Rule

In this description we'll look through this rule with such parameters:

* `str = "abcab"`
* `level = 2`

1. Find number of substrings
    1. Length of `str` minus parameter `level` and plus `1`
    2. `abcab` -> `{"ab", "bc", "ca", "ab"}` `5 - 2 + 1 = 4`
2. Run two loops to compare each substring twice
    1. First loop from `0` through `[substrings_count]`
    2. Second loop from `0` through `[substrings_count]`
3. Compare if index of first loop less than index of second loop
   (we should not compare the pair of substrings already have been compared)
    * `ab` and `bc`
    * `ab` and `ca`
    * `ab` and `ab`
    * `bc` and `ca`
    * `bc` and `ab`
    * `ca` and `ab`
4. If any of those pairs are the same then the rule is falsy. 
   Otherwise, the rule succeeded. 
   In our example the rule is failed because we had a pair of `ab` and `ab`