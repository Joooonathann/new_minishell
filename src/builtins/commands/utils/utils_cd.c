/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:09:03 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 21:08:51 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_home_path(t_minishell **data, char **path, t_tokens *token)
{
	t_vars	*home_var;

	if (ft_count_tokens(token) == 1 || (token->next
			&& ft_strcmp(token->next->value, "~")))
	{
		home_var = get_vars(&(*data)->env, "HOME");
		if (!home_var || !home_var->value)
		{
			ft_error(1, "bash: cd: HOME not set");
			update_vars(&(*data)->env, "?", "1");
			return (0);
		}
		*path = ft_strdup(home_var->value);
		return (1);
	}
	*path = ft_strdup(token->next->value);
	return (1);
}

void	handle_tilde(t_minishell **data, char **path)
{
	t_vars	*home_var;
	char	*temp;

	if ((*path)[0] == '~')
	{
		home_var = get_vars(&(*data)->env, "HOME");
		if (home_var && home_var->value)
		{
			temp = *path;
			*path = ft_strjoin(home_var->value, (*path) + 1);
			free(temp);
		}
	}
}

void	handle_chdir_error(char *path, t_minishell **data)
{
	ft_error(3, "bash: cd: ", path, ": No such file or directory");
	update_vars(&(*data)->env, "?", "1");
}
