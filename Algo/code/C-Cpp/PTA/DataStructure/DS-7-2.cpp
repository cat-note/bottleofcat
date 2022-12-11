/*
7-2 一元多项式的乘法与加法运算
    设计函数分别求两个一元多项式的乘积与和。

输入格式:
    输入分2行，每行分别先给出多项式非零项的个数，再以指数递降方式输入一个多项式非零项系数和指数（绝对值均为不超过1000的整数）。数字间以空格分隔。
输出格式:
    输出分2行，分别以指数递降方式输出乘积多项式以及和多项式非零项的系数和指数。数字间以空格分隔，但结尾不能有多余空格。零多项式应输出0 0。
输入样例:
    4 3 4 -5 2  6 1  -2 0
    3 5 20  -7 4  3 1
输出样例:
    15 24 -25 22 30 21 -10 20 -21 8 35 6 -33 5 14 4 -15 3 18 2 -6 1
    5 20 -4 4 -5 2 9 1 -2 0
代码长度限制
    16 KB
时间限制
    200 ms
内存限制
    64 MB
*/
#include <iostream>

using namespace std;

typedef struct termNode Term;

struct termNode
{
    int coefficient; // 系数
    int exponent;    // 指数
    Term *prev;      // 上一项
    Term *next;      // 下一项
};                   // 多项式的一项

// 声明函数
Term *CreateEmpty();                             // 创建空多项式(链表)
Term *ReadTerms();                               // 读入多项式
Term *MulTerms(Term *terms1, Term *terms2);      // 多项式相乘
Term *RemoveNode(Term *targetNode);              // 从链表中移除相应节点，返回这个节点的前一个节点
void InsertInOrder(Term *terms, Term *termNode); // 按序插入，顺便合并同类项
Term *AddTerms(Term *terms1, Term *terms2);      // 多项式相加
void PrintTerms(Term *terms);                    // 输出多项式

int main()
{
    // 先读入两个多项式
    Term *terms1 = ReadTerms();
    Term *terms2 = ReadTerms();
    PrintTerms(MulTerms(terms1, terms2));
    cout << "\n";
    PrintTerms(AddTerms(terms1, terms2));
    return 0;
}

Term *CreateEmpty()
{
    // 多项式头节点
    Term *head = new Term();
    head->coefficient = 0;
    head->exponent = 0;
    head->prev = NULL;
    head->next = NULL;
    return head;
}

Term *ReadTerms()
{
    // 多项式头节点
    Term *head = CreateEmpty();
    Term *tail = head; // 链表尾指针
    int termsLen;      // 多项式项数
    cin >> termsLen;
    for (int i = 0; i < termsLen; i++)
    {
        Term *termN = new Term();
        cin >> termN->coefficient; // 读入当前项系数
        cin >> termN->exponent;    // 读入当前项指数
        termN->next = NULL;
        termN->prev = tail;
        tail->next = termN; // 接入链表
        tail = termN;       // 改变尾指针
    }
    return head; // 返回代表多项式的链表
}

// 从链表中移除targetNode节点，返回前一个节点的指针
Term *RemoveNode(Term *targetNode)
{
    Term *prevNode = targetNode->prev;
    prevNode->next = targetNode->next;
    if (targetNode->next)
        targetNode->next->prev = prevNode;
    free(targetNode);
    return prevNode;
}

