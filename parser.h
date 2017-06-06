
#ifndef PARSER_H
# define PARSER_H

# include "lst.h"
# include "hashlib.h"

# define PA_ERROR	-2
# define PA_ACCEPT	-1
# define PA_WAIT	0 /*** default state ***/

# define RULE		struct s_rule

# define SYNTAX 	struct s_syntax

# define SY_IFRT	0x00f
# define SY_LITERAL	0x000 /*** raw rule ***/
# define SY_RULE	0x001 /*** recursive rule ***/

# define SY_OCCF	0xf00
# define SY_NONE	0x000 /*** at least one ***/
# define SY_QMARK	0x100 /*** zero or one ***/
# define SY_WSTAR	0x200 /*** 0 or more ***/
# define SY_PLUS	0x300 /*** 1 or more ***/

extern HASH_TABLE	*g_hrule;
extern t_lst		*g_stack;
extern int			g_pstate;

struct				s_rule
{
	char	*id;
	t_lst	**syntax;
	int		nrules;
}

struct				s_syntax
{
	void	*rule;
	int		flag;
};

struct				s_rule_state
{
	RULE	*rule;
	t_lst	*crule;
	int		csyntax;
};

#endif
