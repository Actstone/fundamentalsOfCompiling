#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
#include <map>
#include <stack>
#include "typeinfo"
//#include "test.h"
#include "recursiveSubroutine.h"
#define maxGrammar 6 //假设文法不超过 maxGrammar-1 行。为什么减1 ？是因为需要比多出来一行，来判断为空。
#define VtNum 8


class Content{
public:
    string contentOfGrammar="";
    int lineOfGrammar=0;//记录文法真实的行数。
    int currentRow = 0;//在判断文法是否为左递归时，记录当前判断的所在行。
    char firstOfGrammar[maxGrammar];//记录文法每一行的最左部。
    char VtSet[VtNum]={'i','+','-','*','/','(',')','#'};//非终结符集合。
    int whereError[maxGrammar];
    bool yesOrNo=true;//当为 LL（1）时，yesOrNo 为 true。
    int selectOfLL1[maxGrammar][VtNum]={0};//储存 LL（1）分析表
    int indexOfGrammar = 0;
    
}cont;

void stackE();
void stackA();
void stackT();
void stackB();
void stackF();

void stackE(){
    stackT();
    stackA();
}
void stackA(){
    if(tM.operatorMap[cont.indexOfGrammar]=="-"){
        cout<<cont.indexOfGrammar<<"，识别为减号 - "<<endl;
        cont.indexOfGrammar+=1;
        
        stackT();
        stackA();
        
    }
    if(tM.operatorMap[cont.indexOfGrammar]=="+"){
        cout<<cont.indexOfGrammar<<"，识别为减号 + "<<endl;
        cont.indexOfGrammar+=1;
        
        stackT();
        stackA();
        
    }
}
void stackT(){
    stackF();
    stackB();
}
void stackB(){
    if(tM.operatorMap[cont.indexOfGrammar]=="*"){
        cout<<cont.indexOfGrammar<<"，识别为乘号 * "<<endl;
        cont.indexOfGrammar+=1;
        
        stackF();
        stackB();
    }
    if(tM.operatorMap[cont.indexOfGrammar]=="/"){
        cout<<cont.indexOfGrammar<<"，识别为除号 / "<<endl;
        cont.indexOfGrammar+=1;
        
        stackF();
        stackB();
    }
}
void stackF(){
    if(tM.operatorMap[cont.indexOfGrammar]=="("){
        cout<<cont.indexOfGrammar<<"，识别为左括号 （ "<<endl;
        cont.indexOfGrammar+=1;
        
        stackE();
        if(tM.operatorMap[cont.indexOfGrammar]!=")"){
            cout<<cont.indexOfGrammar<<"，出现错误。判断缺少右括号 "<<endl;
            exit(0);
        }else{
            cout<<cont.indexOfGrammar<<"，识别为右括号 ）"<<endl;
            cont.indexOfGrammar+=1;
        }
    }
    else if ((typeid(tM.countMap[cont.indexOfGrammar])==typeid(double)
              &&tM.countMap[cont.indexOfGrammar]!=0))
    {
        cout<<cont.indexOfGrammar<<"，识别为常数："<<tM.countMap[cont.indexOfGrammar]<<endl;
        cont.indexOfGrammar+=1;
    }
    else if (typeid(tM.identifiterMap[cont.indexOfGrammar])==typeid(string)
             &&tM.identifiterMap[cont.indexOfGrammar].length()>0) {
        cout<<cont.indexOfGrammar<<"，识别标识符："<<tM.identifiterMap[cont.indexOfGrammar]<<endl;
        cont.indexOfGrammar+=1;
    }
    else{
        cout<<"出现错误。判断缺少标识符或者常数"<<endl;
        exit(0);
    }
}
int LL1(){
    
    ifstream fgrammar;fgrammar.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/grammar.txt");
    if(!fgrammar.is_open())exit(0);
    string strGrammar[maxGrammar];//记录文法每一行的内容
    for (int i=0; i<maxGrammar; i++) {
        getline(fgrammar,strGrammar[i]);
        if(strGrammar[i].empty()){
            cont.lineOfGrammar = i;
            break;
        }
        cont.contentOfGrammar = cont.contentOfGrammar + strGrammar[i] + '\n';
    }
    fgrammar.close();
    for(int i=0;i<cont.contentOfGrammar.length();i++){
        cout<<cont.contentOfGrammar[i];
    }
    //记录文法每一行的最左部。
    int lengthOfLine = 0;
    for(int i =0;i<cont.lineOfGrammar;i++){
        if(i==0){
            cont.firstOfGrammar[i]=cont.contentOfGrammar[i];
            //cout<<cont.firstOfGrammar[i]<<endl;
        }else{
            lengthOfLine = lengthOfLine + (int)strGrammar[i-1].length();//加1，是因为在cont.contentOfGrammar 中，每一行的末尾是换行。
            cont.firstOfGrammar[i]=cont.contentOfGrammar[lengthOfLine+i];
            //cout<<cont.firstOfGrammar[i]<<endl;
        }
    }
    //判断文法是否左递归。
    cont.currentRow = 0;
    int lineMiddle = 0;
    for(int i=0;i<cont.contentOfGrammar.length();i++){
        if(cont.contentOfGrammar[i]=='\n')cont.currentRow += 1;
        if(cont.contentOfGrammar[i]=='>'||
           cont.contentOfGrammar[i]=='|')
        {
            lineMiddle +=1;
            if(cont.contentOfGrammar[i+1]==cont.firstOfGrammar[cont.currentRow]){
                cont.whereError[cont.currentRow] = lineMiddle;
            }
        }
    }
    for(int i =0;i<cont.lineOfGrammar;i++){
        if(cont.whereError[i]!=0){
            cont.yesOrNo = false;
            break;
        }
    }
    
    if(cont.yesOrNo){//如果该文法LL（1）文法，则求 select 集合。
        cont.currentRow=0;
        lineMiddle = 0;
        for(int i=0;i<cont.contentOfGrammar.length();i++){
            if(cont.contentOfGrammar[i]=='\n')
            {
                cont.currentRow+=1;
            }
            if(cont.contentOfGrammar[i]=='>'||
               cont.contentOfGrammar[i]=='|')
            {
                lineMiddle += 1;
                if(cont.contentOfGrammar[i+1]=='i'||
                   cont.contentOfGrammar[i+1]=='+'||
                   cont.contentOfGrammar[i+1]=='-'||
                   cont.contentOfGrammar[i+1]=='*'||
                   cont.contentOfGrammar[i+1]=='/'||
                   cont.contentOfGrammar[i+1]=='('||
                   cont.contentOfGrammar[i+1]==')')
                {
                    for (int j=0; j<VtNum-1; j++) { //VtNum 减1，因为不用判断 #
                        if((char)cont.contentOfGrammar[i+1]==cont.VtSet[j]){
                            cont.selectOfLL1[cont.currentRow][j]=lineMiddle;
                            break;
                        }
                    }
                }
            }
            for(int j=0;j<maxGrammar;j++){
                switch (cont.firstOfGrammar[j]) {
                    case 'E':
                        cont.selectOfLL1[0][0]=1;
                        cont.selectOfLL1[0][5]=1;
                        break;
                    case 'A':
                        cont.selectOfLL1[1][6]=4;
                        cont.selectOfLL1[1][7]=4;
                        break;
                    case 'T':
                        cont.selectOfLL1[2][0]=5;
                        cont.selectOfLL1[2][5]=5;
                        break;
                    case 'B':
                        cont.selectOfLL1[3][1]=8;
                        cont.selectOfLL1[3][2]=8;
                        cont.selectOfLL1[3][6]=8;
                        cont.selectOfLL1[3][7]=8;
                        break;
                    default:
                        break;
                }
            }
        }
    }else{
        cout<<"该文法不是 LL（1）文法"<<endl;exit(0);
    }
    
    
    
    //将 LL（1）分析表打印到屏幕上。
    {
        cout<<"*****LL(1)分析表*****"<<endl<<" ";
        for(int i=0;i<VtNum;i++){
            cout<<"   "<<cont.VtSet[i];
        }cout<<endl;
        for(int i=0;i<maxGrammar-1;i++){
            cout<<cont.firstOfGrammar[i]<<"   ";
            for(int j=0;j<VtNum;j++){
                cout<<cont.selectOfLL1[i][j]<<"   ";
            }cout<<endl;
        }
        cout<<endl;
    }
    test();
    cout<<"待判断的表达式："<<endl;
    ifstream fll1source;fll1source.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Csource.txt");
    if(!fll1source.is_open())exit(0);
    string cSource="";getline(fll1source,cSource);cout<<cSource<<endl<<"*******************"<<endl;;
    cont.indexOfGrammar=0;

    stackE();
    if(tM.operatorMap[cont.indexOfGrammar]=="#"){
        cout<<"This is a RIGHT expression ! "<<endl;
    }else{
        cout<<"This ia a WRONG expression ! "<<endl;
    }
    return 0;
}



/*
 E->TA
 A->+TA|-TA|?
 T->FB
 B->*FB|/FB|?
 F->i|(E)
 */

