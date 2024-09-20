#include "minishell.h"
#include <errno.h>
#include <sys/stat.h>

void	cd_command(t_minishell **data)
{
	char		*path;
	char		*current_pwd;
	t_vars		*home_var;
	t_tokens	*token;
	struct stat	info;
	char		*temp;
	char		*tmp;
	int			q;

	token = (*data)->current_tokens;
	if (ft_count_tokens(token) > 2)
	{
		ft_error(1, "bash: cd: too many arguments");
		update_vars(&(*data)->env, "?", "1");
		return ;
	}
	if (ft_count_tokens(token) == 1 || (token->next
			&& ft_strcmp(token->next->value, "~")))
	{
		home_var = get_vars(&(*data)->env, "HOME");
		if (!home_var || !home_var->value)
		{
			ft_error(1, "bash: cd: HOME not set");
			update_vars(&(*data)->env, "?", "1");
			return ;
		}
		path = ft_strdup(home_var->value);
	}
	else
	{
		token = token->next;
		path = ft_strdup(token->value);
		if (path[0] == '~')
		{
			home_var = get_vars(&(*data)->env, "HOME");
			if (home_var && home_var->value)
			{
				temp = path;
				path = ft_strjoin(home_var->value, path + 1);
				free(temp);
			}
		}
	}
	if (lstat(path, &info) == -1)
	{
		ft_error(4, "bash: cd: ", path, ": ", strerror(errno));
		update_vars(&(*data)->env, "?", "1");
		free(path);
		return ;
	}
	if (chdir(path) != 0)
	{
		ft_error(3, "bash: cd: ", path, ": No such file or directory");
		update_vars(&(*data)->env, "?", "1");
		free(path);
		return ;
	}
	update_vars(&(*data)->env, "OLDPWD", get_vars(&(*data)->env, "PWD")->value);
	current_pwd = getcwd(NULL, 0);
	if (current_pwd)
	{
		if (S_ISLNK(info.st_mode))
		{
			q = ft_strlen(current_pwd);
			while (q > 0 && current_pwd[q - 1] != '/')
				q--;
			tmp = malloc(sizeof(char) * (q + ft_strlen(path) + 1));
			if (!tmp)
			{
				ft_error(1, "bash: cd: malloc error");
				free(current_pwd);
				return ;
			}
			ft_strlcpy(tmp, current_pwd, q + 1);
			ft_strcat(tmp, path);
			update_vars(&(*data)->env, "PWD", tmp);
			free(tmp);
		}
		else
			update_vars(&(*data)->env, "PWD", current_pwd);
		free(current_pwd);
	}
	else
	{
		ft_error(1, "bash: cd: getcwd error");
	}
	free(path);
	update_vars(&(*data)->env, "?", "0");
}
