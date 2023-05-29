/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bottom_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:19:49 by tsankola          #+#    #+#             */
/*   Updated: 2023/05/29 17:19:50 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"
#include <unistd.h>

void	bottom_duplicator(int input, int output, int errput)
{
		dup2(input, STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		dup2(errput, STDERR_FILENO);
}