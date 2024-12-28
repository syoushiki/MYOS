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
    // �����ǰ�ַ��ǼӺţ��ݹ鴦��
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
    // �����ǰ�ַ��ǳ˺ţ��ݹ鴦��
    if (word[index] == '*') {
        index++;
        F();
        TO();  // �޸���TO�ݹ����ȱʧ���ŵ�����
    }
}

void F() {
    // �������ĸ 'i'������Ϊ����Ԫ��
    if (word[index] == 'i') {
        index++;
    }
    // ����������� '('���ݹ鴦����ʽ
    else if (word[index] == '(') {
        index++;
        E();
        // ������������� ')', ����
        if (word[index] == ')') {
            index++;
        } else {
            flag = 0;
            printf("\nError: Missing closing parenthesis!\n");
        }
    }
    // ��������ǣ���ʾ�������
    else {
        flag = 0;
        printf("Error: Invalid character '%c'!\n", word[index]);
    }
}

int main() {
    int len;
    int k, i;

    // �û�������԰���������
    printf("Please enter the number of test cases: ");
    scanf("%d", &k);

    for (i = 0; i < k; i++) {
        printf("Enter the expression to test: \n");
        len = 0;
        flag = 1;

        // ���뵥�����ʽ
        scanf("%s", word);

        len = strlen(word);
        word[len] = '\0';  // ȷ���ַ����� '\0' ��β

        // �����﷨��������
        E();

        // ���û�г�����������ɹ��ı��ʽ
        if (flag)
            printf("%s is a valid expression.\n", word);
        else
            printf("%s is not a valid expression.\n", word);

        // ����ȫ�ֱ�����׼����һ�ֲ���
        index = 0;
    }

    return 0;
}

