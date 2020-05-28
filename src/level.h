#ifndef LEVEL_H
#define LEVEL_H

#define TILE_SIZE 48

#define SIZE_X 50
#define SIZE_Y 40


/* A level is a matrix of cells, each cell is represented using chars.
'.' = free cells
'#' = walls */
typedef struct {
    int size_x, size_y;
    char **cells;
} level;

// Create a new level with the given size, allocates memory for it.
level *level_new(int size_x, int size_y);

// Retrieves the cell at a given position, can be used for cells outside the board.
int level_get(const level *lvl, int x, int y);

// Put randomly placed walls of the given size in the level.
void level_fill_random(level *lvl, int wall_size);

// Iterate over lvl using doSimulate.
void doIterations(level *lvl, int numberOfSteps, int birthLimit, int deathLimit);

// Simulate the Steps with rules from Game of Life, changing lvl.
void doSimulationStep(level *lvl, int deathLimit, int birthLimit);

//Count the number of neighbour, touring the neighbour
int countAliveNeighbours(level *lvl, int x, int y);

// Destroy a level, free its memory.
void level_free(level *lvl);

// This is function only for testing, it touring a matrix of characters to printing of each position.
void print_cells(level * lvl);

// Takes the data from new_lvl and change are transferred to lvl
void CopyPaste(level *lvl, level *new_lvl);

//Inicialize map randomly geting a noise map.
void noise_map(level *lvl, float chanceToStartAlive);

#endif
