/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/26 23:57:05 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/01/19 03:13:50 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sequel(char **buff, int *buff_len)
{
	char *line;

	if ((line = ft_readline(">")))
	{
		if (!(*buff = ft_realloc(*buff, *buff_len + 1, ft_strlen(line) + 1)))
		{
			free(line);
			return (NULL);
		}
		ft_strcat(*buff, "\n");
		(*buff_len)++;
	}
	return (line);
}

char *copy_to_buff(char *arg, char *buff, int *i, const char *cmp)
{
	while (*arg && !ft_isinstr(*arg, cmp))
	{
		if (*arg == '\\')
			if (arg++ && !*arg)
				break;
		buff[(*i)++] = *arg++;
	}
	buff[*i] = 0;
	return (arg);
}

char *parse_redirections(char *arg, char **buff, int *i)
{
	int j;
	char c;

	if (!arg || !buff || !*buff)
		return (NULL);
	(*i == 0) ? (buff[0][(*i)++] = *arg) : 0;
	j = *i;
	c = *arg;
	while (*arg == c)
		buff[0][(*i)++] = *arg++;
	if (*i - j > 2)
	{
		ft_printf("parse error near `%.*s'\n", 1 << ((*i - j) > 3), arg - 2);
		return (NULL);
	}
	(*arg == '&') ? buff[0][(*i)++] = '&' : 0;
	arg = ft_skip_chars(arg, " \t", NULL);
	if (!*arg || ft_isinstr(*arg, "&|;"))
	{
		ft_printf("parse error near `%.*s'\n", *arg == '\n' ? (char[]){'\\', 'n', 0} : (char[]){*arg, 0});
		return (NULL);
	}
	return (arg);
}

char *parse_pipe(char *arg, char **buff, int *i)
{
	int j;

	j = *i;
	if (!arg || !buff || !*buff)
		return (NULL);
	while (*arg == '|')
		buff[0][(*i)++] = *arg++;
	buff[0][(*i)] = 0;
	if (*i - j > 2)
	{
		ft_printf("parse error near `%.*s'\n", 1 << ((*i - j) > 3), arg - 2);
		return (NULL);
	}
	arg = ft_skip_chars(arg, " \t", NULL);
	while (!*arg)
	{
		//ft_strdel(&arg);
		arg = sequel(buff, i);
	}
	return (arg);
}

char *parse_quotes(char *arg, char **buff, int *i)
{
	char c;

	if (!arg || !buff || !*buff)
		return (NULL);
	c = *arg++;
	while (1)
	{
		arg = copy_to_buff(arg, *buff, i, (char[2]){c, 0});
		if (*arg == c)
			break;
		//ft_strdel(&arg);
		arg = sequel(buff, i);
	}
	return (arg + 1);
}

char *ft_parse_arg(char *arg, char **buff)
{
	int index;

	index = 0;
	while (arg && *arg)
	{
		arg = copy_to_buff(arg, *buff, &index, ";|&><' \t\"");
		if (*arg == '"' || *arg == '\'')
			arg = parse_quotes(arg, buff, &index);
		else if ((*arg == '|' || *arg == '&') && index == 0)
			return (parse_pipe(arg, buff, &index));
		else if (*arg == ';' && index == 0)
		{
			while (*arg == ';')
				buff[0][index++] = *arg++;
			buff[0][index] = 0;
			return (arg);
		}
		else if ((*arg == '<' || *arg == '>') && BETWEEN(index, 0, 1))
			arg = parse_redirections(arg, buff, &index);
		else
			break;
	}
	return (arg);
}
