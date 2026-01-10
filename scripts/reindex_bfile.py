#!/usr/bin/env python3
import argparse
import sys

def reindex_bfile(inp, out, start):
    comments = []
    values = []

    for line in inp:
        s = line.rstrip("\n")
        if not s:
            continue
        if s.lstrip().startswith("#"):
            comments.append(s)
        else:
            try : 
                _, v = s.split()
                values.append(v)
            except : 
                print("ERROR ON LINE")
                print(s)
                exit(1)

    for c in comments:
        out.write(c + "\n")
    for i, v in enumerate(values, start):
        out.write(f"{i} {v}\n")
    out.write("\n")

def main():
    p = argparse.ArgumentParser()
    p.add_argument("input")
    args = p.parse_args()

    with open(args.input, "r") as inp:
        reindex_bfile(inp, sys.stdout, 1)

if __name__ == "__main__":
    main()
