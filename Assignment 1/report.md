# Assignment 1

## Q1

a.

```
$ time ./slow-pali < t3.txt
Longest palindrome: ___o.O.o___

real    0m0.176s
user    0m0.015s
sys     0m0.015s
```

```
$ time ./slow-pali < t4.txt
Longest palindrome: redder

real    0m21.047s
user    0m0.015s
sys     0m0.000s
```

```
$ time python palindrome.py < t3.txt
Longest palindrome: ___o.O.o___

real    0m0.082s
user    0m0.000s
sys     0m0.015s
```

```
$ time python palindrome.py < t4.txt
Longest palindrome: redder

real    0m0.451s
user    0m0.015s
sys     0m0.015s
```
