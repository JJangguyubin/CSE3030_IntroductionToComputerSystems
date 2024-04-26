#!/usr/bin/python3
import os, sys
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(parent_dir)
from helper import Program


# TODO: Complete this function (do not touch anything else).
def solve():
    prog = Program("./problem2.bin")
    print(prog.read_line()) # Read the initial message of the program.
    prog.send_line("80 400 560")
    print(prog.read_line())

if __name__ == "__main__":
    solve()