// 按指数递降插入到terms中
// 用于乘法，因为乘法乘出来的项指数并不是按序排的
void InsertInOrder(Term *terms, Term *termNode)
{
    Term *position = terms; // 插入在position指向的这个节点的后面
    for (Term *curr = terms->next; curr != NULL; curr = curr->next)
    {
        // 找到第一个比待插入项指数小的，插在这个节点前面
        if (curr->exponent < termNode->exponent)
        {
            position = curr->prev;
            break;
        }
        if (curr->next == NULL)
        { // 如果后面已经没有节点了(到了尾部)
            position = curr;
        }
    }
    // 正好有可以合并同类项的
    if (position->exponent == termNode->exponent)
    {
        // 指数相同，系数相加
        position->coefficient += termNode->coefficient;
        free(termNode); // 释放内存
        if (position->coefficient == 0)
            // 合并同类项后系数为0, 从链表中移除此项
            RemoveNode(position);
    }
    else // 否则正常插入到position后面
    {
        if (position->next)
            position->next->prev = termNode;
        termNode->next = position->next;
        termNode->prev = position;
        position->next = termNode;
    }
}

Term *MulTerms(Term *terms1, Term *terms2)
{

    Term *mulTerms = CreateEmpty(); // 结果多项式
    for (Term *curr1 = terms1->next; curr1 != NULL; curr1 = curr1->next)
    {
        for (Term *curr2 = terms2->next; curr2 != NULL; curr2 = curr2->next)
        {
            // 系数相乘
            int mulCof = curr1->coefficient * curr2->coefficient;
            // 指数相加
            int addExp = curr1->exponent + curr2->exponent;
            // 如果运算后系数不为0，按序插入到结果多项式中，顺带合并同类项
            if (mulCof != 0)
            {
                Term *node = new Term();
                node->coefficient = mulCof;
                node->exponent = addExp;
                InsertInOrder(mulTerms, node);
            }
        }
    }
    return mulTerms;
}

