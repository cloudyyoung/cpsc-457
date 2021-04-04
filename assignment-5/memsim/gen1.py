#!/bin/env python3
import random, sys
from random import randrange as rr

tagRange = (1,99)
reqSize = (1,10000)
freeProb = 0.3

if __name__ == "__main__":
    n = 100
    if len(sys.argv) != 2:
        print(f"gonna generate {n} lines", file=sys.stderr)
    else:
        n = int(sys.argv[1])

    reqs = []
    nfree = int(freeProb * n)
    for i in range(n - nfree):
        reqs.append(f"{rr(*tagRange)} {rr(*reqSize)}")
    for i in range(nfree):
        reqs.append(f"-{rr(*tagRange)}")

    random.shuffle(reqs)

    print("\n".join(reqs))
            
    
