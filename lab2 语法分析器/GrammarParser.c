#include <stdio.h>
#include <string.h>
#include <ctype.h>

// 定义关键字
#define KEYWORDS_COUNT 6
const char *keywords[KEYWORDS_COUNT] = {"begin", "end", "if", "then", "while", "do"};

// 定义运算符和分隔符
#define SYMBOLS_COUNT 12
const char *symbols[SYMBOLS_COUNT] = {"+", "-", "*", "/", "<", "<=", "=", "<>", ">=", ">", ":=", ";"};

// 全局变量
char input[1000]; // 输入缓冲区
int pos = 0;      // 当前读取位置

// 前置声明函数
void expression(int indent);
void statement(int indent);
void statement_list(int indent);

// 辅助函数：跳过空白字符
void skip_whitespace() {
    // 跳过所有空白字符，包括空格、制表符、换行符
    while (isspace(input[pos])) {
        pos++;
    }
}

// 辅助函数：读取一个单词
void read_word(char *word) {
    int i = 0;
    // 读取一个单词，直到遇到非字母数字或下划线字符
    while (isalnum(input[pos]) || input[pos] == '_') {
        word[i++] = input[pos++];
    }
    word[i] = '\0';  // 结束字符
}

// 辅助函数：判断是否为关键字
int is_keyword(const char *str) {
    // 判断当前字符串是否为预定义的关键字之一
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (strncmp(str, keywords[i], strlen(keywords[i])) == 0) {
            return 1;  // 是关键字
        }
    }
    return 0;  // 不是关键字
}

// 语法分析函数：因子
void factor(int indent) {
    skip_whitespace();  // 跳过空白字符
    
    // 如果是数字，处理数字并输出
    if (isdigit(input[pos])) {
        char num[100];
        int i = 0;
        while (isdigit(input[pos])) {
            num[i++] = input[pos++];  // 读取数字
        }
        num[i] = '\0';  // 字符串结束
        printf("%*s因子: %s\n", indent, "", num);  // 打印因子（数字）
    } else if (isalpha(input[pos])) {  // 如果是标识符
        char id[100];
        read_word(id);  // 读取标识符
        printf("%*s因子: %s\n", indent, "", id);  // 打印标识符
    } else if (input[pos] == '(') {  // 如果是左括号，处理括号中的表达式
        printf("%*s因子: (\n", indent, "");
        pos++;
        expression(indent + 4);  // 递归处理括号内的表达式
        skip_whitespace();
        if (input[pos] == ')') {  // 匹配右括号
            printf("%*s)\n", indent, "");
            pos++;
        } else {
            printf("错误：缺少右括号\n");  // 报错：缺少右括号
        }
    } else {
        printf("错误：无效的因子\n");  // 其他无效的输入
    }
}

// 语法分析函数：项
void term(int indent) {
    printf("%*s项:\n", indent, "");
    factor(indent + 4);  // 处理因子
    
    skip_whitespace();
    while (input[pos] == '*' || input[pos] == '/') {  // 处理乘除法运算
        char op[2] = {input[pos], '\0'};  // 存储操作符
        pos++;
        printf("%*s操作符: %s\n", indent + 4, "", op);  // 打印操作符
        factor(indent + 4);  // 处理因子
        skip_whitespace();
    }
}

// 语法分析函数：表达式
void expression(int indent) {
    printf("%*s表达式:\n", indent, "");
    term(indent + 4);  // 处理项
    
    skip_whitespace();
    while (input[pos] == '+' || input[pos] == '-') {  // 处理加减法运算
        char op[2] = {input[pos], '\0'};  // 存储操作符
        pos++;
        printf("%*s操作符: %s\n", indent + 4, "", op);  // 打印操作符
        term(indent + 4);  // 处理项
        skip_whitespace();
    }
}

// 语法分析函数：逻辑运算
void logical_expression(int indent) {
    printf("%*s逻辑运算:\n", indent, "");
    expression(indent + 4);  // 处理表达式
    
    skip_whitespace();
    if (input[pos] == '<' || input[pos] == '>' || input[pos] == '=') {  // 处理关系运算符
        char op[3] = {0};
        if (input[pos] == '<' && input[pos+1] == '=') {  // <= 运算符
            op[0] = '<';
            op[1] = '=';
            op[2] = '\0';
            pos += 2;
        } else if (input[pos] == '>' && input[pos+1] == '=') {  // >= 运算符
            op[0] = '>';
            op[1] = '=';
            op[2] = '\0';
            pos += 2;
        } else if (input[pos] == '<' && input[pos+1] == '>') {  // <> 运算符
            op[0] = '<';
            op[1] = '>';
            op[2] = '\0';
            pos += 2;
        } else {  // 普通的关系运算符
            op[0] = input[pos];
            op[1] = '\0';
            pos++;
        }
        printf("%*s逻辑运算符: %s\n", indent + 4, "", op);  // 打印逻辑运算符
        expression(indent + 4);  // 处理右侧的表达式
    }
}

