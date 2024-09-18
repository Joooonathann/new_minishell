/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenadd_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:59:10 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/06 01:26:52 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_tokenadd_back(t_tokens **tokens, t_tokens *new)
{
	t_tokens	*last;

	if (tokens && new)
	{
		if (*tokens == NULL)
			*tokens = new;
		else
		{
			last = ft_tokenlast(*tokens);
			last->next = new;
			new->prev = last;
		}
	}
}
