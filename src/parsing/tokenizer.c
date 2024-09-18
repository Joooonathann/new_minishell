/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:50:34 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/16 13:54:13 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quote(char *str, QUOTE quote_type)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == (char)quote_type)
			j++;
		i++;
	}
	return (j);
}

char	*create_delim_token(char **str, char **token)
{
	if ((**str == '>' || **str == '<') && **str == *(*str + 1))
	{
		*token = malloc(sizeof(char) * (3));
		if (!*token)
			return (NULL);
		(*token)[0] = **str;
		(*token)[1] = *(*str + 1);
		(*token)[2] = '\0';
		(*str) += 2;
		return (*token);
	}
	else
	{
		*token = malloc(sizeof(char) * (2));
		if (!*token)
			return (NULL);
		(*token)[0] = **str;
		(*token)[1] = '\0';
		(*str)++;
		return (*token);
	}
}

void	copy_in_token(char **str, BOOL *in_quote, QUOTE *quote_type)
{
	if (((**str == SIMPLE && count_quote(*str, SIMPLE) > 1)
			|| (**str == DOUBLE && count_quote(*str, DOUBLE) > 1))
		&& !*in_quote)
	{
		*in_quote = TRUE;
		*quote_type = (QUOTE) **str;
	}
	else if (**str == (char)*quote_type && *in_quote)
	{
		*in_quote = FALSE;
		*quote_type = 0;
	}
}

char	*ms_strtok(char **str, char *delim)
{
	char	*token;
	int		i;
	BOOL	in_quote;
	QUOTE	quote_type;

	i = 0;
	in_quote = FALSE;
	quote_type = 0;
	while (**str == ' ')
		(*str)++;
	if (!**str)
		return (NULL);
	if (ft_strchr((const char *)delim, **str))
		return (create_delim_token(str, &token));
	token = init_ms_token(*str, delim);
	if (!token)
		return (NULL);
	while (**str && (!ft_strchr((const char *)delim, **str) || in_quote))
	{
		copy_in_token(str, &in_quote, &quote_type);
		token[i++] = **str;
		(*str)++;
	}
	token[i] = '\0';
	return (token);
}

void	tokenizer(char **str, t_tokens **tokens)
{
	char	*token;

	while (**str)
	{
		token = NULL;
		token = ms_strtok(str, " |><");
		if (token)
			ft_tokenadd_back(tokens, ft_tokennew(token));
	}
}
