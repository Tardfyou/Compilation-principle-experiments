#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token结构体
typedef struct Token {
    int syn;            // 种别码
    char token[100];    // 符号
    int num;            // 数值(-1表示非数值类型)
} Token;

// 关键字映射表
typedef struct Keyword {
    char keyword[20];
    int syn;
} Keyword;

// 运算符和分隔符映射表
typedef struct Operator {
    char op[10];
    int syn;
} Operator;

Keyword keywords[] = {
    {"begin", 1}, {"if", 2}, {"then", 3}, {"while", 4}, {"do", 5}, {"end", 6}
};

Operator operators[] = {
    {":", 17}, {":=", 18}, {"+", 13}, {"-", 14}, {"*", 15}, {"/", 16},
    {"<", 20}, {"<=", 22}, {"<>", 21}, {">", 23}, {">=", 24}, {"=", 25},
    {";", 26}, {"(", 27}, {")", 28}, {"#", 0}
};

int keywords_count = 6;
int operators_count = 15;

// 判断是否为字母
int isLetter(char c) {
    return isalpha(c) || c == '_';
}

// 跳过空白字符
void skipWhitespace(char *input, int *pos) {
    while (input[*pos] != '\0' && isspace(input[*pos])) {
        (*pos)++;
    }
}

// 获取运算符或分隔符
char* getOperator(char *input, int *pos) {
    static char op[10];
    int i = 0;
    char c = input[*pos];

    // 检查双字符运算符
    if (input[*pos + 1] != '\0') {
        char twoChar[3] = {input[*pos], input[*pos + 1], '\0'};
        for (int i = 0; i < operators_count; i++) {
            if (strcmp(twoChar, operators[i].op) == 0) {
                (*pos) += 2;
                strcpy(op, twoChar);
                return op;
            }
        }
    }

    // 单字符运算符
    op[0] = c;
    op[1] = '\0';
    (*pos)++;
    return op;
}

// 判断是否是关键字
int isKeyword(char *word) {
    for (int i = 0; i < keywords_count; i++) {
        if (strcmp(word, keywords[i].keyword) == 0) {
            return keywords[i].syn;
        }
    }
    return -1;
}

// 主要词法分析函数
void analyze(char *input) {
    int pos = 0;
    Token tokens[100];
    int tokenCount = 0;
    int flag = 0; // flag 初始为 0，不输出

    while (input[pos] != '\0') {
        skipWhitespace(input, &pos);

        if (input[pos] == '\0') break;

        char c = input[pos];

        // 识别数字
        if (isdigit(c)) {
            char num[20];
            int i = 0;
            while (isdigit(input[pos])) {
                num[i++] = input[pos++];
            }
            num[i] = '\0';
            tokens[tokenCount].syn = 11;  // 数字的种别码是11
            strcpy(tokens[tokenCount].token, num);
            tokens[tokenCount].num = atoi(num);
            tokenCount++;
        }

        // 识别标识符和关键字
        else if (isLetter(c)) {
            char word[100];
            int i = 0;
            while (isLetter(input[pos]) || isdigit(input[pos])) {
                word[i++] = input[pos++];
            }
            word[i] = '\0';

            int keywordSyn = isKeyword(word);
            if (keywordSyn != -1) {
                tokens[tokenCount].syn = keywordSyn;
                strcpy(tokens[tokenCount].token, word);
                tokens[tokenCount].num = -1;

                // 如果是 begin，设置 flag 为 1
                if (keywordSyn == 1) {
                    flag = 1;
                }
            } else {
                tokens[tokenCount].syn = 10;  // 标识符的种别码是10
                strcpy(tokens[tokenCount].token, word);
                tokens[tokenCount].num = -1;
            }
            tokenCount++;
        }
        // 识别运算符和分隔符
        else {
            char *op = getOperator(input, &pos);
            int found = 0;
            for (int i = 0; i < operators_count; i++) {
                if (strcmp(op, operators[i].op) == 0) {
                    tokens[tokenCount].syn = operators[i].syn;
                    strcpy(tokens[tokenCount].token, op);
                    tokens[tokenCount].num = -1;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                // 未识别的字符不输出
                continue;
            }
            tokenCount++;
        }
    }

    // 输出分析结果
    for (int i = 0; i < tokenCount; i++) {
        // 如果 flag 为 1，才开始输出
        if (flag == 1) {
            if (i == tokenCount - 1) {
                // 最后一个 token 不输出换行符
                printf("(%d,%s)", tokens[i].syn, tokens[i].token);
            } else {
                // 其他 token 输出换行符
                printf("(%d,%s)\n", tokens[i].syn, tokens[i].token);
            }
        }
    }
}

int main() {
    char userInput[1000] = ""; // 初始化为空
    char line[100];
    
    // 读取用户输入直到遇到 EOF
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // 跳过空白字符后检查这一行是否为空
        if (strspn(line, " \t\n") == strlen(line)) {
            continue;
        }

        strcat(userInput, line);
    }

    analyze(userInput);

    return 0;
}
