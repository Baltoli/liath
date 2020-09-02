#!/usr/bin/env python

import pandas as pd
import sys

def closest(spaces):
    def closest_impl(val):
        min_diff = None
        min_space = None

        for name in ['stack', 'heap']:
            base = spaces[name]
            diff = abs(val - base)
            if min_diff is None or diff < min_diff:
                min_diff = diff
                min_space = name

        return min_space
    return closest_impl

def relative(spaces):
    def relative_impl(row):
        return [
            spaces['stack'] - row.addr if row.space == "stack" else row.addr - spaces['heap'],
            spaces['program'] - row.pc
        ]
    return relative_impl

def main(in_f):
    df = pd.read_csv(in_f)

    max_addr = df["addr"].max()
    min_addr = df["addr"].min()
    max_pc = df["pc"].max()

    spaces = {
        'heap': min_addr,
        'stack': max_addr,
        'program': max_pc
    }

    df["space"] = df["addr"].apply(closest(spaces))
    df[["addr", "pc"]] = df[["addr", "space", "pc"]].apply(relative(spaces), axis=1, result_type='expand')
    print(df.to_csv())

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} data")
    else:
        main(sys.argv[1])
