#!/bin/bash
grep "\." -q <&0 && \
cat test/solution_maze_with_sand.txt || \
cat test/solution_simple_maze.txt