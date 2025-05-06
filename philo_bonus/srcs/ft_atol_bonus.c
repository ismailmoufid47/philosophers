/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-mou <isel-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:03:49 by isel-mou          #+#    #+#             */
/*   Updated: 2025/05/05 12:52:05 by isel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

// ----------------------------------------------------------------------------
// Convert string to long integer
long	ft_atol(const char *str)
{
	long	num;
	int		i;

	if (!str || !*str)
		return (-1);
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '+')
		++i;
	if (str[i] == '-')
		return (-1);
	num = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if (num > (LONG_MAX / 10) || (num == (LONG_MAX / 10) && str[i] > '7'))
			return (-1);
		num = num * 10 + (str[i] - '0');
		++i;
	}
	if (!str[i] && !num)
		return (-1);
	return (num);
}
