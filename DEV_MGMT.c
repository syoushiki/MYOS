#include<stdio.h>
#include<string.h>

char word[1024];
int index = 0;
int flag = 1;

void E();
void EO();
void T();
void TO();
void F();

void E() {
    T();
    EO();
}

void EO() {
    // 如果当前字符是加号，递归处理
    if (word[index] == '+') {
        index++;
        T();
        EO();
    }
}

void T() {
    F();
    TO();
}

void TO() {
    // 如果当前字符是乘号，递归处理
    if (word[index] == '*') {
        index++;
        F();
        TO();  // 修复了TO递归调用缺失括号的问题
    }
}

void F() {
    // 如果是字母 'i'，处理为基本元素
    if (word[index] == 'i') {
        index++;
    }
    // 如果是左括号 '('，递归处理表达式
    else if (word[index] == '(') {
        index++;
        E();
        // 如果遇到右括号 ')', 跳过
        if (word[index] == ')') {
            index++;
        } else {
            flag = 0;
            printf("\nError: Missing closing parenthesis!\n");
        }
    }
    // 如果都不是，表示输入错误
    else {
        flag = 0;
        printf("Error: Invalid character '%c'!\n", word[index]);
    }
}

int main() {
    int len;
    int k, i;

    // 用户输入测试案例的数量
    printf("Please enter the number of test cases: ");
    scanf("%d", &k);

    for (i = 0; i < k; i++) {
        printf("Enter the expression to test: \n");
        len = 0;
        flag = 1;

        // 输入单个表达式
        scanf("%s", word);

        len = strlen(word);
        word[len] = '\0';  // 确保字符串以 '\0' 结尾

        // 调用语法分析函数
        E();

        // 如果没有出错，输出解析成功的表达式
        if (flag)
            printf("%s is a valid expression.\n", word);
        else
            printf("%s is not a valid expression.\n", word);

        // 重置全局变量，准备下一轮测试
        index = 0;
    }

    return 0;
}

