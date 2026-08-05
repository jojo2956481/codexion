/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebeyssa <lebeyssa@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:59:21 by lebeyssa          #+#    #+#             */
/*   Updated: 2026/08/05 14:12:24 by lebeyssa         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_data(char	**datav, t_data *data)
{
	int			i;
	long long	num;
	int			y;

	i = 1;
	while (i < 8)
	{
		num = 0;
		y = 0;
		while (datav[i][y])
		{
			if (ft_isdigit(datav[i][y]) == 0)
				return (-1);
			y++;
		}
		num = ft_atoi(datav[i]);
		if (num < 0 || num > INT_MAX)
			return (-2);
		i++;
	}
	i = init_data(datav, data);
	if (i < 0)
		return (i);
	return (0);
}

int	main(int datac, char **datav)
{
	t_data	data;
	int		error;

	error = 0;
	if (datac == 9)
	{
		error = parse_data(datav, &data);
		if (error < 0)
		{
			code_error_parsing(error);
			return (1);
		}
		error = manage_simu(&data, error);
		if (error < 0)
		{
			code_error_simulation(error);
			return (1);
		}
		return (0);
	}
	printf("Error: enter datauments\n");
	return (1);
}
