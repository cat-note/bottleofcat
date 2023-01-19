#include <iostream>
#include <stack>

using namespace std;

typedef struct TreeNode *Tree;

struct TreeNode
{
    Tree left;  // 左子树
    Tree right; // 右子树
    int height; // 节点所在高度，平衡因子靠这个算
    int val;    // 节点值
};

class AVLTree
{
private:
    Tree tree = NULL; // 空树
public:
    // 更新节点高度
    void updateHeight(Tree node)
    {
        int leftHeight = node->left ? node->left->height : 0;
        int rightHeight = node->right ? node->right->height : 0;
        // 选择较高的那颗子树的高度+1以更新
        if (leftHeight > rightHeight)
            node->height = leftHeight + 1;
        else
            node->height = rightHeight + 1;
    }
    // 获得某个节点的平衡因子
    int balanceFactor(Tree node)
    {
        // 左子树高度-右子树高度
        int leftHeight = node->left ? node->left->height : 0;
        int rightHeight = node->right ? node->right->height : 0;
        return leftHeight - rightHeight;
    }
    // 失衡节点右旋操作，node是失衡结点
    void rotateRight(Tree node)
    {
        Tree nodeLeft = node->left;         // 失衡节点左子树
        Tree nodeRight = node->right;       // 失衡节点右子树
        Tree lChildLeft = nodeLeft->left;   // 失衡节点的左孩子的左子树
        Tree lChildRight = nodeLeft->right; // 失衡节点左孩子的右子树
        // 这里【没有指向父节点】的指针，我们直接修改结点的值来模拟移动结点即可
        int nodeVal = node->val;   // 失衡节点的值
        node->val = nodeLeft->val; // 交换失衡节点和左孩子的值
        nodeLeft->val = nodeVal;
        // 这里已经不是左孩子了，而是“旋转”下来的失衡节点
        nodeLeft->left = lChildRight; // 修改结点的左右子树
        nodeLeft->right = node->right;
        node->left = lChildLeft; // 和子树的根结点接上
        node->right = nodeLeft;
        // 此时node是子树根结点，lChildLeft是左子树，nodeLeft是右子树
        updateHeight(nodeLeft); // 更新有变动的结点的高度，先更新子树再更新根
        updateHeight(node);
    }
    // 失衡节点左旋操作，node是失衡节点
    void rotateLeft(Tree node)
    {
        Tree nodeLeft = node->left;          // 失衡节点左子树
        Tree nodeRight = node->right;        // 失衡节点右子树
        Tree rChildLeft = nodeRight->left;   // 失衡节点的右孩子的左子树
        Tree rChildRight = nodeRight->right; // 失衡节点的右孩子的右子树
        // 这里【没有指向父节点】的指针，我们直接修改结点的值来模拟移动结点
        int nodeVal = node->val;
        node->val = nodeRight->val; // 交换失衡节点和右孩子的值
        nodeRight->val = nodeVal;
        // 这里的nodeRight就是“旋转”下来的节点
        nodeRight->right = rChildLeft;
        nodeRight->left = node->left;
        node->left = nodeRight;
        node->right = rChildRight;
        // 此时node是子树根结点，nodeRight是左子树，rChildRight是右子树
        updateHeight(nodeRight);
        updateHeight(node);
    }
    // 插入操作
    void insert(int value)
    {
        Tree node = new TreeNode();
        node->left = NULL;
        node->right = NULL;
        node->val = value;
        if (!tree) // 树还是空的，作为首个节点
        {
            tree = node;
            tree->height = 1; // 叶节点高度为1
            return;
        }
        // 接下来是树不为空的情况，寻找插入的位置
        Tree curr = tree;
        stack<Tree> access; // 储存插入节点经过的路径的栈，因为后面要回溯
        while (true)
        {
            access.push(curr);
            if (value < curr->val)
            {
                // 插入值比这个节点要小，往左子树插
                if (curr->left)
                {
                    curr = curr->left; // 左子树不为空，进入左子树
                }
                else
                {
                    curr->left = node; // 左子树为空，作为左子树根结点
                    break;
                }
            }
            else if (value > curr->val)
            {
                // 插入值比这个节点要大，往右子树插
                if (curr->right)
                {
                    curr = curr->right; // 右子树不为空，进入右子树
                }
                else
                {
                    curr->right = node; // 右子树为空，作为右子树根结点
                    break;
                }
            }
            else
            {
                // 找到了相同值的节点，不予插入
                delete node;
                return;
            }
        }
        // 插入完成后回溯，检查沿路节点的平衡因子
        while (!access.empty())
        {
            // 弹栈
            Tree curr = access.top();
            access.pop();
            // 更新节点所在高度
            updateHeight(curr);
            // 获得节点平衡因子
            int bf = balanceFactor(curr);
            // 如果插入后，回溯的时候发现有节点的平衡因子=0，就不用再回溯了，这说明插入的节点并不会影响平衡
            if (bf == 0)
                break;
            // 该节点失衡了，需要进行调整
            if (bf > 1)
            {
                // 失衡节点的平衡因子>1，说明左子树比较高，因此找失衡节点的左孩子
                // 看失衡节点左孩子的平衡因子
                int leftBf = balanceFactor(curr->left);
                if (leftBf > 0)
                {
                    // 这个左孩子的左子树高于右子树
                    // 这说明是LL型，即插入在失衡节点【左孩子的左子树中】而导致失衡，需要进行“右旋”进行调整
                    rotateRight(curr);
                }
                else
                {
                    // 这个左孩子的右子树高于左子树
                    // 这说明是LR型，插入在失衡结点【左孩子的右子树中】而导致失衡，需要进行“左旋再右旋”进行调整
                    rotateLeft(curr->left); // 先对左孩子进行左旋
                    rotateRight(curr);      // 再对失衡节点进行右旋
                }
            }
            else if (bf < -1)
            {
                // 失衡节点的平衡因子<-1，说明右子树比较高，因此找失衡节点的右孩子
                int rightBf = balanceFactor(curr->right);
                if (rightBf < 0)
                {
                    // 右孩子的右子树高于左子树
                    // 这说明是RR型，即插入在失衡节点【右孩子的右子树中】，需要进行“左旋”进行调整
                    rotateLeft(curr);
                }
                else
                {
                    // 右孩子的左子树高于右子树
                    // 这说明是RL型，即插入在失衡节点【右孩子的左子树中】，需要进行“右旋再左旋”进行调整
                    rotateRight(curr->right); // 先对右孩子进行右旋
                    rotateLeft(curr);         // 再对失衡节点进行左旋
                }
            }
        }
    }
    // 寻找节点
    Tree search(int value)
    {
        Tree curr = tree;
        while (curr)
        {
            if (value > curr->val)
                curr = curr->right; // 在当前节点的右子树中
            else if (value < curr->val)
                curr = curr->left; // 在当前节点的左子树中
            else
                break; // 找到了
        }
        return curr;
    }
};

int main()
{
    AVLTree tree;     // 初始化AVL树
    int N, searchNum; // 整数数量，搜索次数
    cin >> N >> searchNum;
    for (int i = 0; i < N; i++)
    {
        int num;
        cin >> num;
        tree.insert(num);
    }
    for (int i = 0; i < searchNum; i++)
    {
        int target;
        cin >> target;
        if (tree.search(target))
            cout << "1 ";
        else
            cout << "0 ";
    }
    cout << "\n";
    return 0;
}

/*
    例题:
        Dotcpp 题目 1713: 数据结构-平衡二叉树的基本操作
*/