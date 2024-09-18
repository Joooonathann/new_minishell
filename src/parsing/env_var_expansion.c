/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:33:58 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/16 13:53:49 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_char_to_str(char *str, char c)
{
	char	*result;
	int		i;
	int		len;

	if (!str)
	{
		result = malloc(sizeof(char) * 2);
		result[0] = c;
		result[1] = '\0';
		return (result);
	}
	i = 0;
	len = ft_strlen(str);
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	result[len + 1] = '\0';
	result[len] = c;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	free(str);
	return (result);
}

static int	return_value(t_tokens *token, int i,
						BOOL in_quote, QUOTE quote_type)
{
	return ((token->value[i] == SIMPLE || token->value[i] == DOUBLE)
		&& (!in_quote || (char)quote_type == token->value[i])
		&& count_quote(token->value, SIMPLE) > 1);
}

static int	return_env_value(t_tokens *token, int i)
{
	return (token->value[i + 1] && (ft_isalnum(token->value[i + 1])
			|| token->value[i + 1] == '?'));
}

char	*expand_tokens(t_tokens *token, t_vars **env)
{
	char	*expanded_value;
	int		i;
	BOOL	in_quote;
	QUOTE	quote_type;

	expanded_value = ft_strdup("");
	i = 0;
	in_quote = FALSE;
	quote_type = 0;
	while (token->value[i])
	{
		if (return_value(token, i, in_quote, quote_type))
			handle_quotes(&in_quote, &quote_type,
				token->value[i++], &expanded_value);
		else if ((!in_quote || quote_type == DOUBLE) && token->value[i] == '$')
		{
			if (return_env_value(token, i))
				handle_env_vars(token, &i, env, &expanded_value);
			else
				return (ft_strdup(token->value));
		}
		else
			expanded_value = add_char_to_str(expanded_value, token->value[i++]);
	}
	return (expanded_value);
}

void	env_var_expansion(t_tokens **tokens, t_vars **env)
{
	t_tokens	*current_token;
	char		*expanded_value;

	current_token = *tokens;
	while (current_token)
	{
		expanded_value = expand_tokens(current_token, env);
		if (expanded_value)
		{
			free(current_token->value);
			current_token->value = ft_strdup(expanded_value);
			free(expanded_value);
		}
		current_token = current_token->next;
	}
}
