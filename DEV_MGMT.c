#include<stdio.h>
#include<string.h>
char word[1024];
int index=0;
int flag = 1;
void E();
void EO();
void T();
void TO();
void F();
void E()
{
	T();
	EO();
}
void EO()
{
	if(word[index]=='+')
	{
		index++;
		T();
		EO();
	}
}
void T()
{
	F();
	TO();	
}
void TO()
{
	if(word[index]=='*')
	{
		index++;
		F();
		TO;
	}
}
void F()
{
	if(word[index]=='i')
	 index++;
	else
	if(word[index]=='(')
	{
		index++;
		E();
		if(word[index]==')')
		index++;
		else
		{
			flag=0;
			printf("\nerror!\n");
		}
	}
	else
	{
		flag=0;
		printf("error!\n");
	}
}
int main()
{
	int len;
	int k,i;
	char ch;
	printf("请输入要测试的次数：");
	scanf("%d",&k);
	for(i=0;i<k;i++)
	{
		printf("请输入字符串：\n");
		len=0;
		flag=1;
		scanf("%s",word);
		len=strlen(word);
		word[len+1]='\0';
		E();
		if(flag)
		printf("%s为合法字符串！\n",word);
		strcpy(word,"");
		index=0;
		
	}
	return 0;
 } 
