#!/bin/env python3
"""Python script for generating stress test cases."""

import math
import sys
import typing as t

def generate_single_chain(size: int) -> t.List[int]:
    """Generates a FAT of a single file"""
    return [i - 1 for i in range(size)]

def generate_overlapping_twochains(size: int) -> t.List[int]:
    """Generates a FAT with all blocks pointing to 0 which points to -1"""
    # unfortunately extend doesnt return the list
    # so we have to do this ugly name binding
    data = [-1]
    data.extend(0 for i in range(size - 1))
    return data

def output_fat(fat: t.List[int], end: str = "\n") -> None:
    """Outputs a FAT into stdout"""
    for val in fat:
        print(str(val), end=end)

def main(sel: int) -> None:
    """Generates test files."""
    output_fat(
        [generate_single_chain, generate_overlapping_twochains][sel](10_000_000)
    )

# Call main function
if __name__ == "__main__":
    try:
        sel = int(sys.argv[1])
    except (ValueError, IndexError):
        raise ValueError("Must be provided with integer selector argument.")
    else:
        main(sel)
