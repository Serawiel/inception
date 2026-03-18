#ifndef LIFE_H
#define LIFE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_life
{
	int width;
	int height;
	int ite;
	char **board;
	char **newboard;
} t_life;

#endif