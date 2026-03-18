#include "life.h"

void	fputstr(char *str)
{
	for (int i = 0; str[i]; i++)
		putchar(str[i]);
}

int	parse_arg(t_life *life, char **av)
{
	for (int i = 1; av[i]; i++)
	{
		for (int j = 0; av[i][j]; j++)
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (-1);
		}
	}
	life->width = atoi(av[1]);
	life->height = atoi(av[2]);
	life->ite = atoi(av[3]);
	return (0);
}

void	print_board(t_life life)
{
	for (int i = 0; i < life.height; i++)
	{
		fputstr(life.board[i]);
		putchar('\n');
	}
}

void	free_board(t_life life)
{
	for (int i = 0; i < life.height; i++)
	{
		free(life.board[i]);
	}
	free(life.board);
}

void	free_new_board(t_life life)
{
	for (int i = 0; i < life.height; i++)
	{
		free(life.newboard[i]);
	}
	free(life.newboard);
}

int	create_board(t_life *life)
{
	char	c;
	int		line;
	int		col;
	int		write;

	life->board = NULL;
	line = 0;
	col = 0;
	write = 0;
	life->board = calloc(life->height, sizeof(char *));
	if (!life->board)
	{
		free_board(*life);
		return (-1);
	}
	for (int i = 0; i < life->height; i++)
	{
		life->board[i] = calloc(life->width + 1, sizeof(char));
		if (!life->board[i])
		{
			free_board(*life);
			return (-1);
		}
		for (int j = 0; j < life->width; j++)
			life->board[i][j] = ' ';
		// life->board[i][life->width] = '\0';
	}
	while (read(0, &c, 1) > 0)
	{
		if (c == 'a' && col > 0)
			col--;
		if (c == 'd' && col < life->width - 1)
			col++;
		if (c == 'w' && line > 0)
			line--;
		if (c == 's' && line < life->height - 1)
			line++;
		if (c == 'x')
		{
			write = !write;
		}
		if (write == 1)
			life->board[line][col] = 'O';
	}
	return (0);
}

int	count_neighbors(t_life life, int col, int line)
{
	int	count;

	count = 0;
	for (int i = line - 1; i <= line + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			if (i == line && j == col)
				continue ;
			if (i >= 0 && i < life.height && j >= 0 && j < life.width)
			{
				if (life.board[i][j] == 'O')
					count++;
			}
		}
	}
	return (count);
}

void	game(t_life *life)
{
	life->newboard = calloc(life->height, sizeof(char *));
	if (!life->newboard)
	{
		free_new_board(*life);
		return ;
	}
	for (int i = 0; i < life->height; i++)
	{
		life->newboard[i] = calloc(life->width, sizeof(char));
		if (!life->newboard[i])
		{
			free_new_board(*life);
			return ;
		}
		for (int j = 0; j < life->width; j++)
		{
			if (life->board[i][j] == ' ')
			{
				if (count_neighbors(*life, j, i) == 3)
					life->newboard[i][j] = 'O';
				else
					life->newboard[i][j] = ' ';
			}
			else
			{
				if (!(count_neighbors(*life, j, i) == 3
						|| count_neighbors(*life, j, i) == 2))
					life->newboard[i][j] = ' ';
				else
					life->newboard[i][j] = 'O';
			}
		}
	}
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			life->board[i][j] = life->newboard[i][j];
		}
	}
	free_new_board(*life);
}

int	main(int ac, char **av)
{
	t_life life;

	if (ac != 4)
		return (-1);
	if (parse_arg(&life, av) == -1)
	{
		fputstr("Error: invalid args\n");
		return (-1);
	}
	if (create_board(&life) == -1)
	{
		fputstr("Error: invalid board\n");
		return (-1);
	}
	if (life.ite > 0)
	{
		for (int i = 0; i < life.ite; i++)
			game(&life);
	}
	print_board(life);
	free_board(life);
	return (0);
}