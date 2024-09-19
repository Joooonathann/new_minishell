/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalbiser <jalbiser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:46:17 by jalbiser          #+#    #+#             */
/*   Updated: 2024/09/19 17:05:21 by jalbiser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lstcmd *init_commands(void)
{
    t_lstcmd *commands;

    commands = (t_lstcmd *)malloc(2 * sizeof(t_lstcmd));
    if (!commands)
        return (NULL);
    commands[0].name = "echo";
    commands[0].func = echo_command;
    commands[1].name = NULL;
    commands[1].func = NULL;
    return (commands);
}

void handler_builtins(t_minishell data)
{
    int i;
    t_lstcmd *commands;

    i = 0;
    commands = init_commands();
    if (!commands)
        return;

    while (commands[i].name != NULL)
    {
        if (ft_strcmp(commands[i].name, data.current_tokens->value))
        {
            commands[i].func(data);
            free(commands);
            return;
        }
        i++;
    }
    free(commands);
    // extern_command(data); // Décommenter pour les commandes externes
}