Term *AddTerms(Term *terms1, Term *terms2)
{
    Term *addTerms = CreateEmpty();
    Term *addTail = addTerms; // 尾指针
    Term *curr1 = terms1->next;
    Term *curr2 = terms2->next;
    // 加法需要找到两个多项式中的同指数项，用两个指针来找
    while (curr1 && curr2)
    {
        Term *node = new Term(); // 待加入结果的新项
        if (curr1->exponent == curr2->exponent)
        {
            // 这个节点后面不会再用到了，运算后可以直接加入addTerms
            // 指数相同，系数相加
            int addCof = curr1->coefficient + curr2->coefficient;
            if (addCof != 0) // 合并后系数不为0，才加入到结果中
            {
                if (curr1->exponent == addTail->exponent) // 💡 容易遗漏的情况，运算后加到结果中还要判断能不能再合并一次同类项
                {
                    // 如果还能再合并同类项
                    // 系数相加
                    addTail->coefficient += addCof;
                    if (addTail->coefficient == 0)
                        // 相加后等于0，需要将此项从链表中移除
                        addTail = RemoveNode(addTail);
                    free(node);
                }
                else
                {
                    node->coefficient = addCof;
                    node->exponent = curr1->exponent;
                    node->next = NULL;
                    addTail->next = node;
                    node->prev = addTail;
                    addTail = node;
                }
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        else if (curr1->exponent > curr2->exponent) // 当前多项式1中的某项比多项式2的某项指数要大，先在多项式1往后找
        {
            node->coefficient = curr1->coefficient;
            node->exponent = curr1->exponent;
            node->prev = addTail;
            node->next = NULL;
            addTail->next = node;
            addTail = node;
            curr1 = curr1->next;
        }
        else if (curr1->exponent < curr2->exponent) // 当前多项式2中的某项比多项式1的某项指数要大，先在多项式2往后找
        {
            node->coefficient = curr2->coefficient;
            node->exponent = curr2->exponent;
            node->prev = addTail;
            node->next = NULL;
            addTail->next = node;
            addTail = node;
            curr2 = curr2->next;
        }
    }
    // 如果还有剩余(没有同类项)的节点，直接接到链表后面即可
    // 注意这里没有动prev指针，因为输出时只需要next
    if (curr1)
        addTail->next = curr1;
    else if (curr2)
        addTail->next = curr2;
    return addTerms;
}

void PrintTerms(Term *terms)
{
    bool hasPrinted = false; // 前面是否已经输出了项
    for (Term *curr = terms->next; curr != NULL; curr = curr->next)
    {
        if (hasPrinted) // 防止多余空格
            cout << " ";
        else
            hasPrinted = true;
        cout << curr->coefficient << " " << curr->exponent;
    }
    // 如果没有打印出东西，说明是零多项式
    if (!hasPrinted)
        cout << "0 0";
}

/*
    关于输入：
        比如第一行是 4 3 4 -5 2  6 1  -2 0
            - 第一个数字代表这个多项式【有4项】
            - 后面两个数一对: [3 4] [-5 2] [6 1] [-2 0]，代表的是3x^4-5x^2+6x-2
    这题我选择了采用链表实现。代码编写起来倒还挺顺，但排错过程可谓是非常痛苦...
    这里说几个着重要注意的点：
        输入的两行多项式中的项都是【按指数降序排列】的:
            - 这十分有利于多项式的加法，因为用两个指针分别顺着找两个多项式，很快就能找到【同类项】。

            - 但是这【并不利于多项式的乘法】，因为多项式相乘【指数需要相加】，而输出的时候【题目要求也按指数降序输出】，因此在创建代表乘法结果的多项式的时候需要注意排序。

            - 在将新项插入结果多项式(链表)(无论是加法结果还是乘法结果)时，要注意【合并同类项】

            - 合并同类项可能产生【系数为0】的项，这些项是不能在结果中输出的

        我的解决方案:

            对于乘法:
                - 乘法就是把一个多项式的每一项与另一个多项式的每一项相乘，得出结果多项式

                - 每运算出一项，将其【按指数降序】插入结果多项式(链表)，在插入过程中解决排序问题

                - 在上述插入过程中需要寻找位置，而在这个过程中还【可以进行合并同类项】
                    (因为是按序插入的，每次找到插入位置后和前一项进行比较，如果前一项指数和待插入项指数一致就可以合并)

                - 合并同类项后如果系数为0，就把此项从结果多项式(链表)中移除

            对于加法:
                - 多项式的加法其实就是合并两个多项式中的同类项，因此重点在于寻找两个多项式的同类项

                - 得益于本题已经将多项式【按指数降序排列】，用两个指针顺着分别找两个多项式中的项即可，过程演示如下：
                    p1
                    ↓
                    9 8 3    (这里仅列出每项的指数)
                    8 7 5 3
                    ↑
                    p2

                    1. p1和p2对比: p1项指数9 > p2项指数8
                        - 将p1指向的项加入结果
                        - p1指针后移，p2不动
                        - 此时的结果多项式(仅列出指数): 【9】
                    2. p1和p2对比: p1项指数8 = p2项指数8
                        - 合并p1和p2代表的项
                        - p1指针后移，p2指针后移
                        - 此时的结果多项式(仅列出指数): 【9 8】
                    3. p1和p2对比: p1项指数3 < p2项指数7
                        - 将p2代表的项加入到结果中，直至p2代表的项的指数>=p1代表的项的指数
                        - p1指针不动，p2指针后移
                        - 结果多项式(仅列出指数): 【9 8 7 5】
                    4. p1和p2对比: p1项指数3 = p2项指数3
                        - 合并p1和p2代表的项
                        - p1指针后移，p2指针后移
                        - 此时的结果多项式(仅列出指数): 【9 8 7 5 3】

                - 相加后系数为0的项就不必加入结果中了

                - 别忘了还要和结果多项式中的项进行合并同类项

                - 合并同类项后如果系数为0，就把此项从结果多项式(链表)中移除


    💡 这题有个地方我写复杂了: 完全可以在结果中保留【系数为0】的节点，只要在输出函数中忽略掉这些节点就行。

一个测试例, 用于测试加法的合并同类项是否过关：
4 3 40 -5 40  -3 1  -2 0
3 5 40 -3 40  3 1

    - SomeBottle 2022.12.11
*/