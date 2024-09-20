/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:49:08 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/20 16:03:18 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_command(t_minishell **data)
{
	t_vars	*env;

	env = (*data)->env;
	while (env)
	{
		if (!env->hide && env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	exit(0);
}
