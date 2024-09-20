/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 02:29:22 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 16:01:25 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	handle_value_append(char *str, t_vars *new, t_vars *tmp, BOOL add)
{
	char	*new_value;
	int		value_start;

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
	}
	else
		new->value = ft_strndup_range(str, calculate_size_export(str, add) + 2,
				ft_strlen(str));
}

static void	handle_value_set(char *str, t_vars *new, BOOL add)
{
	if (calculate_size_export(str, add) == (int)ft_strlen(str))
		new->value = NULL;
	else
		new->value = ft_strndup_range(str, calculate_size_export(str, add) + 1,
				ft_strlen(str));
}

void	add_vars_export(char *str, t_minishell **data, BOOL add)
{
	t_vars	*new;
	t_vars	*tmp;

	new = malloc(sizeof(t_vars));
	if (!new)
		return ;
	new->key = ft_strndup_range(str, 0, calculate_size_export(str, add) - 1);
	tmp = get_vars(&(*data)->env, new->key);
	if (add)
		handle_value_append(str, new, tmp, add);
	else
		handle_value_set(str, new, add);
	if (tmp)
		delete_vars(&(*data)->env, tmp);
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
