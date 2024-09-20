/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tokens_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 19:16:42 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 19:28:36 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens_split(t_tokens **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return;
	while (tokens[i])
	{
		ft_free_tokens(&tokens[i]);
		tokens[i] = NULL;
		i++;
	}
	free(tokens);
	tokens = NULL;
}
