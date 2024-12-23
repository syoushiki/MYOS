#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define Maxline  1024

//-----全局变量------
char keyword[][10] = { "auto","bool","break","case","char","const","continue","default","do"
,"double","else","enum","extern","float","for","goto","if","int","long","main","register","return"
,"short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile"
,"while","scanf","printf","include","define" };	//38个保留字（关键字）
char delimiter[][10] = { "(",")","[","]","{","}",".",",",";","'","#","?",'"', };//13个界符
char calculation[][10] = { "[+]","[-]","[*]","[/]","[%]","[++]","[--]","[==]","[!=]","[>]","[<]","[>=]"
,"[<=]","[&&]","[||]","[!]","[=]","[+=]","[-=]","[*=]","[/=]","[%=]","[&=]","[^=]","[|=]","[&]","[|]"
,"[^]","[~]","[<<]","[>>]" };//31个运算符
int nk = 38, nd = 13, nc = 31;//分别表示关键字、界符、运算符的个数
//下面分别是关键字、界符、运算符、整型、浮点数和标识符在单词表中的偏移量
int ck = 1, cd = 70, cc = 39, ci = 83, cf = 84, cn = 85;

//------函数声明------
bool pd_integer(char ch);	    //判断是否是整数
bool pd_character(char ch);		//判断是否是字母
int  pd_keyword(char s[]);		//判断字符串是否是保留字（关键字）是则返回下标，否则返回-1
int  pd_delimiter(char ch);     //判断字符是否是界符，是返回下标，否则返回-1
int  pd_calculation(char s[]);	//判断字符串是否是运算符，是返回下标，否则返回-1
char* pd_spcode(int n);         //根据种别码的范围判断类别
int search_spcode(char s[]);	//查询字符串s的种别码

//------主函数------
int main() {
	char test[1030] = { 0 };
	FILE* fp = fopen("test.txt", "r");
	if (fp == NULL) {
		printf("打开文件失败!\n");
		return 0;
	}
	while (fgets(test, Maxline, fp) != NULL) {
		int i = 0, j = 0;
		while (i < strlen(test)) {
			if (test[i] == ' ' || test[i] == '\n' || test[i] == '\t') {
				i++;
				continue;//遇到空格或换行符，跳过
			}
			bool flag = true;
			char str[100] = { 0 };
			j = 0;
			if (pd_integer(test[i])) {
				//如果是数字，循环读取,包括小数点，因为可能是浮点数
				while (pd_integer(test[i]) || (test[i] == '.' && flag)) {
					if (test[i] == '.')flag = false;//浮点型只允许出现一个小数点
					str[j++] = test[i++];
				}
				i--;
			}
			else if (pd_character(test[i]) || test[i] == '_') {
				//如果是字母或下划线或数字（标识符可以有数字）
				while (pd_character(test[i]) || test[i] == '_' || pd_integer(test[i]))str[j++] = test[i++];
				i--;
			}
			else if (test[i] == '+' || test[i] == '-' || test[i] == '*' || test[i] == '/'
				|| test[i] == '%' || test[i] == '=' || test[i] == '!' || test[i] == '>'
				|| test[i] == '<' || test[i] == '&' || test[i] == '|' || test[i] == '^') {

				if (test[i + 1] == '=' || test[i] == test[i + 1]) {
					str[0] = test[i], str[1] = test[i + 1];
					i++;
				}
				else {
					str[0] = test[i];
				}
			}
			else {
				str[0] = test[i];
			}
			i++;
			printf("( %s , %d )------%s\n", str, search_spcode(str), pd_spcode(search_spcode(str)));

		}
	}
	return 0;
}

//------函数实现------
bool pd_integer(char ch) {//判断是否是整数
	if (ch >= '0' && ch <= '9')return true;
	return false;
}

bool pd_character(char ch) {//判断是否是字母
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))return true;
	return false;
}

int pd_keyword(char s[]) {//判断字符串是否是保留字（关键字）是则返回下标，否则返回-1
	for (int i = 0; i < nk; i++)
		if (strcmp(s, keyword[i]) == 0)return i;
	return -1;
}

int pd_delimiter(char ch) {//判断字符是否是界符，是返回下标，否则返回-1
	for (int i = 0; i < nd; i++)
		if (ch == delimiter[i][0])return i;
	return -1;

}
int pd_calculation(char s[]) {//判断字符是否是运算符，是返回下标，否则返回-1
	for (int i = 0; i < nc; i++) {
		if (strlen(calculation[i]) - 2 == strlen(s)) {//如果长度相等
			bool flag = true;
			for (int j = 1, k = 0; j < strlen(calculation[i]) - 1 && k < strlen(s); j++, k++)
				if (calculation[i][j] != s[k]) {
					flag = false;//有一个元素不等，标记后直接退出
					break;
				}
			if (flag)return i;//返回下标
		}

	}
	return -1;
}

char* pd_spcode(int n) {    //根据种别码的范围判断类别
	static char name[20];//静态局部变量
	if (n >= 1 && n <= nk) strcpy(name, "关键字");
	else if (n >= cc && n < cd) strcpy(name, "运算符");
	else if (n >= cd && n < ci) strcpy(name, "界符");
	else if (n == ci) strcpy(name, "整型");
	else if (n == cf)strcpy(name, "浮点型");
	else if (n == cn) strcpy(name, "标识符");
	else strcpy(name, "未识别");
	return name;
}

int search_spcode(char s[]) {	//查询字符串s的种别码
	if (pd_character(s[0])) {	//如果第一个字符是字母，说明是关键字或标识符
		if (pd_keyword(s) != -1) //说明是关键字
			return pd_keyword(s) + ck;//下标从0开始，要+1
		else return cn;	//否则就是标识符（标识符可以有下划线）
	}

	if (s[0] == '_')return cn;//下划线开头一定是标识符

	if (pd_integer(s[0])) {//开头是数字说明是整数或者浮点数
		if (strstr(s, ".") != NULL)return cf;//如果有小数点说明是浮点型
		return ci;//否则就是整型
	}

	if (strlen(s) == 1) { //长度为1，说明是界符或者运算符
		char ch = s[0];
		if (pd_delimiter(ch) != -1)//判断是否是界符
			return pd_delimiter(ch) + cd;
	}

	if (pd_calculation(s) != -1)//如果是运算符
		return pd_calculation(s) + cc;

	return -1;//否则就是未标识符

}
