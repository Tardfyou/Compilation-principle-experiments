%union
{
    int ival;
    /* Removed char *sval as it was only used for the useless nonterminal */
}

%token <ival>NUM
%nterm <ival>exp

/* Define precedence and associativity */
/* Make - lower precedence than + */
%left '-'
%left '+'

%%
/* Grammar rules */
exp:
    exp '+' exp
    | exp '-' exp
    | NUM
    ;
/* Removed useless: STR; rule */
%%