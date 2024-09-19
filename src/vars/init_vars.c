/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 00:07:39 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 22:46:07 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_vars(t_vars *new, t_vars **vars)
{
	t_vars	*tmp;

	tmp = *vars;
	if (!*vars)
		*vars = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static int	calculate_size(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	exist_vars(t_vars *vars, char *key)
{
	while (vars)
	{
		if (ft_strcmp(vars->key, key))
			return (1);
		vars = vars->next;
	}
	return (0);
}

static void	process_add(char *str, t_vars **vars)
{
	t_vars	*new;
	int		size_key;

	size_key = calculate_size(str);
	new = malloc(sizeof(t_vars));
	if (!new)
		return ;
	new->key = ft_strndup_range(str, 0, size_key - 1);
	if (!new->key)
		return ;
	new->value = ft_strndup_range(str, size_key + 1, ft_strlen(str));
	if (!new->value)
		return ;
	new->hide = FALSE;
	new->next = NULL;
	if (!exist_vars(*vars, new->key))
		add_vars(new, vars);
	else
	{
		free(new->key);
		free(new->value);
		free(new);
	}
}

void	init_hidden(t_vars **vars)
{
	t_vars	*new;

	new = malloc(sizeof(t_vars));
	if (!new)
		return ;
	new->key = ft_strdup("?");
	if (!new->key)
		return ;
	new->value = ft_strdup("0");
	if (!new->value)
		return ;
	new->hide = TRUE;
	new->next = NULL;
	if (!exist_vars(*vars, new->key))
		add_vars(new, vars);
	else
	{
		free(new->key);
		free(new->value);
		free(new);
	}
}

t_vars	*init_vars(char **envp)
{
	t_vars	*vars;
	int		i;

	vars = NULL;
	i = 0;
	while (envp[i])
		process_add(envp[i++], &vars);
	init_hidden(&vars);
	return (vars);
}
