/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenlast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:04:48 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/06 01:26:54 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*ft_tokenlast(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = NULL;
	if (tokens)
	{
		temp = tokens;
		while (temp->next)
			temp = temp->next;
	}
	return (temp);
}
