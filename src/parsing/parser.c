/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekrause <emeric.yukii@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:52:19 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/17 17:47:57 by ekrause          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(int count, ...)
{
	va_list	args;
	char	*str;
	int		i;

	str = NULL;
	i = 0;
	va_start(args, count);
	while (i < count)
	{
		str = va_arg(args, char *);
		write(2, str, ft_strlen(str));
		i++;
	}
	write(2, "\n", 1);
	va_end(args);
}

int	check_meta_characters(t_tokens *tokens)
{
	while (tokens)
	{
		if (tokens->type == TYPE_REDIRECTION && (!tokens->next
				|| tokens->next->type != TYPE_ARGUMENT))
		{
			if (tokens->next)
				ft_error(5, "myfuckingbash: ",
					"syntax error near unexpected token ", "`",
					tokens->next->value, "\'");
			else
				ft_error(3, "myfuckingbash: ",
					"syntax error near unexpected token ", "newline");
			return (0);
		}
		else if (tokens->type == TYPE_PIPE && (!tokens->next
				|| !tokens->prev))
		{
			ft_error(5, "myfuckingbash: ",
				"syntax error near unexpected token ", "`", tokens->value,
				"\'");
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

void	trime_tokens(t_tokens **tokens)
{
	t_tokens	*tmp;
	t_tokens	*del;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->value[0] == '\0')
		{
			del = tmp;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				*tokens = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			tmp = tmp->next;
			free(del->value);
			free(del);
		}
		else
		{
			tmp = tmp->next;
		}
	}
}

t_tokens	*parser(char *str, t_vars **env)
{
	t_tokens	*tokens;

	tokens = NULL;
	tokenizer(&str, &tokens);
	env_var_expansion(&tokens, env);
	add_token_type(&tokens);
	if (!check_meta_characters(tokens))
		return (NULL);
	trime_tokens(&tokens);
	trime_useless_quotes(&tokens);
	return (tokens);
}
