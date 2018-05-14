#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int *maze_p = NULL;
static int row;
static int column;

void dump_maze(int *maze) {
	for (int i = 0; i< row; i++) {
		for (int j = 0; j < column; j++){
			printf("%d ", *(maze + i * column + j));
		}
		printf("\n");
	}
	printf("------------------\n");
}

void dump_list(Step* head) {
	Step *iterator = head;
	while (iterator != NULL) {
		printf("Step: %d, %d\n", iterator->current_x,  iterator->current_y);
		iterator = iterator->next;
	}
}

bool check_wall(int r, int c) {
	//printf("value (%d %d): %d\n", r, c,  *(maze_p + r * column + c));
	if (r < 0 || r >= row || c < 0 || c >= column) {
		return true;
	}
	
	return (*(maze_p + r * column + c) == 1);
}

bool check_result(int r, int c){
	return (*(maze_p + r * column + c) == 3);
}

int determine_next_action(int r, int c, int* solution) {
	int list[4] = {0, 0, 0, 0};

	if (check_wall(r, c+1)) {
		list[0] = 50;
	} else {
		list[0] = *(solution + r * column + c + 1);
	}
	if (check_wall(r+1, c)) {
		list[1] = 50;
	} else {
		list[1] = *(solution + (r+1) * column + c);
	}
	if (check_wall(r, c-1)) {
		list[2] = 50;
	} else {
		list[2] = *(solution + r * column + c - 1);
	}
	if (check_wall(r-1, c)) {
		list[3] = 50;
	} else {
		list[3] = *(solution + (r-1) * column + c);
	}
	int index = 0;
	for (int i = 0; i < 4; i++) {
		if (list[i] < list[index]) {
			index = i;
		}
	}

	return index + 1;
}

bool explore_next(int r, int c, int* solution, int* action_s) {
	if (check_result(r, c)) {
		*(solution + r * column + c) = 100;
		return true;
	}

	if (check_wall(r, c)) {
		return false;
	}

	*(solution + r * column + c) += 1;

#if 0
	printf("solution\n");
	dump_maze(solution);
	printf("action solution\n");
	dump_maze(action_s);
#endif
	int action = determine_next_action(r, c, solution);
	*(action_s + r * column + c) = action;
		//printf("at (%d, %d), action %d\n", r, c, action);
		switch (action) {
			case 1:
				if (explore_next(r, c + 1, solution, action_s)) {
					return true;
				}
				break;
			case 2:
				if (explore_next(r + 1, c, solution, action_s)) {
					return true;
				} 
				break;
			case 3:
				if (explore_next(r, c - 1, solution, action_s)) {
					return true;
				}
				break;
			case 4:
				if (explore_next(r - 1, c, solution, action_s)) {
					return true;
				} 
				break;
			default:
				return false;
		}

		if (explore_next(r, c, solution, action_s)) {
			return true;
		} else 
			return false;
}

void process_solution(int start_r, int start_c, int* action_s) {

	int *tmp_solution = (int *) malloc(row*column*sizeof(int));

	memset(tmp_solution, 0, row*column*sizeof(int));

	*(tmp_solution + start_r * column + start_c) = 2;

	int node = *(maze_p + start_r * column + start_c);
	int new_r = start_r;
	int new_c = start_c;

	while (node != 100)  {

		int act = *(action_s + new_r * column + new_c);

		printf("action(%d, %d): %d\n", new_r, new_c, act);

		switch (act) {
			case 1:
				new_c += 1;
				break;
			case 2:
				new_r += 1;
				break;
			case 3:
				new_c -= 1;
				break;
			case 4:
				new_r -= 1;
				break;
			default:
				node = 100;
				break;
		}

		if (node == 100) {
			*(tmp_solution + new_r *column + new_c) = 8;
			break;
		}

		node = *(maze_p + new_r * column + new_c);
		*(tmp_solution + new_r *column + new_c) = 1;

	}

	dump_maze(tmp_solution);

	free(tmp_solution);
}

int solve_maze(int *maze, maze_param param) {

	printf("param row: %d, column: %d, start_column: %d\n", param.row, param.column, param.start_column);

	maze_p = maze;
	row = param.row;
	column = param.column;
	
	dump_maze(maze_p);

	int *solution = (int *) malloc(row * column * sizeof(int));

	memset(solution, 0, row*column*sizeof(int));

	int *action_solution = (int *) malloc(row*column*sizeof(int));

	memset(action_solution, 0, row*column*sizeof(int));
#if 0
	Step * a = (Step*)malloc(sizeof(Step));
	a->current_x = 0;
	a->current_y = 0;
	a->next = NULL;

	Step * b = (Step*)malloc(sizeof(Step));
	b->current_x = 1;
	b->current_y = 1;
	b->next = NULL;

	update_step(a);
	update_step(b);
	printf("-----\n");
	dump_list(head);
	update_step(a);
	printf("-----\n");
	dump_list(head);

	update_step(b);
	printf("-----\n");
	dump_list(head);
#endif

	printf("action: %d\n", explore_next(param.start_row, param.start_column, solution, action_solution));

	dump_maze(solution);

	dump_maze(action_solution);

	process_solution(param.start_row, param.start_column, action_solution);

	free(solution);
	free(action_solution);

	return 10;
}


