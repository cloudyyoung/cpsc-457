# deadlock detection - skeleton for Assignment 4

To compile all code, type:
```
$ make
```

To run the resulting code on file test1.txt:
```
$ ./deadlock < test1.txt
```

## IMPORTANT

Only modify and submit the `deadlock_detector.cpp` file. Your TAs will
supply their own versions of the other files (such as main.cpp) to
compile and test your code.

## Test files

The repository includes several test files. Here are correct results for these test files.

| test file   | correct edge_index | correct cycle | minimum timings     | good timings     |
| :---------- | :-------------: | :-----------: | :-----------------: | :--------------: |
| `test1.txt` | `-1`            | `[]`          | `0.0000s`           | `0.0000s`        |
| `test2.txt` | `5`             | `[5,7]`       | `0.0000s`           | `0.0000s`        |
| `test3.txt` | `3`             | `[p7,p3]`     | `0.0000s`           | `0.0000s`        |
| `test4.txt` | `3`             | `[12,7]`      | `0.0000s`           | `0.0000s`        |
| `test5.txt` | `6`             | `[2,77]`      | `0.0000s`           | `0.0000s`        |
| `test6.txt` | `9903`          | `[ab,cd,ef]`  |`8.9431s`            | `0.8771s`        |
| `test7.txt` | `29941`         | `[is,this,answer,the,correct]`  |`191.7872s`    | `8.0726s`        |


