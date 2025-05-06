%{
#include "lab107.tab.h"
%}

%option noyywrap
%option yylineno

NUMSYM [0-9]+
EOLSYM \n
ADDSYM \+
SUBSYM \-
MULSYM \*
DIVSYM \/
EXPOSYM \^
LPSYM \(
RPSYM \)
WHITESYM [ \t]
UNKNOWSYM .

%%
{NUMSYM} {yylval=atof(yytext); printf("Type(%d):NUM Val=%.10g\n", NUM, yylval); return NUM;}
{EOLSYM} {return EOL;}
{ADDSYM} {printf("Type(%d):ADD\n", ADD); return ADD;}
{SUBSYM} {printf("Type(%d):SUB\n", SUB); return SUB;}
{MULSYM} {printf("Type(%d):MUL\n", MUL); return MUL;}
{DIVSYM} {printf("Type(%d):DIV\n", DIV); return DIV;}
{EXPOSYM} {printf("Type(%d):EXPO\n", EXPO); return EXPO;}
{LPSYM} {printf("Type(%d):LP\n", LP); return LP;}
{RPSYM} {printf("Type(%d):RP\n", RP); return RP;}
{WHITESYM} {/*skip the white space symbol*/}
{UNKNOWSYM} {/*printf("Character %s Unknow at line(%d:%d-%d:%d)\n",yytext,yylloc.first_line,yylloc.first_column,yylloc.last_line,yylloc.last_column);*/}
%%