//0905105

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>
#include<cmath>
#include<stack>
#include<list>
#include<vector>

using namespace std;


struct dtree{
    string idx;
    double gain;
    struct dtree *prev;
    struct dtree *child[10];
};

struct example
{
    int vals[10];
    example(int arr[])
    {
        for(int i=0;i<10;i++) vals[i]=arr[i];
    }
};

struct node
{
    int type,attr,val;
    int child[10];
};

class decision_tree
{
    int node_cnt;
    vector<node> tree;
public:
    decision_tree()
    {
        node_cnt=0;
    }
    void make_tree(vector<example> &examples)
    {
        ID3(examples,0);
    }
    int ID3(vector<example> &examples,int attr_mask)
    {
        int pos=0,neg=0;
        for(int i=0;i<examples.size();i++)
        {
            if(examples[i].vals[9]==1) pos++;
            else neg++;
        }
        int node_no=node_cnt;
        node_cnt++;
        node nw_node;
        if(pos==examples.size())
        {
            nw_node.type=0;
            nw_node.val=1;
            tree.push_back(nw_node);
            return node_no;
        }
        else if(neg==examples.size())
        {
            nw_node.type=0;
            nw_node.val=0;
            tree.push_back(nw_node);
            return node_no;
        }
        if(attr_mask==(1<<8)-1)
        {
            nw_node.type=0;
            nw_node.val=pos>neg?1:0;
            tree.push_back(nw_node);
            return node_no;
        }
        int attr=get_best_attr(examples,attr_mask);
        nw_node.type=1;
        nw_node.attr=attr;
        tree.push_back(nw_node);
        vector<example> tmp[10];
        for(int i=0;i<examples.size();i++)
        {
            tmp[examples[i].vals[attr]].push_back(examples[i]);
        }
        for(int i=0;i<=9;i++)
        {
            if(tmp[i].empty())
            {
                node nd;
                nd.type=0;
                nd.val=pos>neg?1:0;
                tree.push_back(nd);
                tree[node_no].child[i]=node_cnt;
                node_cnt++;
            }
            else
            {
                int n=ID3(tmp[i],attr_mask | (1<<attr));
                tree[node_no].child[i]=n;
            }
        }
    }
    int get_best_attr(vector<example> &examples,int attr_mask)
    {
        double mn=5.00; //INF
        int attr;
        for(int i=0;i<9;i++)
        {
            if(attr_mask & (1<<i)==0)
            {
                double tmp=expected_entropy(examples,i);
                if(tmp<mn) mn=tmp,attr=i;
            }
        }
        return attr;
    }
    double expected_entropy(vector<example> &examples,int attr)
    {
        int cnt[2][10]={0};
        for(int i=0;i<examples.size();i++)
        {
            cnt[examples[i].vals[9]][examples[i].vals[attr]]++;
        }
        int total=examples.size();
        double ret=0;
        for(int i=0;i<9;i++)
        {
            ret+=(cnt[0][i]+cnt[1][i])/total*entropy(cnt[0][i],cnt[1][i]);
        }
        return ret;
    }
    double entropy(int a,int b)
    {
        double x=(double) a/b;
        return -x*log(x)-(1-x)*log(1-x);
    }
};

int main(){

    int training[600][10],testing[200][10];
    std::fstream mfile("data.txt", std::ios_base::in);
    int a,b;
    int i,j,k,tr=0,ts=0;
    double t,temp;
    double cnt=0, cntall[9][11]={0}, cntttl[9][11]={0};
    double totalEntropy, entrpall[9][11]={0};
    double gainall[9];
    dtree mainTree;
    mainTree.idx = "-1";
    mainTree.prev = 0;
    std::stack<int> visited;
    std::list<int> others;

    for(i=0;i<683;i++){
        if((i%7)!=0){
            for(j=0;j<10;j++)
            {
                mfile >> a;
                training[tr][j] = a;
            }
            tr++;
        }
        else if((i%7)==0){
            for(j=0;j<10;j++)
            {
                mfile >> a;
                testing[ts][j] = a;
            }
            ts++;
        }
    }

    for(i=0;i<tr;i++){
        if(training[i][9]==1){
            cnt = cnt + 1;
        }
    }

    for(i=0;i<tr;i++){
        for(j=0;j<9;j++){
            a = training[i][j];
            if(training[i][9]==1){
                cntall[j][a]++;
            }
            cntttl[j][a]++;
        }
    }


    totalEntropy = - (cnt/tr)*log2(cnt/tr) - ((tr-cnt)/tr)*log2((tr-cnt)/tr);
    //cout<<"Total entropy: "<<totalEntropy<<endl;

    for(j=0;j<9;j++){
        //entrpall[j] = - (cntall[j]/tr)*log2(cntall[j]/tr) - ((tr-cntall[j])/tr)*log2((tr-cntall[j])/tr);
        for(k=1;k<=10;k++){
            if(cntall[j][k]>0 && cntall[j][k]<cntttl[j][k]){
                //entrpall[j] = entrpall[j] - (cntall[j][k]/tr)*log2(cntall[j][k]/tr);
                temp = cntall[j][k]/cntttl[j][k];
                entrpall[j][k] = - temp*log2(temp) - (1-temp)*log2(1-temp);
                //cout<<k<<" "<<cntall[j][k]<<" "<<cntttl[j][k]<<" "<<entrpall[j][k]<<endl;
            }
            //cout<<cntall[j][k]<<" ";
        }
    }

    for(j=0;j<9;j++){
        t=0;
        for(k=1;k<=10;k++){
            t = t + (cntttl[j][k]/tr)*entrpall[j][k];
        }
        gainall[j] = totalEntropy - t;
        //cout<<j<<" -> "<<gainall[j]<<endl;
    }

    //mainTree = ID3(mainTree, gainall, entrpall);

    double maxg=gainall[0];
    int big=0;
    for(i=0;i<9;i++){
        if(gainall[i]>=maxg){
            maxg = gainall[i];
            big = i;
        }
    }

    for(i=0;i<9;i++){
        if(i==big){
            visited.push(big);
            for(j=1;j<=10;j++){
                dtree ss;
                ss.idx = big + j;
                ss.prev = &mainTree;
                mainTree.child[j-1] = &ss;
            }
        }
        else
            others.push_back(i);
    }


    //cout<<"Root Node: "<<big;


    vector<example> examples;
    ifstream fin("data.csv");
    string str;

    while(fin>>str)
    {
        for(int i=0;i<str.size();i++) if(str[i]==',') str[i]=' ';
        stringstream ss;
        ss<<str;
        int arr[10];
        for(int j=0;j<10;j++) ss>>arr[j];
        example ex(arr);
        examples.push_back(ex);
        //for(int j=0;j<10;j++) cout<<arr[j]<<" ";
        //cout<<endl;
    }


    decision_tree *mtree = new decision_tree();
    mtree->make_tree(examples);


    return 0;
}