// 语法分析函数：赋值语句
void assignment_statement(int indent) {
    printf("%*s赋值语句:\n", indent, "");
    
    char id[100];
    read_word(id);  // 读取标识符
    printf("%*s标识符: %s\n", indent + 4, "", id);  // 打印标识符
    
    skip_whitespace();
    if (input[pos] == ':' && input[pos + 1] == '=') {  // 判断是否为赋值操作符 ":="
        pos += 2;
        skip_whitespace();
        expression(indent + 4);  // 处理右侧的表达式
    } else {
        printf("错误：缺少赋值符号 :=\n");  // 报错：缺少赋值符号
    }
}

// 语法分析函数：条件语句（if）
void if_statement(int indent) {
    printf("%*s条件语句:\n", indent, "");

    pos += 2;  // 跳过 "if"
    skip_whitespace();

    logical_expression(indent + 4);  // 处理条件表达式
    skip_whitespace();

    if (strncmp(&input[pos], "then", 4) == 0) {  // 如果匹配 "then"
        pos += 4;
        skip_whitespace();

        statement_list(indent + 4);  // 处理语句块

        skip_whitespace();
        if (strncmp(&input[pos], "end", 3) == 0) {  // 如果匹配 "end"
            pos += 3;
            printf("%*s结束条件语句\n", indent, "");
        } else {
            printf("%*s错误：缺少 end\n", indent, "");
        }
    } else {
        printf("错误：缺少 then\n");  // 报错：缺少 "then"
    }
}

// 语法分析函数：循环语句（while）
void while_statement(int indent) {
    printf("%*s循环语句:\n", indent, "");

    pos += 5;  // 跳过 "while"
    skip_whitespace();

    logical_expression(indent + 4);  // 处理循环条件表达式
    skip_whitespace();

    if (strncmp(&input[pos], "do", 2) == 0) {  // 如果匹配 "do"
        pos += 2;
        skip_whitespace();

        statement_list(indent + 4);  // 处理语句块

        skip_whitespace();
        if (strncmp(&input[pos], "end", 3) == 0) {  // 如果匹配 "end"
            pos += 3;
            printf("%*s结束循环语句\n", indent, "");
        } else {
            printf("%*s错误：缺少 end\n", indent, "");
        }
    } else {
        printf("错误：缺少 do\n");  // 报错：缺少 "do"
    }
}

// 语法分析函数：语句
void statement(int indent) {
    skip_whitespace();
    
    if (strncmp(&input[pos], "if", 2) == 0 && 
        (input[pos+2] == ' ' || input[pos+2] == '\t' || input[pos+2] == '\n')) {
        if_statement(indent);  // 处理 if 语句
    } else if (strncmp(&input[pos], "while", 5) == 0 && 
               (input[pos+5] == ' ' || input[pos+5] == '\t' || input[pos+5] == '\n')) {
        while_statement(indent);  // 处理 while 语句
    } else if (isalpha(input[pos])) {  // 如果是标识符，处理赋值语句
        assignment_statement(indent);
    } else {
        printf("错误：无效的语句\n");  // 其他无效的语句
        while (input[pos] != ';' && strncmp(&input[pos], "end", 3) != 0 && input[pos] != '\0') {
            pos++;
        }
    }
}

// 语法分析函数：语句串
void statement_list(int indent) {
    printf("%*s语句串:\n", indent, "");
    skip_whitespace();

    while (1) {
        // 遇到 end 或文件结束就结束语句串
        if (strncmp(&input[pos], "end", 3) == 0 || input[pos] == '\0') {
            return;
        }

        statement(indent + 4);  // 处理当前语句
        skip_whitespace();

        // 如果有分号就跳过它
        if (input[pos] == ';') {
            pos++;
            skip_whitespace();
        } else {
            // 没有分号也尝试猜测是否为新语句的开始
            if (strncmp(&input[pos], "if", 2) == 0 ||
                strncmp(&input[pos], "while", 5) == 0 ||
                isalpha(input[pos])) {
                continue;  // 新语句，继续循环
            } else {
                break;  // 否则认为语句结束
            }
        }
    }
}

// 语法分析函数：程序
void program() {
    printf("程序:\n");
    
    skip_whitespace();
    if (strncmp(&input[pos], "begin", 5) == 0) {  // 检查程序是否以 "begin" 开始
        pos += 5;
        skip_whitespace();
        statement_list(4);  // 分析语句串
        
        skip_whitespace();
        if (strncmp(&input[pos], "end", 3) == 0) {  // 检查程序是否以 "end" 结束
            pos += 3;
            printf("结束\n");
        } else {
            printf("错误：缺少 end");
        }
    } else {
        printf("错误1-1：缺少 begin");
    }
}

// 主函数
int main() {
    int ch, i = 0;
    
    // 读取输入直到EOF
    while ((ch = getchar()) != EOF && i < 999) {
        input[i++] = ch;
    }
    input[i] = '\0';
    
    // 分析程序
    program();
    
    return 0;
}
