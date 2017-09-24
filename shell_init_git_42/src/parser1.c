/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/21 19:47:07 by jye               #+#    #+#             */
/*   Updated: 2017/09/24 11:52:26 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include "token.h"
#include "etc_parse.h"
#include "command.h"
#include "ft_printf.h"
#include "libft.h"

#define STX_IS_AMP(sym) ((sym) == ampersand)
#define STX_IS_PER(sym) ((sym) == period)
#define STX_IS_NUM(sym) ((sym) == number)
#define STX_IS_WORD(sym) ((sym) == word)
#define STX_PLS_NORM1(sym) (STX_IS_AMP((sym)) || STX_IS_PER((sym)))
#define STX_PLS_NORM2(sym) (STX_IS_NUM((sym)) || STX_IS_WORD((sym)))
#define SYNTAX_IS_EOL(sym)  (STX_PLS_NORM1((sym)) || STX_PLS_NORM2((sym)))

static t_lst	*syntax_get_next_token_node(t_token *stack)
{
	char	*prompt;
	char	*exp;
	t_lst	*tokens;

	prompt = ((char*[]){"cmdand> ", "cmdor> ", "pipe> "})[stack->sym - andsym];
	while ((exp = ft_readline(prompt, ft_strlen(prompt))) &&
			(exp != 0 && exp != (char *)-1))
	{
		exp = transmute_exp_spec(exp, 0);
		if (exp && (tokens = tokenize(exp)) == 0)
		{
			free(exp);
			continue ;
		}
		free(exp);
		break ;
	}
	if (exp == 0 || exp == (char *)-1)
	{
		if (exp == 0)
			ft_dprintf(2, "%s: Unexpected end of line\n", "minishell");
		return ((t_lst *)0);
	}
	return (tokens);
}

static int		syntax_check_stack(t_token *to, t_token *stack)
{
	if (to->sym <= number)
		*stack = ((t_token){.s = 0, .sym = 0, .symbreak = 0});
	else if (stack->sym > number)
	{
		parse_error("minishell", stack->s, ERROR_EXPECTED);
		return (1);
	}
	else
		*stack = ((t_token){.s = to->s, .sym = to->sym, .symbreak = 0});
	return (0);
}

static int		syntax_check_(t_token *stack, t_lst *tokens)
{
	t_lst	*last;
	t_token	*to;

	while (42)
	{
		while (tokens)
		{
			to = (t_token *)tokens->data;
			if (syntax_check_stack(to, stack))
				return (1);
			last = tokens;
			tokens = tokens->next;
		}
		if (SYNTAX_IS_EOL(stack->sym))
			return (0);
		if ((stack->sym > period && stack->sym < greater) &&
			(tokens = syntax_get_next_token_node(stack)))
		{
			last->next = tokens;
			continue ;
		}
		else
			return (1);
	}
}

int				syntax_check(t_lst *tokens)
{
	t_token	stack;

	if (tokens == (t_lst *)0)
		return (1);
	stack = (t_token){.s = (((t_token *)tokens->data)->s),
						.sym = (((t_token *)tokens->data)->sym),
						.symbreak = 0};
	return (syntax_check_(&stack, tokens));
}
