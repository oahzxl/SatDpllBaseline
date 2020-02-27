#include "head.h"
#define EPOCH 5             // 重复运行次数
#define PATH_SIZE 1024      // 路径字符串大小

char* ChooseExample();

int main()
{
    int op = 1, literal_num, result, i;
    char filename[PATH_SIZE];
    clock_t t, t1, t2;
    ClauseNode* s;

    while(op)
    {
newone:
        system("cls");
        printf("\n\n");
        printf("         基于SAT的二进制数独游戏求解程序\n");
        printf("-------------------------------------------------\n");
        printf("     1. SAT算例求解       2. 数独（待完成）\n");
        printf("     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0-2]: ");
        scanf("%d", &op);

        // 基本SAT任务求解
        if(op == 1)
        {
            strcpy(filename, ChooseExample());                        // 获得路径
            CnfParser(&literal_num, filename, 1);                     // 先获得文字数，最后的1表明是初始化
            int truth_table[literal_num];                             // 真值表
            for(i = 0; i < literal_num; i++)    truth_table[i] = 1;   // 初始化真值表，这里把初始值设为1，也可改为0
            t = t1 = t2 = 0;                                          // 初始化计时器
            // Print(s);                                              // 此句能够可视化数据
            printf("\n    计算中,将重复计算%d次取平均值...\n\n", EPOCH);

            // 重复计算EPOCH次 取平均值
            for(i = 0; i < EPOCH; i++)
            {
                t = clock();
                s = CnfParser(&literal_num, filename, 0);
                t1 += clock() - t;
                result = DpllSolver(s, truth_table);
                t2 += clock() - t;
            }

            // 输出结果
            printf("    加载耗时：%ldms\n", t1 / EPOCH);           // 取平均时间
            printf("    求解耗时：%ldms\n", t2 / EPOCH - t1 / EPOCH);
            printf("    总耗时  ：%ldms\n\n", t2 / EPOCH);
            if(result)
            {
                printf("    结果：子句集可满足，一组解如下:\n");   // 有解
                printf("    解为：");                              // 打印真值为1的变元
                for(int k = 0; k < literal_num; k++)    if(truth_table[k])  printf("%d ", k + 1);
                printf("\n");
            }
            else    printf("    结果：子句集不可满足！\n");        // 无解
            Get2Char();
        }

        else if(op == 0) break;

        else
        {
            printf("\n    请重新输入！\n");
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

char* ChooseExample()
{
    int op = 0;
    printf("\n     1.性能测试 sud00009  2.性能测试 ais10\n");
    printf("     3.功能测试 sat20     4.功能测试 unsat-5\n");
    printf("     0.自行输入\n");
    printf("    请输入需要计算的算例[0-4]：");
    while(1)
    {
        scanf("%d", &op);
        if(op == 1) return  "./SAT测试备选算例/基准算例/性能测试/sud00009.cnf";
        else if(op == 2)    return "./SAT测试备选算例/基准算例/性能测试/ais10.cnf";
        else if(op == 3)    return "./SAT测试备选算例/基准算例/功能测试/sat-20.cnf";
        else if(op == 4)    return "./SAT测试备选算例/基准算例/功能测试/unsat-5cnf-30.cnf";
        else if(op == 0)
        {
            printf("    请输入：");
            static char filename[PATH_SIZE];
            scanf("%s", filename);
            return filename;
        }
        printf("    请重新输入:");
    }
}
