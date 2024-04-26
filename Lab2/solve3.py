#!/usr/bin/python3
import os, sys
parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(parent_dir)
from helper import Program


# TODO: Complete this function (do not touch anything else).
def solve():
    prog = Program("./problem3.bin")
    print(prog.read_line()) # Read the initial message of the program.
    prog.send_line("3")
    print(prog.read_line())
    prog.send_line("7")
    print(prog.read_line())
    prog.send_line("0")
    print(prog.read_line())


if __name__ == "__main__":
    solve()
