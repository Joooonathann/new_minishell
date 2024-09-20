/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 03:58:23 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 04:18:43 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_command(t_minishell **data)
{
	t_tokens	*token;
	t_vars		*var_to_remove;

	token = (*data)->current_tokens;
	if (ft_count_tokens(token) < 2)
	{
		update_vars(&(*data)->env, "?", "0");
		return ;
	}
	token = token->next;
	while (token)
	{
		var_to_remove = get_vars(&(*data)->env, token->value);
		if (var_to_remove && !var_to_remove->hide)
			delete_vars(&(*data)->env, var_to_remove);
		token = token->next;
	}
	update_vars(&(*data)->env, "?", "0");
}
