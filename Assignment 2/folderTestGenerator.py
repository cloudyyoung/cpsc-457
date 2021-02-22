# /usr/bin/env python3
"""Generates a test for 457 A2"""

import shutil
import random
import os

if __name__ == "__main__":

    try:
        shutil.rmtree("test5")
    except FileNotFoundError:
        pass

    os.mkdir("test5")

    # Make a 1000 deep directory chain
    path = "test5"
    offBranch = ""

    for i in range(100):
        newDir = f"deepDir{i}"
        path += "/" + newDir
        os.mkdir(path)
        if i == 50:
            offBranch = path

    # Add a single file at the bottom
    with open(path + "/bottom.txt", "w") as file:
        print("this is the file at the bottom of the chain", file=file)

    # Add a branch off using the path retrieved midway
    giantFolder = offBranch + "/giant"
    os.mkdir(giantFolder)

    randomWords = [
        "one",
        "longer",
        "common",
        "the",
        "it",
        "as",
        "gigantic",
        "123",
        "abc|defg",
    ]

    for i in range(2000):
        with open(giantFolder + f"file{i}.txt", "w") as file:
            print(f"File {i}: ", file=file)
            print("======", file=file)
            print(
                f"{random.choice(randomWords)} {random.choice(randomWords)}", file=file
            )

    # Add many 'top' level directories
    for i in range(100):
        string = f"test5/normal{i}"
        os.mkdir(string)
        for j in range(2):
            os.mkdir(string + f"/sub{j}")
            for k in range(2):
                os.mkdir(string + f"/sub{j}" + f"/subSub{k}")
                for num in range(15):
                    with open(
                        string + f"/sub{j}" + f"/subSub{k}" + f"/normal{num}.c", "w"
                    ) as file:
                        randString = "".join(
                            str(char)
                            for char in random.choices(
                                "abcdefghijklmnopqrstuvwxyz 012345", k=1000
                            )
                        )
                        print("int main() {", file=file)
                        print(f'char *string = "{randString}";', file=file)
                        print("}", file=file)
