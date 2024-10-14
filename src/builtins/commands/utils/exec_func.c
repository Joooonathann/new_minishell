/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:38:01 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/14 17:51:39 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_process(t_minishell **data, t_bool env, t_bool data_free)
{
	if ((*data)->tokens)
		ft_free_tokens(&(*data)->tokens);
	if ((*data)->current_tokens)
		free_current(&(*data)->current_tokens);
	if ((*data)->tokens_split)
		free_split_tokens((*data)->tokens_split);
	if ((*data)->prompt)
		free((*data)->prompt);
	if ((*data)->env && env)
		delete_all_vars(&(*data)->env);
	if (*data && data_free)
		free(*data);
}

int	count_tokens_split(t_tokens **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

int	nofork_command(t_tokens *tokens)
{
	if (ft_strcmp(tokens->value, "exit") || ft_strcmp(tokens->value, "cd")
		|| ft_strcmp(tokens->value, "unset") || ft_strcmp(tokens->value,
			"export"))
		return (1);
	return (0);
}
