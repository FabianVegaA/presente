#include "level.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

level *level_new(int size_x, int size_y)
{
    // Ask memory for the level struct
    level *lvl = malloc(sizeof(level));

    // Set the size
    lvl->size_x = size_x;
    lvl->size_y = size_y;

    // Ask memory for the cell* array
    lvl->cells = malloc(lvl->size_y * sizeof(char *));

    // Ask memory for each row
    for (int y = 0; y < lvl->size_y; y++)
    {
        lvl->cells[y] = malloc(lvl->size_x * sizeof(char));
    }

    // Initialize level
    for (int y = 0; y < lvl->size_y; y++)
    {
        for (int x = 0; x < lvl->size_x; x++)
        {
            lvl->cells[y][x] = '.';
        }
    }

    // Retreieve level's pointer
    return lvl;
}

int level_get(const level *lvl, int x, int y)
{
    // For cells outside the matrix:
    if (x < 0)
        return '#';
    if (y < 0)
        return '#';
    if (x >= lvl->size_x)
        return '#';
    if (y >= lvl->size_y)
        return '#';
    // Regular cells
    return lvl->cells[y][x];
}

void level_fill_random(level *lvl, int wall_size)
{
    // Check that the wall_size is smaller than the level
    assert(wall_size < lvl->size_x);
    assert(wall_size < lvl->size_y);
    // Compute a good number of walls so that up to 1/10 of the level is filled
    int n_walls = lvl->size_x * lvl->size_y / wall_size / 10;
    // Add each of the walls
    for (int i = 0; i < n_walls; i++)
    {
        int vert = rand() % 2; // vertical or horizontal?
        if (vert)
        {
            // Initial wall position
            int wx = rand() % (lvl->size_x);
            int wy = rand() % (lvl->size_y - wall_size);
            // Iterate to place wall vertically
            for (int k = 0; k < wall_size; k++)
            {
                lvl->cells[wy + k][wx] = '#';
            }
        }
        else
        {
            // Initial wall position
            int wx = rand() % (lvl->size_x - wall_size);
            int wy = rand() % (lvl->size_y);
            // Iterate to place wall horizontally
            for (int k = 0; k < wall_size; k++)
            {
                lvl->cells[wy][wx + k] = '#';
            }
        }
    }
}

void noise_map(level *lvl, float chanceToStartAlive){
    srand(getpid());
    
    for(int x = 0; x < lvl->size_y; x++){
        for(int y = 0; y < lvl->size_x; y++){
            float random = rand() % 100;
            if(random/100 < chanceToStartAlive){
                lvl->cells[x][y] = '#';
            }
        }
    }  
}

void doIterations(level *lvl, int numberOfSteps, int birthLimit, int deathLimit)
{
    if(numberOfSteps > 0){
        for (int iter = 0; iter < numberOfSteps; iter++)
        {     
            doSimulationStep(lvl, birthLimit, deathLimit);
        }
    }

}

void doSimulationStep(level *lvl, int birthLimit, int deathLimit)
{   
    // Initialize new level
    level *new_lvl = level_new(lvl->size_x, lvl->size_y);
    int neighbour;

    //Do apply changes from old level into new level
    for (int x = 0; x < lvl->size_y; x++)
    {
        for (int y = 0; y < lvl->size_x; y++)
        {
            neighbour = countAliveNeighbours(lvl, x, y);

            if(lvl->cells[x][y] == '#'){
                if(neighbour < deathLimit){
                    new_lvl->cells[x][y] = '#';
                }
                else{
                    new_lvl->cells[x][y] = '.';
                }
            }
            else{
                if(neighbour < birthLimit){
                    new_lvl->cells[x][y] = '.';
                }
                else{
                    new_lvl->cells[x][y] = '#';
                }
            }
        }
    }
    // Changes are transferred to level
    CopyPaste(lvl, new_lvl);
    
    // Free new_level of the memory
    level_free(new_lvl);
}

void CopyPaste(level *lvl, level *new_lvl){
    for (int i = 0; i < new_lvl->size_y; i++){
        for (int j = 0; j < new_lvl->size_x; j++){
            lvl->cells[i][j] = new_lvl->cells[i][j];
        }
    }
}

int countAliveNeighbours(level *lvl, int x, int y)
{
    int neighbour = 0;

    //The neighbour of (x,y)
    for (int dir_x = -1; dir_x <= 1; dir_x++)
    {
        for (int dir_y = -1; dir_y <= 1; dir_y++)
        {
            //This searching in his eights neighbour
            //The edges of the level isn't taken into account
            if (x + dir_x <= lvl->size_x && x + dir_x >= 0 && y + dir_y <= lvl->size_y && y + dir_y >= 0)
            {
                if (lvl->cells[x + dir_x][y + dir_y] == '#')
                {
                    neighbour += 1;
                }
            }
        }
    }
    return neighbour;
}

// Free level memory
void level_free(level *lvl)
{
    // Free each row first
    for (int y = 0; y < lvl->size_y; y++)
    {
        free(lvl->cells[y]);
    }
    // Free char* array
    free(lvl->cells);
    // Free the level
    free(lvl);
}

void print_cells(level *lvl)
{
    printf("%d x %d\n", lvl->size_x, lvl->size_y);

    for (int i = 0; i < lvl->size_y; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < lvl->size_x; j++)
        {
            printf("%c", lvl->cells[i][j]);
        }
        printf("\n");
    }
}
