/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 22:21:39 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 23:06:31 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_command(t_minishell **data)
{
	char	cwd[PATH_MAX];

	(void) data;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_error(2, "pwd: error retrieving current directory: ",
			strerror(errno));
		exit(1);
	}
	printf("%s\n", cwd);
	exit(0);
}