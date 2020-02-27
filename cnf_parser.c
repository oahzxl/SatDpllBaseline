#include "head.h"
#define BUFF_SIZE 256

status GetPInfo(char* buff, int* var_num, int* clause_num, FILE* fp);                   // ���p���м�¼���Ӿ�ͱ�Ԫ����
ClauseNode* ReadClauses(char* buff, FILE* fp, int* var_num, int clause_num);  // ��ȡ�������Ӿ�
ClauseNode* CreateClauseUnit(char* buff, FILE* fp);                           // ���������Ӿ�
LiteralNode* CreateLiteralUnit(char* buff);                                   // ����һ���ı���Ԫ

ClauseNode* CnfParser(int* literal_num, char* filename, int init)
{
    int clause_num;
    char buff[BUFF_SIZE];
    ClauseNode* s;

    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        printf("   ·������\n");
        return ERROR;
    }

    // �����ļ�
    GetPInfo(buff, literal_num, &clause_num, fp);        // ����Ӿ�������������
    if(init)    return NULL;                             // ��ʼ������ʱֻ��Ҫ������ ֱ�ӷ��ؼ���
    s = ReadClauses(buff, fp, literal_num, clause_num);  // �����Ӿ���

    fclose(fp);
    return s;
}

status GetPInfo(char* buff, int* literal_num, int* clause_num, FILE* fp)
{
    // ������ע�Ͳ���
    while(fgets(buff, BUFF_SIZE, fp))
    {
        if(buff[0] == 'p')  break;
    }

    // ���p�е����ݣ���buff�е������Կո���ָ����Ҳ����ֱ�Ӹ������ȡ����
    char* token = strtok(buff, " ");
    token = strtok(NULL, " ");    // ÿ����һ��strtok������һ��������
    token = strtok(NULL, " ");
    *literal_num = atoi(token);   // �ַ���ת����
    token = strtok(NULL, " ");
    *clause_num = atoi(token);
    return OK;
}

ClauseNode* ReadClauses(char* buff, FILE* fp, int* var_num, int clause_num)
{
    ClauseNode *s, *tmp;

    // ����root
    s = (ClauseNode *)malloc(sizeof(ClauseNode));
    s->right = NULL;
    tmp = s;

    // ѭ����ȡ�Ӿ�
    for(int i = 0; i < clause_num; i++)
    {
        tmp->down = CreateClauseUnit(buff, fp);
        tmp = tmp->down;
    }
    return s;
}

ClauseNode* CreateClauseUnit(char* buff, FILE* fp)
{
    ClauseNode *c;
    LiteralNode *e, *l_tmp;

    // �����Ӿ俪ͷ
    c = (ClauseNode *)malloc(sizeof(ClauseNode));
    c->right = NULL;
    c->down = NULL;

    // �����һ��Ԫ��
    fscanf(fp, "%s", buff);
    c->right = CreateLiteralUnit(buff);
    l_tmp = c->right;

    // ����ʣ�µ�Ԫ��
    fscanf(fp, "%s", buff);
    while(strcmp(buff, "0"))
    {
        e = CreateLiteralUnit(buff);
        l_tmp->right = e;
        l_tmp = l_tmp->right;
        fscanf(fp, "%s", buff);
    }
    return c;
}

LiteralNode* CreateLiteralUnit(char* buff)
{
    LiteralNode *e;
    e = (LiteralNode *)malloc(sizeof(LiteralNode));
    e->data = atoi(buff);
    e->right = NULL;
    return e;
}