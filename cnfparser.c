#include "head.h"
#define BUFF_SIZE 256
#define PATH_SIZE 256

// 改我
char base_path[PATH_SIZE] = "C:\\Users\\vince\\Desktop\\程序设计课设\\指导\\SAT测试备选算例";

status SkipComments(char* buff, FILE* fp);                                    // 跳过comment
status GetPInfo(char* buff, int* var_num, int* clause_num);                   // 获得p行中记录的子句和变元数量
ClauseNode* ReadClauses(char* buff, FILE* fp, int* var_num, int clause_num);  // 读取并储存子句
ClauseNode* CreateClauseUnit(char* buff, FILE* fp);                           // 创建单个子句
LiteralNode* CreateLiteralUnit(char* buff);                                   // 创建一个文本单元

ClauseNode* CreateClause(int* var_num, char* filename)
{
    int clause_num;
    char buff[BUFF_SIZE];
    ClauseNode* s;

    // 读取文件
    strcat(base_path, filename);         // 获得文件最终路径
    FILE* fp = fopen(base_path, "r");
    if(!fp)
    {
        printf("   路径都输不对还好意思抄代码？\n");
        return ERROR;
    }

    // 处理文件
    SkipComments(buff, fp);
    GetPInfo(buff, var_num, &clause_num);
    s = ReadClauses(buff, fp, var_num, clause_num);

    fclose(fp);
    return s;
}

status SkipComments(char* buff, FILE* fp)
{
    while(fgets(buff, BUFF_SIZE, fp))
    {
        if(buff[0] == 'p')  break;
    }
    return OK;
}

status GetPInfo(char* buff, int* var_num, int* clause_num)
{
    // p cnf var_num clause_num
    char* token = strtok(buff, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    *var_num = atoi(token);
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
    LiteralNode *e, *tmp;

    // 创建子句开头
    c = (ClauseNode *)malloc(sizeof(ClauseNode));
    c->right = NULL;
    c->down = NULL;


    // 保存第一个元素
    fscanf(fp, "%s", buff);
    c->right = CreateLiteralUnit(buff);
    tmp = c->right;

    // 保存剩下的元素
    fscanf(fp, "%s", buff);
    while(strcmp(buff, "0"))
    {
        e = CreateLiteralUnit(buff);
        tmp->right = e;
        tmp = tmp->right;
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
