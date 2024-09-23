/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:48:53 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 01:01:20 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_value(char **splited_path, char *full_path, t_minishell *data)
{
	int	i;

	i = 0;
	while (splited_path[i])
	{
		full_path = build_full_path(splited_path[i],
				data->current_tokens->value);
		if (!full_path)
		{
			free_split(splited_path);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_split(splited_path);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(splited_path);
	return (NULL);
}

char	*get_path(t_minishell *data)
{
	char	**splited_path;
	char	*full_path;
	t_vars	*tmp;

	full_path = NULL;
	tmp = get_vars(&data->env, "PATH");
	if (!tmp)
		return (NULL);
	splited_path = ft_split(tmp->value, ':');
	if (!splited_path)
		return (NULL);
	return (get_value(splited_path, full_path, data));
}
