# detects primes from a list of numbers on standard input

This is a skeleton code for an assignment in CPSC457.

The program takes a single argument - number of threads that should be used
to speed up the computation.

* reads in a positive integers in range (2 .. 2^63-1) from standard input
* for each number on input:
  * if the number is not prime, it is ignored
  * if the number is prime, it is printed to standard output


compile with:
```sh
$ make
```

To find all prime numbers betwee 1 and 1000:
```console
$ seq 1000 | ./detectPrimes 5
Using 5 threads.
Identified 168 primes:
  2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101
  103 107 109 113 127 131 137 139 149 151 157 163 167 173 179 181 191 193 197
  199 211 223 227 229 233 239 241 251 257 263 269 271 277 281 283 293 307 311
  313 317 331 337 347 349 353 359 367 373 379 383 389 397 401 409 419 421 431
  433 439 443 449 457 461 463 467 479 487 491 499 503 509 521 523 541 547 557
  563 569 571 577 587 593 599 601 607 613 617 619 631 641 643 647 653 659 661
  673 677 683 691 701 709 719 727 733 739 743 751 757 761 769 773 787 797 809
  811 821 823 827 829 839 853 857 859 863 877 881 883 887 907 911 919 929 937
  941 947 953 967 971 977 983 991 997

Finished in 0.0000s
```
To test the numbers in file `easy.txt`:
```console
$ ./detectPrimes 1 < ./easy.txt
Using 1 thread.
Identified 7 primes:
  2 3 5 7 101 103 107

Finished in 0.0000s
```

