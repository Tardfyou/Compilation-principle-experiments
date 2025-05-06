%{
    #include <stdio.h>
    #include <stdlib.h>
    %}
    
    digit   [0-9]
    letter  [a-zA-Z]
    
    %%
    "begin"    { printf("(1,begin)\n"); }
    "if"       { printf("(2,if)\n"); }
    "then"     { printf("(3,then)\n"); }
    "while"    { printf("(4,while)\n"); }
    "do"       { printf("(5,do)\n"); }
    "end"      { printf("(6,end)\n"); }
    ":"        { printf("(17,:)\n"); }
    ":="       { printf("(18,:=)\n"); }
    "+"        { printf("(13,+)\n"); }
    "-"        { printf("(14,-)\n"); }
    "*"        { printf("(15,*)\n"); }
    "/"        { printf("(16,/)\n"); }
    "<"        { printf("(20,<)\n"); }
    "<="       { printf("(22,<=)\n"); }
    "<>"       { printf("(21,<>)\n"); }
    ">"        { printf("(23,>)\n"); }
    ">="       { printf("(24,>=)\n"); }
    "="        { printf("(25,=)\n"); }
    ";"        { printf("(26,;)\n"); }
    "("        { printf("(27,()\n"); }
    ")"        { printf("(28,)\n"); }
    "#"        { printf("(0,#)\n"); }
    
    {letter}({letter}|{digit})*  { printf("(10,%s)\n", yytext); }
    {digit}+   { printf("(11,%s)\n", yytext); }
    
    [ \t\n]     ;  // 忽略空格、制表符和换行符
    
    .           { printf("Invalid token: %s\n", yytext); }
    
    %%
    int main() {
        yylex();  // 调用flex生成的词法分析器
        return 0;
    }
    