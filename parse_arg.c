/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/26 23:57:05 by aait-ihi          #+#    #+#             */
/*   Updated: 2019/12/27 02:12:59 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sequel(char **buff)
{
	char *line;
	char *tmp;
	int len;

	while (!(line = ft_readline()) || *line == '\0')
	{
		free(line);
		ft_printf(">");
	}
	if (!(tmp = ft_strnew(ft_strlen(line) + ft_strlen(buff))))
	{
		free(line);
		return (NULL);
	}
	ft_strcpy(tmp,buff);
	ft_strdel(buff);
	*buff = tmp;
	return (line);
}

char *parse_pipe_or_semicoma(char *line,char *buff, char c)
{
	while(*line == c)
		*buff++ == *line++;
	ft_skip_chars(line, " ");
	return(line);
}

char *parse_and()
{

}

int escap(char *str, const char *from, char *to, int size)
{
	ft_translate(str - size, from, to);
	return (1);
}

char *ft_parse_arg(char *arg, char *buff, const char *cmp)
{
	const char *tmp = arg;

	while (*arg && !ft_isinstr(*arg, cmp))
	{
		if (*arg == '\\')
			if (arg++ && !*arg)
				break;
		*buff++ = *arg++;
	}
	*buff = 0;
	if (*arg == '"' && *cmp != '"')
		return (ft_parse_arg(arg + 1, buff, "\""));
	if (*arg == '"' && *cmp == '"' && escap(buff, "~", "\2", arg - tmp))
		return (ft_parse_arg(arg + 1, buff, " \t'\""));
	if (*arg == '\'' && *cmp != '\'')
		return (ft_parse_arg(arg + 1, buff, "'"));
	if (*arg == '\'' && *cmp == '\'' && escap(buff, "$~", "\1\2", arg - tmp))
		return (ft_parse_arg(arg + 1, buff, " \t'\""));
	if (*cmp == '\'' || *cmp == '"')
	{
		PRINT_ERROR(UNMATCHED, cmp);
		return (NULL);
	}
	return (arg);
}
