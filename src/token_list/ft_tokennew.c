/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokennew.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:40:21 by ekrause           #+#    #+#             */
/*   Updated: 2024/10/15 22:28:17 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*ft_tokennew(char *value)
{
	t_tokens	*elem;

	elem = malloc(sizeof(t_tokens));
	if (!elem)
		return (NULL);
	elem->next = NULL;
	elem->prev = NULL;
	elem->redirection = '\0';
	elem->pipe = '\0';
	elem->quote = 0;
	elem->value = value;
	return (elem);
}

t_tokens	*ft_tokennew_rework(char *value, t_token_type type)
{
	t_tokens	*elem;

	elem = malloc(sizeof(t_tokens));
	if (!elem)
		return (NULL);
	elem->next = NULL;
	elem->prev = NULL;
	elem->redirection = '\0';
	elem->pipe = '\0';
	elem->quote = 0;
	elem->value = value;
	elem->type = type;
	return (elem);
}
