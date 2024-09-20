/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:17:13 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 16:17:42 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_count_vars(t_vars *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (!env->hide)
			i++;
		env = env->next;
	}
	return (i);
}

char	**get_env(t_vars *env)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (get_count_vars(env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env)
	{
		if (!env->hide)
		{
			result[i] = malloc(sizeof(char) * (ft_strlen(env->key)
						+ ft_strlen(env->value) + 2));
			if (!result[i])
				return (NULL);
			ft_strcpy(result[i], env->key);
			ft_strcat(result[i], "=");
			ft_strcat(result[i], env->value);
			i++;
		}
		env = env->next;
	}
	result[i] = NULL;
	return (result);
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		total_len;

	total_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * total_len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}
