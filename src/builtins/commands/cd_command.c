/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:05:03 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/24 00:47:49 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_cd_command(t_minishell **data, char *path, struct stat info)
{
	char	*current_pwd;

	if (chdir(path) != 0)
	{
		handle_chdir_error(path, data);
		return ;
	}
	current_pwd = getcwd(NULL, 0);
	if (current_pwd)
	{
		update_vars(&(*data)->env, "OLDPWD", get_vars(&(*data)->env,
				"PWD")->value);
		if (S_ISLNK(info.st_mode))
			handle_symlink(data, path, current_pwd);
		else
			update_vars(&(*data)->env, "PWD", current_pwd);
		free(current_pwd);
	}
	else
		ft_error(1, "bash: cd: getcwd error");
}

static void	create_pwd_copy(t_minishell **data)
{
	t_vars	*new_pwd;

	new_pwd = malloc(sizeof(t_vars));
	if (!new_pwd)
		return ;
	new_pwd->key = ft_strdup("PWD");
	new_pwd->value = ft_strdup("");
	new_pwd->hide = TRUE;
	new_pwd->next = NULL;
	if (!exist_masked((*data)->env, "PWD") && !exist_vars((*data)->env, "PWD"))
		add_vars(new_pwd, &(*data)->env);
	else
	{
		free(new_pwd->key);
		free(new_pwd->value);
		free(new_pwd);
	}
}

static void	create_oldpwd_copy(t_minishell **data)
{
	t_vars	*new_oldpwd;

	new_oldpwd = malloc(sizeof(t_vars));
	if (!new_oldpwd)
		return ;
	new_oldpwd->key = ft_strdup("OLDPWD");
	new_oldpwd->value = ft_strdup("");
	new_oldpwd->hide = TRUE;
	new_oldpwd->next = NULL;
	if (!exist_masked((*data)->env, "OLDPWD") && !exist_vars((*data)->env,
			"OLDPWD"))
		add_vars(new_oldpwd, &(*data)->env);
	else
	{
		free(new_oldpwd->key);
		free(new_oldpwd->value);
		free(new_oldpwd);
	}
}

static void	create_copy(t_minishell **data)
{
	create_pwd_copy(data);
	create_oldpwd_copy(data);
}

void	cd_command(t_minishell **data)
{
	char		*path;
	struct stat	info;
	t_tokens	*token;

	token = (*data)->current_tokens;
	if (ft_count_tokens(token) > 2)
	{
		ft_error(1, "bash: cd: too many arguments");
		update_vars(&(*data)->env, "?", "1");
		return ;
	}
	if (!handle_home_path(data, &path, token))
		return ;
	handle_tilde(data, &path);
	if (lstat(path, &info) == -1)
	{
		ft_error(4, "bash: cd: ", path, ": ", strerror(errno));
		update_vars(&(*data)->env, "?", "1");
		free(path);
		return ;
	}
	create_copy(data);
	handle_cd_command(data, path, info);
	free(path);
	update_vars(&(*data)->env, "?", "0");
}
