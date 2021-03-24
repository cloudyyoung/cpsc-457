#!/usr/bin/env python3

import sys

READ_SIZE = 2**20

def usage():
    print("Usage: dup.py N")
    print("  - copies N bytes from stdin to stdout")
    print("  - if stdin has less than N bytes, the data is repeated")
    print("    until N bytes are produced")
    sys.exit(-1)

def w(b):
    sys.stdout.buffer.write(b)

def main2():
    if len(sys.argv) != 2: usage()
    try:
        N = int(sys.argv[1])
        assert N>0
    except:
        usage()

    remaining = N
    saved = bytearray()
    while True:
        block = sys.stdin.buffer.read( min(READ_SIZE, remaining))
        if len(block) == 0: break
        w(block)
        remaining -= len(block)
        if len(saved) < remaining:
            saved.extend(memoryview(block)[:remaining-len(saved)])
            pass

    if len(saved) == 0 and remaining > 0:
        print("No data on standard input!!!")
        sys.exit(-1)
        
    if len(saved) > 0:
        while len(saved) < min(READ_SIZE,remaining):
            saved.extend(saved)

    while remaining > 0:
        bs = min(len(saved), remaining)
        w(saved[:bs])
        remaining -= bs
        

if __name__ == "__main__":
    main2()
