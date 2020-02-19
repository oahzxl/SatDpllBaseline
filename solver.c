#include "head.h"

ClauseNode* AddClause(ClauseNode* s, int var);        // 增加单子句
ClauseNode* IsUnitClause(ClauseNode* s);              // 判断是否存在单子句
ClauseNode* CopyS(ClauseNode* s);                     // 复制表
ClauseNode* CopyClause(ClauseNode* s);                // 复制一个子句
ClauseNode* DeleteClause(ClauseNode* s);              // 删除一个子句
ClauseNode* DeleteLiteral(ClauseNode* s, int var);    // 删除负变元
status Print(ClauseNode* s);                          // 打印表
status IsEmptyClause(ClauseNode* s);                  // 判断是否存在空子句
status EvaluateClause();                              // 评估句子真假状态
status RecordTruth(ClauseNode* s, int* truth_table);  // 记录单子句中元素的真值
status RemoveVar(ClauseNode* s, int var);             // 删除变元所在子句与负变元
int PickVar(ClauseNode* s);                           // 直接取第一个子句的第一个变元

status DPLL(ClauseNode* s, int* truth_table)
{
    ClauseNode* tmp = s;
    ClauseNode* unit_clause = IsUnitClause(tmp);      // 获得单子句

    // 单子句规则
    while(unit_clause)
    {
        RecordTruth(unit_clause, truth_table);        // 记录单子句中的变元的真值
        int var = unit_clause->right->data;
        RemoveVar(s, var);

        if(s->down == NULL) return FOUND;             // S为空
        else if(IsEmptyClause(s))   return NOTFOUND;  // S中存在空子句

        tmp = s;
        unit_clause = IsUnitClause(tmp);              // 还存在单子句则继续循环
    }

    // 分裂策略
    int var = PickVar(s);                                            // 选变元
    if(DPLL(AddClause(CopyS(s), var), truth_table))  return FOUND;   // 变元的真值选对了,此处传入S的拷贝
    return DPLL(AddClause(s, -var), truth_table);                    // 变元的真值选错了
}

ClauseNode* CopyS(ClauseNode* s)
{
    ClauseNode *s_new, *tmp, *s_origin = s->down;

    // 创建root
    s_new = (ClauseNode *)malloc(sizeof(ClauseNode));
    s_new->right = NULL;
    tmp = s_new;

    // 循环读取子句
    while(s_origin)
    {
        tmp->down = CopyClause(s_origin);
        tmp = tmp->down;
        s_origin = s_origin->down;
    }
    return s_new;
}

ClauseNode* CopyClause(ClauseNode* s)
{
    ClauseNode* clause_tmp;
    LiteralNode *literal_tmp, *s_tmp = s->right;

    // 创建子句结点
    clause_tmp = (ClauseNode *)malloc(sizeof(ClauseNode));
    clause_tmp->down = NULL;
    clause_tmp->right = NULL;

    // 创建第一个文字结点
    if(s_tmp)
    {
        literal_tmp = (LiteralNode *)malloc(sizeof(LiteralNode));
        literal_tmp->data = s_tmp->data;
        literal_tmp->right = NULL;
        s_tmp = s_tmp->right;
        clause_tmp->right = literal_tmp;
    }

    // 循环创建文字结点
    while(s_tmp)
    {
        literal_tmp->right = (LiteralNode *)malloc(sizeof(LiteralNode));
        literal_tmp->right->data = s_tmp->data;
        literal_tmp->right->right = NULL;
        literal_tmp = literal_tmp->right;
        s_tmp = s_tmp->right;
    }
    return clause_tmp;
}

ClauseNode* AddClause(ClauseNode* s, int var)
{
    ClauseNode* tmp;
    tmp = (ClauseNode *)malloc(sizeof(ClauseNode));
    tmp->right = (LiteralNode *)malloc(sizeof(LiteralNode));
    tmp->right->data = var;
    tmp->right->right = NULL;
    tmp->down = s->down;
    s->down = tmp;
    return s;
}

int PickVar(ClauseNode* s)
{
    int var = s->down->right->data;
    return var;
}

status RemoveVar(ClauseNode* s, int var)
{
    ClauseNode *c_tmp = s->down, *last_c_tmp = s;
    while(c_tmp)                                     // 循环读取每一个子句
    {
        c_tmp = DeleteLiteral(c_tmp, var);           // 删除负变元，如果遇到了正变元则返回NULL
        if(c_tmp == NULL)
        {
            c_tmp = DeleteClause(last_c_tmp->down);  // 与正变元相同则删除子句
            last_c_tmp->down = c_tmp;
            continue;
        }
        if(c_tmp == NULL)   break;
        last_c_tmp = c_tmp;  // 下一个子句
        c_tmp = c_tmp->down;
    }
    return OK;
}

ClauseNode* DeleteClause(ClauseNode* s)
{
    ClauseNode* tmp = s;
    LiteralNode* l;
    while(s->right)                   // 先删除子句中所有文字
    {
        l = s->right;
        s->right = s->right->right;
        free(l);
    }
    s = s->down;
    free(tmp);                        // 再删除空子句
    return s;
}

ClauseNode* DeleteLiteral(ClauseNode* s, int var)
{
    LiteralNode *l = s->right, *tmp;

    // 处理第一个文字
    if(l->data == -var)                  // 负变元
    {
        tmp = s->right->right;
        free(l);
        s->right = tmp;
        return s;
    }
    else if(l->data == var) return NULL; // 正变元

    // 循环处理接下来的文字
    while(l)
    {
        if(l->right && l->right->data == -var)
        {
            tmp = l->right;
            l->right = l->right->right;
            free(tmp);
        }
        else if(l->right && l->right->data == var)  return NULL;
        l = l->right;
    }
    return s;
}

status IsEmptyClause(ClauseNode* s)
{
    ClauseNode* tmp = s->down;
    while(tmp)
    {
        if(!tmp->right) return YES;
        tmp = tmp->down;
    }
    return NO;
}

status RecordTruth(ClauseNode* s, int* truth_table)
{
    int idx = abs(s->right->data) - 1;   // 序号=绝对值-1
    if(s->right->data > 0)  truth_table[idx] = 1;
    else    truth_table[idx] = 0;
    return OK;
}

ClauseNode* IsUnitClause(ClauseNode* s)
{
    ClauseNode* tmp = s;
    while(tmp)
    {
        if(tmp->right && !tmp->right->right)    return tmp;
        tmp = tmp->down;
    }
    return NULL;
}

status Print(ClauseNode* s)
{
    ClauseNode* c_tmp = s->down;
    LiteralNode* l_tmp;
    printf("\n-----Start-----\n");
    while(c_tmp)
    {
        l_tmp = c_tmp->right;
        while(l_tmp)
        {
            printf("%d ", l_tmp->data);
            l_tmp = l_tmp->right;
        }
        if(c_tmp->right)    printf("\n");
        else    printf("空\n");
        c_tmp = c_tmp->down;
    }
    printf("------End------\n");
    return OK;
}
