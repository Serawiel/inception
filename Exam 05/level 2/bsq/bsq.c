#include "bsq.h"

void	free_map(char **map, int nline)
{
	int	i;

	i = 0;
	while (i < nline)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int	clean_exit(char *line, FILE *stream, t_map *map, int nline)
{
	if (line)
		free(line);
	if (map->map)
		free_map(map->map, nline);
	if (stream)
		fclose(stream);
	printf("Error: invalid map\n");
	return (-1);
}

int	is_printable(char c)
{
	return (c >= 32 && c <= 126);
}

void	skip_spaces(char *line, int *i)
{
	while (line[*i] == ' ')
		(*i)++;
}

int	get_first_line(char *line, t_map *map)
{
	int	i;

	i = 0;
	skip_spaces(line, &i);
	if (line[i] < '0' || line[i] > '9')
		return (-1);
	map->nb_line = 0;
	while (line[i] >= '0' && line[i] <= '9')
		map->nb_line = map->nb_line * 10 + (line[i++] - '0');
	if (map->nb_line <= 0)
		return (-1);
	skip_spaces(line, &i);
	if (!is_printable(line[i]))
		return (-1);
	map->empty = line[i++];
	skip_spaces(line, &i);
	if (!is_printable(line[i]) || line[i] == map->empty)
		return (-1);
	map->obstacle = line[i++];
	skip_spaces(line, &i);
	if (!is_printable(line[i]) || line[i] == map->empty
		|| line[i] == map->obstacle)
		return (-1);
	map->full = line[i++];
	skip_spaces(line, &i);
	if (line[i] != '\n' && line[i] != '\0')
		return (-1);
	return (0);
}

int	min(int a, int b, int c)
{
	if (a < b)
	{
		if (a < c)
			return (a);
		else
			return (c);
	}
	else
	{
		if (b < c)
			return (b);
		else
			return (c);
	}
}

void	solve_bsq(t_map map)
{
	char	**calc;
	int		size;
	int		col;
	int		line;
	int		max;
	int		linemax;
	int		colmax;

	size = 0;
	col = 0;
	line = 0;
	max = 0;
	linemax = 0;
	colmax = 0;
	calc = calloc(map.nb_line, sizeof(char *));
	while (map.map[0][size])
		size++;
	for (int i = 0; i < map.nb_line; i++)
	{
		calc[i] = calloc(size, sizeof(char));
	}
	while (line < map.nb_line)
	{
		while (map.map[line][col])
		{
			if (map.map[line][col] == map.obstacle)
				calc[line][col] = 0;
			else if (line == 0 || col == 0)
				calc[line][col] = 1;
			else
			{
				calc[line][col] = min(calc[line - 1][col - 1], calc[line][col
						- 1], calc[line - 1][col]) + 1;
				if (calc[line][col] > max)
				{
					max = calc[line][col];
					linemax = line;
					colmax = col;
				}
			}
			col++;
		}
		col = 0;
		line++;
	}
	free_map(calc, map.nb_line);
	for (int i = linemax - max + 1; i <= linemax; i++)
	{
		for (int j = colmax - max + 1; j <= colmax; j++)
			map.map[i][j] = map.full;
	}
	for (int i = 0; i < map.nb_line; i++)
	{
		printf("%s\n", map.map[i]);
	}
}

int	main(int ac, char **av)
{
	FILE	*stream;
	char	*line;
	size_t	size;
	ssize_t	bnread;
	ssize_t	nread;
	t_map	map;
	int		nline;

	bnread = 0;
	nline = 0;
	line = NULL;
	size = 0;
	map.map = NULL;
	if (ac == 0 || ac > 2)
	{
		printf("Error: Usage: <file>\n");
		return (-1);
	}
	if (ac == 1)
		stream = stdin;
	if (ac == 2)
		stream = fopen(av[1], "r");
	if (stream == NULL)
	{
		printf("Error : open file.\n");
		return (-1);
	}
	nread = getline(&line, &size, stream);
	if (nread == -1 || get_first_line(line, &map) == -1)
		return (clean_exit(line, stream, &map, nline));
	map.map = calloc(map.nb_line, sizeof(char *));
	if (!map.map)
		return (clean_exit(line, stream, &map, nline));
	while ((nread = getline(&line, &size, stream)) != -1)
	{
		if (nread < 2 || (nread != bnread && (bnread != 0)) || (line[nread
				- 1] != '\n'))
			return (clean_exit(line, stream, &map, nline));
		for (int i = 0; i < nread - 1; i++)
		{
			if (line[i] != map.empty && line[i] != map.obstacle)
				return (clean_exit(line, stream, &map, nline));
		}
		map.map[nline] = calloc(nread, sizeof(char));
		if (!map.map[nline])
			return (clean_exit(line, stream, &map, nline));
		for (int i = 0; i < nread - 1; i++)
			map.map[nline][i] = line[i];
		map.map[nline][nread - 1] = '\0';
		nline++;
		if (nline > map.nb_line)
			return (clean_exit(line, stream, &map, nline));
		bnread = nread;
	}
	if (nline < map.nb_line)
		return (clean_exit(line, stream, &map, nline));
	solve_bsq(map);
	free(line);
	free_map(map.map, nline);
	fclose(stream);
	return (0);
}
