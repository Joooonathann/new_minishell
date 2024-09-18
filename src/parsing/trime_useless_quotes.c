/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trime_useless_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekrause <emeric.yukii@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:33:40 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/16 16:19:37 by ekrause          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_new_len(t_tokens *tokens)
{
	int		len;
	int		i;
	BOOL	in_quote;
	QUOTE	quote_type;

	len = 0;
	i = 0;
	in_quote = FALSE;
	quote_type = 0;
	while (tokens->value[i])
	{
		if ((tokens->value[i] == SIMPLE || tokens->value[i] == DOUBLE)
			&& (!in_quote || tokens->value[i] == (char)quote_type))
		{
			in_quote = !in_quote;
			if (in_quote)
				quote_type = tokens->value[i];
			else
				quote_type = 0;
		}
		else
			len++;
		i++;
	}
	return (len);
}

char	*init_new_value(t_tokens *tokens)
{
	char	*new_value;
	int		len;

	len = get_new_len(tokens);
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return (NULL);
	new_value[len] = '\0';
	return (new_value);
}

void	create_new_token_without_quotes(t_tokens *token, char **new_value)
{
	BOOL	in_quote;
	QUOTE	quote_type;
	int		i;
	int		j;

	in_quote = FALSE;
	quote_type = 0;
	j = 0;
	i = 0;
	while (token->value[i])
	{
		if ((token->value[i] == SIMPLE || token->value[i] == DOUBLE)
			&& (!in_quote || token->value[i] == (char)quote_type))
		{
			in_quote = !in_quote;
			if (in_quote)
				quote_type = token->value[i];
			else
				quote_type = 0;
		}
		else
			(*new_value)[j++] = token->value[i];
		i++;
	}
}

void	trime_useless_quotes(t_tokens **tokens)
{
	t_tokens	*current_token;
	char		*new_value;

	current_token = *tokens;
	while (current_token)
	{
		new_value = init_new_value(current_token);
		if (!new_value)
			return ;
		create_new_token_without_quotes(current_token, &new_value);
		free(current_token->value);
		current_token->value = ft_strdup(new_value);
		free(new_value);
		current_token = current_token->next;
	}
}
