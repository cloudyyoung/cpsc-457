#!/bin/env python3

import os, sys, subprocess, shlex, collections, hashlib, mmap


def get_file_type(pth):
    ft = subprocess.check_output(["file", "-b", pth])
    ft = (ft.decode("utf-8").strip() + ",").split(",")[0]
    return ft


def dig(pth):
    digger = hashlib.sha256()
    with open(pth, "rb") as fd:
        while True:
            buff = fd.read(2 ** 16)
            if not buff:
                break
            digger.update(buff)
    return digger.hexdigest()


def update_whist(whist, fname):
    with open(fname, "rb") as fp:
        mm = fp.read()
        if len(mm) == 0:
            return
        wl = 0
        for i in range(len(mm)):
            c = mm[i]
            # capitalize
            if c >= 65 and c <= 90:
                c += 32
            if c < 97 or c > 122:
                # on non-alpha char update histogram and reset word
                if wl >= 3:
                    word = mm[i - wl : i].decode().lower()
                    whist[word] += 1
                wl = 0
            else:
                wl += 1
        # record last word
        if wl >= 3:
            i = len(mm)
            word = mm[i - wl : i].decode()
            whist[word] += 1
            wl = 0


def get_dir_stats(n, dname):
    all_files = collections.defaultdict(list)
    whist = collections.defaultdict(int)
    n_files = 0
    n_dirs = 0
    largest_file = ""
    largest_file_size = -1
    total_file_size = 0
    ftypes = collections.defaultdict(int)
    for root, dirs, files in os.walk(dname, followlinks=True):
        for file in files:
            # print("Processing", root, file)
            pth = os.path.join(root, file)
            try:
                all_files[dig(pth)].append(pth)
            except KeyboardInterrupt:
                raise
            except:
                print("Could not read file: ", pth)
                continue
            update_whist(whist, pth)
            s = os.path.getsize(pth)
            if s > largest_file_size:
                largest_file_size = s
                largest_file = pth
            total_file_size += s
            ft = get_file_type(pth)
            ftypes[ft] += 1
        n_dirs += len(dirs)
        n_files += len(files)

    print("--------------------------------------------------------------");
    print(f"Largest file:      \"{largest_file}\"")
    print(f"Largest file size: {largest_file_size}")
    print(f"Number of files:   {n_files}")
    print(f"Number of dirs:    {n_dirs}")
    print(f"Total file size:   {total_file_size}")

    ftypes = [(-e[1], e[0]) for e in ftypes.items()]
    ftypes = sorted(ftypes)
    ftypes = ftypes[:n]
    print("Most common file types:")
    for ft in ftypes:
        print(f' - {-ft[0]:4d}x "{ft[1]}"')

    whist = [(-e[1], e[0]) for e in whist.items()]
    whist = sorted(whist)
    whist = whist[:n]
    print("Most common words:")
    for w in whist:
        print(f" - {-w[0]:4d}x \"{w[1]}\"")

    all_files = sorted(all_files.items(), key=lambda x: len(x[1]), reverse=True)[:n]
    gcount = 1
    for grp in all_files:
        if len(grp[1]) < 2:
            break
        print(f"Duplicate files - group {gcount}")
        gcount += 1
        for f in grp[1]:
            print(f" - \"{f}\"")
    print("--------------------------------------------------------------");


def main(argv):
    if len(argv) != 3:
        print(f"Usage: {argv[0]} N dirname")
        sys.exit(-1)
    get_dir_stats(int(argv[1]), argv[2])


if __name__ == "__main__":
    main(sys.argv)