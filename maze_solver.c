#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "solver.h"

int main(int argc, char *argv[])
{
	char buf[1];
	
	printf("argv: %s\n", argv[1]);
	if (argc != 2) {
		printf("Only work with input file specified\n");
		exit(0);
	}

	FILE *fp = NULL;

	fp = fopen(argv[1], "r");

	if (fp == NULL) {
		printf("file is not found\n");
		exit(0);
	}

	char line[80];
	char* token;
	int row = 1;
   	int column = 1;

	fgets(line, sizeof(line), fp);

	printf("line: %s\n", line);

	token = strtok(line, " ");
	row = strtol(token, NULL, 10);
	token = strtok(NULL, " ");
	column = strtol(token, NULL, 10);

	printf("row: %d, column: %d\n", row, column);
	
	int* maze = (int *)malloc(row * column * sizeof(int));

	struct maze_param param = {0, 0, 0, 0, 0};
	param.row = row;
	param.column = column;

	char *c;
	int ret = 0;
	for (int i = 0; i < row; i++) {
		fgets(line, sizeof(line), fp);
		c = strtok(line, " ");
		for (int j = 0; j < column; j++) {
			if (strncmp(c, "S", 1) == 0) {
				*(maze + i*column + j) = 2;
				param.start_column = j;
				param.start_row = i;
				c = strtok(NULL, " ");
				continue;
			}
			if (strncmp(c, "E", 1) == 0) {
				*(maze + i*column + j) = 3;
				param.end = 3;
				c = strtok(NULL, " ");
				continue;
			}
			if (strncmp(c, ".", 1) == 0) {
				*(maze + i*column + j) = 0;
				c = strtok(NULL, " ");
				continue;
			}
			if (strncmp(c, "#", 1) == 0) {
				*(maze + i*column + j) = 1;
				c = strtok(NULL, " ");
				continue;
			}
		}
	}

	ret = solve_maze(maze, param);

	printf("calling solve_maze: %d\n", ret);

	free(maze);

    return 0;
}
