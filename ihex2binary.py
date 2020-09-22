#!/usr/bin/env python3

import intelhex
import sys


assert len(sys.argv) == 2


fname = sys.argv[1];

ih = intelhex.IntelHex16bit(fname)

for si,segment in enumerate(ih.segments()):

    start = segment[0]
    end = segment[1]
    nbytes = end-start
    assert(nbytes%2 == 0)

    nwords = nbytes//2
    start_word = start//2
    end_word = end//2


    if start_word == 0x2007:
        segment_type = "Configuration word"
    elif start_word >= 0x2100:
        segment_type = "Data segment"
    else:
        segment_type = "Program segment"

    print("---------------------------------------------------------------------------")
    print("{} at address 0x{:04x} of length {}".format(segment_type, start_word, nwords))

    words = [ih[wa] for wa in range(start_word,end_word)]

    if start_word >= 0x2100:
        rowwidth = 12
        for rowstart in range(0, nwords, rowwidth):
            rowend = min(rowstart+rowwidth, nwords)
            row = " ".join("0x{:02x},".format(w) for w in words[rowstart:rowend])
            print("   ", row)

    else:
        rowwidth = 12
        for rowstart in range(0, nwords, rowwidth):
            rowend = min(rowstart+rowwidth, nwords)
            row = " ".join("0x{:04x},".format(w) for w in words[rowstart:rowend])
            print("   ", row)
