#ifndef BSQ_H
#define BSQ_H

#include <stdlib.h>
#include <stdio.h>

typedef struct s_map
{
	int nb_line;
	char empty;
	char full;
	char obstacle;
	char **map;
} t_map;

#endif