//0905105

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>
#include<cmath>
#include<stack>
#include<list>

using namespace std;

struct dtree{
    int idx;
    double gain;
    struct dtree *prev;
    struct dtree *child[10];
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
    mainTree.idx = -1;
    mainTree.prev = 0;
    std::stack<int> visited;
    std::list<int> others;

    for(i=0;i<683;i++){
        if((i%5)!=0){
            for(j=0;j<10;j++)
            {
                mfile >> a;
                training[tr][j] = a;
            }
            tr++;
        }
        else if((i%5)==0){
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
        cout<<j<<" -> "<<gainall[j]<<endl;
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
    cout<<big<<endl;
    for(i=0;i<9;i++){
        if(i==big){
            visited.push(big);
            for(j=1;j<=10;j++){
                dtree ss;
                ss.idx = big + j;
                //cout<<ss.idx<<" ";
                ss.prev = &mainTree;
                mainTree.child[j-1] = &ss;
            }
        }
        else
            others.push_back(i);
    }

    int midarray[11][500][10]={0};
    for(k=1;k<=10;k++){
        int m=0;
        for(i=0;i<tr;i++){
            if(training[i][big]==k){
                for(j=0;j<10;j++){
                    midarray[k][m][j]=training[i][j];
                }
            }
            m++;
        }
    }
    int counteve[11]={0},countevepos[11]={0},countpos[11][9][11]={0},counttotal[11][9][11]={0};
    for(k=1;k<=10;k++){
        for(i=0;i<tr;i++){
            for(j=0;j<9;j++){
                if(midarray[k][i][big]==k){
                    a = midarray[k][i][j];
                    if(midarray[k][i][9]==1){
                        countpos[k][j][a]++;
                        countevepos[k]++;
                    }
                    counttotal[k][j][a]++;

                    counteve[k]++;
                }
            }
        }
    }

    double tEntropy[11]={0},bEntropy[11][9][11]={0};
    for(i=1;i<=10;i++){
        tEntropy[i] = - (countevepos[i]/counteve[i])*log2(countevepos[i]/counteve[i]) - (1 - countevepos[i]/counteve[i])*log2(1 - countevepos[i]/counteve[i]);
        for(j=0;j<9;j++){
            for(k=1;k<=10;k++){
                if(counttotal[i][j][k]>0){
                    temp = countpos[i][j][k]/counttotal[i][j][k];
                    bEntropy[i][j][k] = - temp*log2(temp) - (1-temp)*log2(1-temp);
                }
            }
        }
    }
    double gaineve[11][9]={0};
    for(i=1;i<=10;i++){
        for(j=0;j<9;j++){
            t=0;
            for(k=1;k<=10;k++){
                t = t + (counttotal[i][j][k]/counteve[i])*bEntropy[i][j][k];
            }
            gaineve[i][j] = tEntropy[i] - t;
            cout<<i<<j<<" -> "<<gaineve[i][j]<<endl;
        }
    }


    //cout<<"Root Node: "<<big;


    return 0;
}
