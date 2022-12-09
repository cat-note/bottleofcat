#include <iostream>

using namespace std;

typedef struct termNode Term;

struct termNode
{
    int coefficient; // 系数
    int exponent;    // 指数
    Term *next;      // 下一项
};                   // 多项式的一项

// 打印多项式
void printTerms(Term *terms)
{
    bool hasPrint = false; // 是否打印出了内容
    for (Term *curr = terms->next; curr != NULL; curr = curr->next)
    {
        if (curr->coefficient != 0)
        { // 系数为0的项就当不存在
            if (hasPrint)
                cout << " "; // 如果前面以及打印出了内容，这里要加空格
            else
                hasPrint = true;
            cout << curr->coefficient << " " << curr->exponent;
        }
    }
    if (!hasPrint)
        cout << "0 0"; // 0多项式
}

int main()
{
    int termNum1; // 第一个多项式的项数
    cin >> termNum1;
    Term *terms1 = new Term(); // 初始化第一个多项式头节点
    Term *tail1 = terms1;      // 第一个多项式的尾指针
    terms1->next = NULL;
    for (int i = 0; i < termNum1; i++)
    {
        Term *termN = new Term();
        cin >> termN->coefficient; // 读入系数
        cin >> termN->exponent;    // 读入指数
        tail1->next = termN;       // 加入多项式
        tail1 = termN;             // 改变尾指针
    }
    int termNum2; // 第二个多项式的项数
    cin >> termNum2;
    Term *terms2 = new Term(); // 初始化第二个多项式头节点
    Term *tail2 = terms2;      // 第二个多项式的尾指针
    terms2->next = NULL;
    Term *multiList = new Term(); // 乘法结果链表头指针
    multiList->next = NULL;
    Term *multiTail = multiList; // 乘法结果链表尾指针
    // 先算乘法--------将多项式2的每一项与多项式1相乘
    for (int i = 0; i < termNum2; i++)
    {
        int cof2, exp2;
        cin >> cof2; // (第二个多项式)某项的系数
        cin >> exp2; // (第二个多项式)某项的指数
        for (Term *curr1 = terms1->next; curr1 != NULL; curr1 = curr1->next)
        {
            // 系数相乘
            int cofM = cof2 * curr1->coefficient;
            // 指数相加
            int expM = exp2 + curr1->exponent;
            if (multiTail != multiList && multiTail->exponent == expM)
            {
                // 指数相同，合并同类项
                // 系数相加
                multiTail->coefficient += cofM;
            }
            else
            {
                // 否则作为新项目加入结果中
                Term *termN = new Term();
                termN->coefficient = cofM;
                termN->exponent = expM;
                multiTail->next = termN; // 加入乘法结果多项式
                multiTail = termN;       // 改变乘法结果链表尾指针指向
            }
        }
        Term *termN = new Term();
        termN->coefficient = cof2;
        termN->exponent = exp2;
        termN->next = NULL;
        tail2->next = termN;
        tail2 = termN; // 改变多项式2尾指针指向
    }
    // 再算加法--------两多项式同指数项相加, 其余拷贝
    Term *p1 = terms1->next; // 多项式1同时作为加法结果输出
    Term *p2 = terms2->next;
    Term *addList = new Term(); // 加法结果链表头指针
    addList->next = NULL;
    Term *addTail = addList; // 加法结果链表尾指针
    while (p1 != NULL && p2 != NULL)
    {
        Term *temp;
        if (p1->exponent == p2->exponent)
        {
            temp = p1->next;
            p1->coefficient += p2->coefficient; // 系数相加
            // 0系数的话就不加入结果了
            p1->next = NULL;
            addTail->next = p1; // 加入到结果链表中
            addTail = p1;
            p1 = temp;
            p2 = p2->next;
        }
        else if (p1->exponent > p2->exponent)
        {
            // 此时多项式1的某一项的指数大于多项式2某一项的指数
            temp = p1->next;
            p1->next = NULL;
            addTail->next = p1; // 加入到结果链表中
            addTail = p1;
            p1 = temp;
        }
        else if (p2->exponent > p1->exponent)
        {
            // 此时多项式2的某一项的指数大于多项式1某一项的指数
            temp = p2->next;
            p2->next = NULL;
            addTail->next = p2; // 加入到结果链表中
            addTail = p2;
            p2 = temp;
        }
    }
    if (p1)
    {                       // 如果多项式1还剩几项没有加入
        addTail->next = p1; // 把剩余的项目接入结果
    }
    else if (p2)
    {
        addTail->next = p2;
    }
    // 输出结果
    printTerms(multiList);
    cout << "\n";
    printTerms(addList);
    return 0;
}

// 这题的测试点1 我死活排查不出来...改日再战吧