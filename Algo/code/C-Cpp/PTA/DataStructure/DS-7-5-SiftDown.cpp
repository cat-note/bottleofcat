#include <iostream>
#include <vector>

using namespace std;

vector<int> H; // 初始化存放堆的数组

void Swap(int& src,int& dest); // 交换结点值
void InitializeHeap(); // 自底向上建堆
void SiftDown(int i); // 下滤下标为 i 的结点
void PrintPath(int start); // 打印从start下标到根节点的路径

int main(){
    int insertNum,pathNum;
    cin>>insertNum>>pathNum; // 输入元素个数和路径个数
    H.resize(insertNum+1); // 初始化数组结点数
    for(int i=0;i<insertNum;i++){
        int insertVal;
        cin>>insertVal;
        // 为了方便，从下标 1 开始存储
        H[i+1]=insertVal;
    }
    // 初始化堆
    InitializeHeap();
    // 输出堆
    for(int i=1;i<H.size();i++){
        cout<<H[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<pathNum;i++){
        int startInd;
        cin>>startInd;
        PrintPath(startInd);
        if(i!=pathNum-1)
            cout<<"\n";
    }
    return 0;
}

void InitializeHeap(){
    // 初始化堆其实就是从最后一个有孩子的结点开始向前
    for(int i=((H.size()-1)>>1);i>=1;i--){
        SiftDown(i);
    }
}

void SiftDown(int i){
    // 下滤堆中下标为 i 的结点
    // 因为是小根堆，应该和孩子中更小的那个比较
    // 注意，结点数n = H.size()-1，n/2 的地方为最后一个有孩子的结点的位置
    while(i<=((H.size()-1)>>1)){
        int leftInd=(i<<1),rightInd=leftInd+1;
        if(rightInd<H.size()&&H[rightInd]<H[leftInd]){
            // 如果结点还有右边孩子，比较两个孩子的大小，如果右孩子更小则准备和右孩子比
            // 也就是让 H[i] 和较小的一个孩子比
            leftInd=rightInd;
        }
        if(H[i]<=H[leftInd]){
            // 如果 i 结点比孩子都小，那就不用继续了
            break;
        }
        // 否则交换
        Swap(H[i],H[leftInd]);
        i=leftInd;
    }
}

// 交换结点值
void Swap(int& src,int& dest){
    int temp=src;
    src=dest;
    dest=temp;
}

// 注意末尾不能有多余空格
void PrintPath(int start){
    int i=start;
    for(;i>1;i>>=1){
        cout<<H[i]<<" ";
    }
    cout<<H[i];
}
