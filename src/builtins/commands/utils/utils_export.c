/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 15:59:10 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 04:00:40 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_sorted(t_vars **sorted, t_vars *new)
{
	t_vars	*current;

	if (*sorted == NULL || ft_strcmp_tr((*sorted)->key, new->key) > 0)
	{
		new->next = *sorted;
		*sorted = new;
	}
	else
	{
		current = *sorted;
		while (current->next && ft_strcmp_tr(current->next->key, new->key) < 0)
			current = current->next;
		new->next = current->next;
		current->next = new;
	}
}

t_vars	*sort_vars(t_vars *vars)
{
	t_vars	*sorted;
	t_vars	*next;

	sorted = NULL;
	while (vars)
	{
		next = vars->next;
		insert_sorted(&sorted, vars);
		vars = next;
	}
	return (sorted);
}

t_vars	*dup_env(t_vars *vars)
{
	t_vars	*result;
	t_vars	*new;

	result = NULL;
	while (vars)
	{
		if (vars->hide == FALSE)
		{
			new = malloc(sizeof(t_vars));
			if (!new)
				return (NULL);
			new->key = ft_strdup(vars->key);
			new->value = ft_strdup(vars->value);
			new->hide = FALSE;
			new->next = NULL;
			add_vars(new, &result);
		}
		vars = vars->next;
	}
	return (result);
}

void	print_env(t_vars *vars)
{
	t_vars	*sorted_vars;
	t_vars	*current;

	sorted_vars = sort_vars(dup_env(vars));
	current = sorted_vars;
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
	delete_all_vars(&sorted_vars);
}

int	calculate_size_export(char *str, t_bool add)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] != '=' && !(add && str[i] == '+' && str[i
					+ 1] == '=')))
		i++;
	return (i);
}
