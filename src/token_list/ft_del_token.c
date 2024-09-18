/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_del_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:08:07 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/06 01:26:18 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_token(t_tokens *token)
{
	if (token)
	{
		if (token->value)
			free(token->value);
		free(token);
	}
}

void	ft_del_token(t_tokens **token, t_tokens **tokens)
{
	t_tokens	*temp;

	temp = *token;
	if (!(*token)->prev)
	{
		*tokens = (*tokens)->next;
		*token = (*token)->next;
		(*token)->prev = NULL;
	}
	else
	{
		(*token)->prev->next = (*token)->next;
		(*token)->next->prev = (*token)->prev;
		*token = (*token)->next;
	}
	ft_free_token(temp);
}
