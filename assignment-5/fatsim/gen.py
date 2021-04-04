#!/bin/env python3
import random, sys
from random import randrange as rr

class FatMaker:
    def __init__(self):
        self.fat = []
    def alloc(self, n, last=None):
        if last == None:
            last = -1
            n = n-1
        firstBc = len(self.fat)
        bc = firstBc
        for i in range(n):
            self.fat.append(bc + 1)
            bc += 1
        self.fat.append(last)
        return firstBc

    def cycle(self,n):
        firstBc = len(self.fat)
        bc = firstBc
        for i in range(n-1):
            self.fat.append(bc + 1)
            bc += 1
        self.fat.append(firstBc)
        return firstBc

    def lookup(self, first, n = 0):
        b = first
        for i in range(n):
            b = self.fat[b]
            if b == -1:
                break
        return b

    def shuffle(self):
        perm = list(range(len(self.fat)))
        random.shuffle(perm)
        fat = self.fat[:]
        for i in range(len(self.fat)):
            a = i
            b = fat[i]
            a = perm[a]
            if b != -1: b=perm[b]
            self.fat[a] = b

    def disp(self):
        cnt = 0;
        for f in self.fat:
            print(f"{f:>4d}", end=' ')
            cnt += 1
            if cnt % 10 == 0: print()
        if cnt % 10 != 0: print()

    def size(self):
        return len(self.fat)
            
from random import random as rn
from random import randrange as rr

def main(n):
    s = n // 100 + 4
    fm = FatMaker()
    files = [fm.cycle(1)]
    while fm.size() < n:
        if rn() < 0.1:
            files.append( fm.cycle( rr(1, s)))
        elif rn() < 0.1 or len(files) < 3:
            files.append( fm.alloc( rr(1,2*s)))
        else:
            f = files[rr(0,len(files))]
            nf = fm.alloc( rr(0,s), fm.lookup(f, rr(0,s)))
            files.append( nf)
        # print("fm.size=", fm.size())

    # fm.disp()
    fm.shuffle()
    fm.disp()
    return

if __name__ == "__main__":
    n = 100
    seed = 0
    if len(sys.argv) >= 2:
        n = int(sys.argv[1])
    if len(sys.argv) >= 3:
        seed = int(sys.argv[2])
    random.seed( seed, 2)

    main(n)
    
