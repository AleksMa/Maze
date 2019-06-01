#!/bin/bash
# It is BASH test script

walk_out_of_maze="$@"

tmp=`mktemp`

echo -n Simple maze " "
echo "cat test/simple_maze.txt | $walk_out_of_maze > $tmp" | sh

{ \
	diff $tmp test/solution_simple_maze.txt || \
	diff $tmp test/solution2_simple_maze.txt
} && \
echo -e "\033[32mOK\033[m" || \
{ \
	echo failed, wrong way
	cat $tmp
	rm $tmp
	exit 1
}

echo -n Maze with sand " "
echo "cat test/maze_with_sand.txt | $walk_out_of_maze > $tmp" | sh

diff $tmp test/solution_maze_with_sand.txt && \
echo -e "\033[32mOK\033[m" || \
{ \
	echo failed, wrong way
	cat $tmp
	rm $tmp
	exit 1
}

rm $tmp
echo -e "\033[32mAll passed\033[m"