#!/usr/bin/env python

import pandas as pd
import sys

def from_hex(s):
    return int(s, 16)

# Use the heuristic check that we won't use 40 TB of memory, so the address is
# on the stack if it's closer to the top than the bottom and vice-versa.
def relative_addr(addr, stack, heap):
    if abs(stack - addr) < abs(heap - addr):
        return ('stack', stack - addr)
    else:
        return ('heap', addr - heap)

def main(in_f):
    df = pd.read_csv(in_f, converters = { 'addr' : from_hex })
    stack_top = max(df.addr)
    heap_bottom = min(df.addr)
    df['section'], df['offset'] = df.addr.apply(relative_addr, args=(stack_top, heap_bottom)).str
    print(df.to_csv(index=False))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} data")
    else:
        main(sys.argv[1])
