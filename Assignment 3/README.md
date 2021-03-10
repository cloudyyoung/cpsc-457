Calculates PI by summing the circle's area, as explained here:

https://en.wikipedia.org/wiki/Approximations_of_%CF%80#Summing_a_circle's_area

You need to re-implement `count_pi()` in calcpi.cpp to make it multi-threaded.

To compile the code:
```
make
```
To run it with r=1000 and n_threads=1:
```
./calcpi 1000 1
```
