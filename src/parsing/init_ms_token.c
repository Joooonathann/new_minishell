/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ms_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:12:00 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/16 13:55:04 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	calc_len(char **str, BOOL *in_quote, QUOTE *quote_type, int *len)
{
	if (((**str == SIMPLE && count_quote(*str, SIMPLE) > 1)
			|| (**str == DOUBLE && count_quote(*str, DOUBLE) > 1))
		&& !*in_quote)
	{
		*in_quote = TRUE;
		*quote_type = (QUOTE) * *str;
	}
	else if (**str == (char)*quote_type && *in_quote)
	{
		*in_quote = FALSE;
		*quote_type = 0;
	}
	(*len)++;
	(*str)++;
}

char	*init_ms_token(char *str, char *delim)
{
	char	*token;
	int		len;
	BOOL	in_quote;
	QUOTE	quote_type;

	len = 0;
	in_quote = FALSE;
	quote_type = 0;
	while (*str == ' ')
		str++;
	while (*str && (!ft_strchr((const char *)delim, *str) || in_quote))
		calc_len(&str, &in_quote, &quote_type, &len);
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	token[len] = '\0';
	return (token);
}
