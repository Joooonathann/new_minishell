/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:57:32 by ekrause           #+#    #+#             */
/*   Updated: 2024/09/22 02:20:14 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env_vars_heredoc(char *str, int *i, t_vars **env,
		char **expanded_value)
{
	char	*env_var;

	env_var = NULL;
	(*i)++;
	if (str[*i] == '?')
		env_var = add_char_to_str(env_var, str[(*i)++]);
	else
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			env_var = add_char_to_str(env_var, str[(*i)++]);
	}
	if (get_vars(env, env_var))
		*expanded_value = ft_strcat_dynamic(*expanded_value, get_vars(env,
					env_var)->value);
	free(env_var);
}

void	expand_var_heredoc(char **str, t_vars **env)
{
	char	*expanded_value;
	int		i;

	expanded_value = ft_strdup("");
	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$')
		{
			if ((*str)[i + 1] && (ft_isalnum((*str)[i + 1]) || (*str)[i
					+ 1] == '?'))
				handle_env_vars_heredoc(*str, &i, env, &expanded_value);
			else
			{
				expanded_value = ft_strcat_dynamic(expanded_value, *str);
				i++;
			}
		}
		else
			expanded_value = add_char_to_str(expanded_value, (*str)[i++]);
	}
	free(*str);
	*str = ft_strdup(expanded_value);
	free(expanded_value);
}
