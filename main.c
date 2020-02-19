#include "head.h"

int main()
{
    int op = 1, var_num, result;
    while(op)
    {
newone:
        system("cls");
        printf("\n\n");
        printf("         基于SAT的二进制数独游戏求解程序\n");
        printf("-------------------------------------------------\n");
        printf("     1. 随便写一个       2. 这个干脆不写了\n");
        printf("     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~2]: ");
        scanf("%d",&op);

        // 基本SAT任务求解
        if(op == 1)
        {
            clock_t start_time, end_time;
            start_time = clock();  // 开始计时

            // 这里可以选择要运行的算例
            char filename[] =  "\\基准算例\\性能测试\\ais10.cnf";
            //char filename[] = "\\基准算例\\功能测试\\unsat-5cnf-30.cnf";
            //char filename[] = "\\不满足算例\\u-problem10-100.cnf";
            //char filename[] = "\\满足算例\\L\\ec-iso-ukn009.shuffled-as.sat05-3632-1584.cnf";
            //char filename[] = "\\基准算例\\性能测试\\sud00009.cnf";

            // 读取文件，初始化
            ClauseNode* f = CreateClause(&var_num, filename);        // 先读取cnf文件，创建Clause
            int truth_table[var_num];                                // 真值表
            for(int k = 0; k < var_num; k++)    truth_table[k] = 1;  // 初始化真值表，这里把初始值设为1，也可改为0
            // Print(s);                                             // 此句能可视化数据

            // dpll
            result = DPLL(f, truth_table);
            end_time = clock();  // 结束计时

            // 输出结果
            printf("\n    耗时: %ldms\n", end_time - start_time);
            if(result)  printf("\n    结果：嘿嘿嘿~\n");  // 有解
            else    printf("\n    结果：你在想屁吃!\n");  // 无解

            // 打印真值为1的变元
            printf("\n    解为：");
            for(int k = 0; k < var_num; k++)    if(truth_table[k])  printf("%d ", k + 1);
            Get2Char();
        }

        else if(op == 0) break;

        else
        {
            printf("\n    会不会选？给爷爬！\n");
            Get2Char();
            goto newone;
        }
    }
    return OK;
}

void Get2Char()
{
    getchar();
    getchar();
}
