/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:29:22 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 04:02:04 by jalbiser         ###   ########.fr       */
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

	sorted_vars = sort_vars(dup_env(vars));
	while (sorted_vars)
	{
		printf("declare -x %s", sorted_vars->key);
		if (sorted_vars->value)
			printf("=\"%s\"", sorted_vars->value);
		printf("\n");
		sorted_vars = sorted_vars->next;
	}
	delete_all_vars(&sorted_vars);
}

static int	calculate_size_export(char *str, BOOL add)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] != '=' && !(add && str[i] == '+' && str[i
				+ 1] == '=')))
		i++;
	return (i);
}

static int	is_valid_splited(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '=' || ft_isdigit(str[0]))
		return (0);
	while (str[i])
	{
		if (str[i] == '+' && i != 0 && str[i + 1] && str[i + 1] == '=')
			return (2);
		if (str[i] == '=')
			break ;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	add_vars_export(char *str, t_minishell **data, BOOL add)
{
	t_vars	*new;
	t_vars	*tmp;
	char	*new_value;
	int		value_start;

	new = malloc(sizeof(t_vars));
	if (!new)
		return ;
	new->key = ft_strndup_range(str, 0, calculate_size_export(str, add) - 1);
	if (calculate_size_export(str, add) == (int)ft_strlen(str))
		new->value = NULL;
	else
	{
		if (add)
		{
			tmp = get_vars(&(*data)->env, new->key);
			if (tmp->value)
			{
				value_start = calculate_size_export(str, add) + 2;
				new_value = malloc(sizeof(char) * (ft_strlen(tmp->value)
							+ ft_strlen(str) - value_start + 1));
				if (!new_value)
					return ;
				ft_strcpy(new_value, tmp->value);
				ft_strcat(new_value, str + value_start);
				new->value = new_value;
				delete_vars(&(*data)->env, tmp);
			}
			else
			{
				if (tmp)
					delete_vars(&(*data)->env, tmp);
				free(new->value);
				new->value = ft_strndup_range(str, calculate_size_export(str,
							add) + 2, ft_strlen(str));
			}
		}
		else
		{
			new->value = ft_strndup_range(str, calculate_size_export(str, add)
					+ 1, ft_strlen(str));
		}
	}
	new->hide = FALSE;
	new->next = NULL;
	add_vars(new, &(*data)->env);
}

void	export_command(t_minishell **data)
{
	if (ft_count_tokens((*data)->current_tokens) == 1)
		print_env((*data)->env);
	(*data)->current_tokens = (*data)->current_tokens->next;
	while ((*data)->current_tokens)
	{
		if (!is_valid_splited((*data)->current_tokens->value))
		{
			ft_error(3, "myfuckingbash: export: '",
				(*data)->current_tokens->value, "': not a valid identifier");
			update_vars(&(*data)->env, "?", "1");
			return ;
		}
		if (is_valid_splited((*data)->current_tokens->value) == 1)
			add_vars_export((*data)->current_tokens->value, data, FALSE);
		else if (is_valid_splited((*data)->current_tokens->value) == 2)
			add_vars_export((*data)->current_tokens->value, data, TRUE);
		(*data)->current_tokens = (*data)->current_tokens->next;
	}
}
