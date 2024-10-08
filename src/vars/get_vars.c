/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 00:40:42 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/22 02:01:17 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exist_masked(t_vars *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) && env->hide)
			return (1);
		env = env->next;
	}
	return (0);
}

int	exist_vars(t_vars *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) && !env->hide)
			return (1);
		env = env->next;
	}
	return (0);
}

void	delete_all_vars(t_vars **vars)
{
	t_vars	*vars_tmp;

	while (*vars)
	{
		vars_tmp = (*vars)->next;
		if ((*vars)->key)
			free((*vars)->key);
		if ((*vars)->value)
			free((*vars)->value);
		free(*vars);
		*vars = vars_tmp;
	}
}

void	update_vars(t_vars **env, char *key, char *value)
{
	t_vars	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
		}
		tmp = tmp->next;
	}
}

t_vars	*get_vars(t_vars **env, char *key)
{
	t_vars	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
