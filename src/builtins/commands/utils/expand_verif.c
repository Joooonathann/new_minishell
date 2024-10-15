/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_verif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:11:43 by jalbiser          #+#    #+#             */
/*   Updated: 2024/10/15 22:11:40 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_quotes(const char *quote, const char *str)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(quote, str);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, quote);
	free(tmp);
	return (result);
}

static int	check_quotes(t_minishell **data, char **tmp, char *quoted,
		char *dbl_quoted)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i][0] == '"')
			expand_var_heredoc(&tmp[i], &(*data)->env);
		if (ft_strcmp(tmp[i], quoted) || ft_strcmp(tmp[i], dbl_quoted))
			return (0);
		i++;
	}
	return (1);
}

static int	prepare_quotes(char **quoted, char **dbl_quoted, char *filename)
{
	*quoted = join_quotes("'", filename);
	*dbl_quoted = join_quotes("\"", filename);
	if (!(*quoted) || !(*dbl_quoted))
	{
		free(*quoted);
		free(*dbl_quoted);
		return (0);
	}
	return (1);
}

int	expand_verif(t_minishell **data, char *line)
{
	char	**tmp;
	char	*quoted;
	char	*dbl_quoted;
	char	*tmp_line;
	int		res;

	tmp_line = ft_strdup(line);
	expand_var_heredoc(&tmp_line, &(*data)->env);
	if (ft_strcmp(tmp_line, (*data)->files->name))
	{
		free(tmp_line);
		return (1);
	}
	if (!prepare_quotes(&quoted, &dbl_quoted, (*data)->files->name))
		return (0);
	tmp = ft_split((*data)->prompt, ' ');
	if (!tmp)
		return (0);
	res = check_quotes(data, tmp, quoted, dbl_quoted);
	free(quoted);
	free(dbl_quoted);
	free_split(tmp);
	return (res);
}
