# Rules

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
4. If any of those pairs is the same then the rule is falsy. Otherwise, the rule succeeded