/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:50:32 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/20 19:26:26 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_tokens **tokens)
{
	t_tokens	*previous_token;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		previous_token = *tokens;
		*tokens = (*tokens)->next;
		if (previous_token->value)
		{
			free(previous_token->value);
			previous_token->value = NULL;
		}
		free(previous_token);
		previous_token = NULL;
	}
	*tokens = NULL;
}
