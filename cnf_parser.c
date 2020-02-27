#include "head.h"
#define BUFF_SIZE 256

status GetPInfo(char* buff, int* var_num, int* clause_num, FILE* fp);                   // 获得p行中记录的子句和变元数量
ClauseNode* ReadClauses(char* buff, FILE* fp, int* var_num, int clause_num);  // 读取并储存子句
ClauseNode* CreateClauseUnit(char* buff, FILE* fp);                           // 创建单个子句
LiteralNode* CreateLiteralUnit(char* buff);                                   // 创建一个文本单元

ClauseNode* CnfParser(int* literal_num, char* filename, int init)
{
    int clause_num;
    char buff[BUFF_SIZE];
    ClauseNode* s;

    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        printf("   路径错误！\n");
        return ERROR;
    }

    // 处理文件
    GetPInfo(buff, literal_num, &clause_num, fp);        // 获得子句数，文字数字
    if(init)    return NULL;                             // 初始化数组时只需要文字数 直接返回即可
    s = ReadClauses(buff, fp, literal_num, clause_num);  // 加载子句数

    fclose(fp);
    return s;
}

status GetPInfo(char* buff, int* literal_num, int* clause_num, FILE* fp)
{
    // 先跳过注释部分
    while(fgets(buff, BUFF_SIZE, fp))
    {
        if(buff[0] == 'p')  break;
    }

    // 获得p行的数据，将buff中的内容以空格符分割，这里也可以直接根据序号取数字
    char* token = strtok(buff, " ");
    token = strtok(NULL, " ");    // 每调用一次strtok，返回一部分内容
    token = strtok(NULL, " ");
    *literal_num = atoi(token);   // 字符串转数字
    token = strtok(NULL, " ");
    *clause_num = atoi(token);
    return OK;
}

ClauseNode* ReadClauses(char* buff, FILE* fp, int* var_num, int clause_num)
{
    ClauseNode *s, *tmp;

    // 创建root
    s = (ClauseNode *)malloc(sizeof(ClauseNode));
    s->right = NULL;
    tmp = s;

    // 循环读取子句
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

    // 创建子句开头
    c = (ClauseNode *)malloc(sizeof(ClauseNode));
    c->right = NULL;
    c->down = NULL;

    // 保存第一个元素
    fscanf(fp, "%s", buff);
    c->right = CreateLiteralUnit(buff);
    l_tmp = c->right;

    // 保存剩下的元素
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
