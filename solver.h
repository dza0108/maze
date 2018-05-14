#ifndef SOLVER_H_
#define SOLVER_H_

typedef struct maze_param {
	int row;
	int column;
	int start_column;
	int start_row;
	int end;
} maze_param;

typedef enum OPS {
	NO = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,
	DONE = 5,
	UNKNOWN = 6
} OPS;

typedef struct Step {
	int current_x;
	int current_y;
	enum OPS action;
	struct Step* next;
} Step;

int solve_maze(int *maze, maze_param param);

#endif /* SOLVER_H_ */
