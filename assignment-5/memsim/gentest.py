#!/bin/env python3
import random, sys
import subprocess
from random import randrange as rr

# range [1, 10 000 000]
tagRange = (1,1001)
# range [1, 10 000 000]
reqSize = (1,10_000_001)
# range [1, 1 000 000]
pageSize = (1,1_000_001)
# percentage of requests are deallocate
freeProb = 0.3

if __name__ == "__main__":

    iters = int(sys.argv[1])

    # get seed
    try:
        seed = int(sys.argv[2])
    except IndexError:
        seed = 0
    
    # get number of requests
    try:
        n = int(sys.argv[3])
    except IndexError:
        n = 1_000_000

    for seed in range(seed, iters+seed):

        random.seed(seed)

        reqs = []
        nfree = int(freeProb * n)
        reqs.extend(f"{rr(*tagRange)} {rr(*reqSize)}" for _ in range(n - nfree))
        reqs.extend(f"-{rr(*tagRange)}" for _ in range(nfree))

        random.shuffle(reqs)

        page_size = rr(*pageSize)

        pyRes = subprocess.run(["./memsim.py", str(-page_size)], input="\n".join(reqs), text=True, capture_output=True)
        cRes = subprocess.run(["./memsim", str(page_size)], input="\n".join(reqs), text=True, capture_output=True)

        pyOut = pyRes.stdout.splitlines()[2:5]
        cOut = cRes.stdout.splitlines()[2:5]

        if (pyOut != cOut):
            print(f"Differing behaviour on seed {seed}:")
            print("Python: \n" + "\n".join(pyOut))
            print("Cpp: \n" + "\n".join(cOut))
            print()
        else:
            print(f"Success on seed {seed}.")
    