/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 04:02:15 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 19:39:56 by jalbiser         ###   ########.fr       */
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

static int	process_delete(t_vars **env, t_vars **delete)
{
	t_vars	*current;
	t_vars	*previous;

	if (*env == *delete)
	{
		current = *env;
		*env = current->next;
	}
	else
	{
		previous = *env;
		current = (*env)->next;
		while (current && current != *delete)
		{
			previous = current;
			current = current->next;
		}
		if (!current)
			return (0);
		previous->next = current->next;
	}
	return (1);
}

void	up_shlvl(t_vars **env)
{
	t_vars	*new;
	char	*tmp;
	int		i;

	if (get_vars(env, "SHLVL"))
	{
		i = ft_atoi(get_vars(env, "SHLVL")->value);
		i++;
		tmp = ft_itoa(i);
		update_vars(env, "SHLVL", tmp);
		free(tmp);
	}
	else
	{
		new = malloc(sizeof(t_vars));
		if (!new)
			return ;
		new->value = ft_strdup("SHLVL");
		new->key = ft_strdup("0");
		new->next = NULL;
		add_vars(new, env);
	}
}

int	delete_vars(t_vars **env, t_vars *delete)
{
	if (!env || !*env || !delete)
		return (0);
	if (!process_delete(env, &delete))
		return (0);
	if (delete->key)
		free(delete->key);
	if (delete->value)
		free(delete->value);
	free(delete);
	return (1);
}
