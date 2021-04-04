#!/bin/env python3

# ==========================================================================
# very inefficient implementation of the memsim assignment
# (c) Pavol Federl, 2021
# ==========================================================================

import sys, os, time

debug = True


class Table:
    def __init__(self, max_col_width=None) -> None:
        self.rows = []
        self.curr_row = {}
        self.max_widths = {}
        self.n_cols = 0
        self.col_align = {}
        self.default_max_col_width = max_col_width
        self.max_col_widths = {}

    def get_max_col_width(self, col):
        return self.max_col_widths.get(col, self.default_max_col_width)

    def set_max_col_width(self, col, width):
        self.max_col_widths[col] = width

    def new_row(self, sep: str = None):
        assert sep is None or len(sep) == 1
        if len(self.curr_row) > 0:
            self.rows.append(self.curr_row)
        self.curr_row = {}
        if sep is not None:
            self.rows.append(sep)
        return self

    # txt can be string with new lines, or array of strings with newlines
    def add_to_col(self, column: int, txt: str):
        assert isinstance(txt, str)
        # update number of columns
        self.n_cols = max(self.n_cols, column + 1)
        # make sure entry for column exists
        self.curr_row[column] = self.curr_row.get(column, [])
        # make all lines shorter
        lines = txt.splitlines()
        if lines == []:
            lines = [""]
        shortlines = []
        mcw = self.get_max_col_width(column)
        for line in lines:
            while mcw is not None and len(line) > mcw:
                shortlines.append(line[:mcw])
                line = line[mcw:]
            shortlines.append(line)
        # add each short line to the column
        for line in shortlines:
            self.curr_row[column].append(line)
            self.max_widths[column] = max(self.max_widths.get(column, 0), len(line))
        return self

    def set_col_align(self, col, align="left"):
        assert align in ["left", "right", "center"]
        self.col_align[col] = align
        return self

    def print(self):
        print(self.to_str(), end="")

    def to_str(self):
        self.new_row()
        res = ""
        for row in self.rows:
            res += self.row_to_str(row)
        return res

    def row_to_str(self, row):
        if isinstance(row, str):
            res = ""
            for col in range(self.n_cols):
                self.max_widths[col] = self.max_widths.get(col, 0)
                res += row * (2 + self.max_widths[col])
                if col < self.n_cols - 1:
                    res += "+"
            res += "\n"
            return res
        n_sub_rows = 0
        for c in range(self.n_cols):
            row[c] = row.get(c, [])
            n_sub_rows = max(n_sub_rows, len(row[c]))
        res = ""
        for sr in range(n_sub_rows):
            for col in range(self.n_cols):
                self.max_widths[col] = self.max_widths.get(col, 0)
                align = self.col_align.get(col, "left")
                if sr < len(row[col]):
                    subrow = row[col][sr]
                else:
                    subrow = ""
                assert len(subrow) <= self.max_widths[col], f"subrow={subrow}, col={col}"
                pad = self.max_widths[col] - len(subrow)
                if align == "left":
                    res += " " + subrow + " " + (" " * pad)
                elif align == "right":
                    res += (" " * pad) + " " + subrow + " "
                else:
                    res += (" " * (pad // 2)) + " " + subrow + " " + (" " * (pad - pad // 2))
                if col < self.n_cols - 1:
                    res += "|"
            res += "\n"
        return res


class Part:
    def __init__(self, tag, size, addr) -> None:
        self.tag = tag
        self.size = size
        self.addr = addr

    def __repr__(self) -> str:
        return f"({self.tag},{self.size} @{self.addr})"


class Memsim:
    def __init__(self, page_size) -> None:
        self.parts = []
        self.page_size = page_size
        self.start_time = time.time()

    def alloc(self, tag, size):
        debug and print(f"alloc {tag} {size}")
        max_ind, found = 0, False
        for i in range(len(self.parts)):
            p = self.parts[i]
            if p.tag >= 0 or p.size < size:
                continue
            if found and p.size <= self.parts[max_ind].size:
                continue
            max_ind = i
            found = True
        if not found:
            self.parts.append(
                Part(
                    -1, 0, 0 if len(self.parts) == 0 else self.parts[-1].addr + self.parts[-1].size
                )
            )
            self.merge()
            while self.parts[-1].size < size:
                self.parts[-1].size += self.page_size
            max_ind = len(self.parts) - 1
        assert max_ind < len(self.parts)
        assert self.parts[max_ind].tag < 0
        assert self.parts[max_ind].size >= size
        self.parts[max_ind].tag = tag
        if self.parts[max_ind].size == size:
            return
        p = Part(-1, self.parts[max_ind].size - size, self.parts[max_ind].addr + size)
        self.parts.insert(max_ind + 1, p)
        self.parts[max_ind].size = size
        self.parts[max_ind].tag = tag

    def free(self, tag):
        debug and print(f"free {tag}")
        for p in self.parts:
            p.tag = -1 if p.tag == tag else p.tag
        self.merge()

    def merge(self):
        newp = []
        for p in self.parts:
            if p.tag < 0 and len(newp) > 0 and newp[-1].tag < 0:
                newp[-1].size += p.size
            else:
                newp.append(p)
        self.parts = newp

    def show_partitions(self):
        if not debug:
            return
        t = Table()
        t.new_row("-")
        t.add_to_col(0, "tag\nsize\naddr")
        t.set_col_align(0, "right")
        col = 1
        for p in self.parts:
            t.add_to_col(col, str(p.tag))
            t.add_to_col(col, str(p.size))
            t.add_to_col(col, str(p.addr))
            t.set_col_align(col, "center")
            col += 1
        t.new_row("-")
        print("  " + "\n  ".join(t.to_str().splitlines()))

    def report(self):
        elapsed = time.time() - self.start_time
        max_addr = sum(p.size for p in self.parts)
        nreq = (max_addr + self.page_size - 1) // self.page_size
        max_size, max_addr = 0, 0
        for p in self.parts:
            if p.tag < 0 and max_size < p.size:
                max_size, max_addr = p.size, p.addr

        print("\n----- Results ---------------------------------")
        print(f"pages requested:                {nreq}")
        print(f"largest free partition size:    {max_size}")
        print(f"largest free partition address: {max_addr}")
        print(f"elapsed time:                   {elapsed:.3f}")


def main(argv):
    try:
        if len(argv) != 2:
            raise "!@*!(@*&!@#"
        page_size = int(argv[1])
    except:
        print(f"Usage: {argv[0]} page-size")
        sys.exit(-1)

    global debug
    if page_size > 0:
        debug = True
    else:
        page_size = -page_size
        debug = False

    msim = Memsim(page_size)
    for line in sys.stdin:
        if line.isspace():
            continue
        toks = line.split()
        assert len(toks) in [1, 2]
        toks = tuple(int(e) for e in toks)
        if toks[0] < 0:
            msim.free(-toks[0])
        else:
            msim.alloc(toks[0], toks[1])
        msim.show_partitions()
    msim.report()


if __name__ == "__main__":
    main(sys.argv)
