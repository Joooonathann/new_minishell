/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 22:21:39 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/21 16:25:42 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_command(t_minishell **data)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	if (get_vars(&(*data)->env, "PWD"))
		printf("%s\n", get_vars(&(*data)->env, "PWD")->value);
	else
		printf("%s\n", cwd);
	exit(0);
}
