%{
    #include <stdio.h>
    
    int num_lines = 1;  // 假设至少有一行内容
    int num_chars = 0;
    %}
    
    %%
    
    \n              { num_lines++; }  // 每遇到换行符，行数增加
    .               { num_chars++; }  // 每遇到一个字符，字符数增加
    
    %%
    
    int main() {
        yylex();  // 调用yylex来开始分析
        printf("Lines=%d,Chars=%d", num_lines, num_chars);  // 输出行数和字符数
        return 0;
    }
    