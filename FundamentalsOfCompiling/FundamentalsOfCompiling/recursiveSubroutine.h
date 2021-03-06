#include "cifaFenxi.h"
using namespace std;
class RecursiveSubroutine{
public:
    int recursiveFlag = 0;
}rS;

//E -> T {ω0 T}
//T -> F {ω1 F}
//F -> i | (E)
//w0(+,-);w1(*,/)


//递归子程序的子程序的函数实现
void E();
void T();
void F();

void E(){
    T();
    while(tM.operatorMap[rS.recursiveFlag] == "+"||
          tM.operatorMap[rS.recursiveFlag] == "-"){
        if (tM.operatorMap[rS.recursiveFlag]=="+") {
            cout<<rS.recursiveFlag<<"，判断为    +"<<endl;
        }
        if(tM.operatorMap[rS.recursiveFlag]=="-"){
            cout<<rS.recursiveFlag<<"，判断为    -"<<endl;
        }
        rS.recursiveFlag+=1;
        
        T();
    }
}
void T(){
    
    F();
    while(tM.operatorMap[rS.recursiveFlag] == "*"||
          tM.operatorMap[rS.recursiveFlag] == "/"){
        if (tM.operatorMap[rS.recursiveFlag]=="*") {
            cout<<rS.recursiveFlag<<"，判断为    *"<<endl;
        }
        
        if (tM.operatorMap[rS.recursiveFlag]=="/") {
            cout<<rS.recursiveFlag<<"，判断为    /"<<endl;
        }
        rS.recursiveFlag+=1;
        
        F();
    }
}
void F(){
    
    if(tM.operatorMap[rS.recursiveFlag] == "("){
        cout<<rS.recursiveFlag<<"，判断为    （ "<<endl;
        rS.recursiveFlag+=1;
        
        E();
        if(tM.operatorMap[rS.recursiveFlag] != ")"){
            cout<<"   出现错误。判断缺少右括号 "<<endl;
            
        }else{
            cout<<rS.recursiveFlag<<"，判断为    ）"<<endl;
            rS.recursiveFlag+=1;
        }
    }
    else if (typeid(string)==typeid(tM.identifiterMap[rS.recursiveFlag])
             &&tM.identifiterMap[rS.recursiveFlag]!="")
    {
        cout<<rS.recursiveFlag<<"，判断为标识符     "<<tM.identifiterMap[rS.recursiveFlag]<<endl;
        rS.recursiveFlag +=1;
        
    }
    else if(typeid(double)==typeid(tM.countMap[rS.recursiveFlag])
            &&tM.countMap[rS.recursiveFlag]!=0)
    {
        cout<<rS.recursiveFlag<<"，判断为常数      "<<tM.countMap[rS.recursiveFlag]<<endl;
        rS.recursiveFlag +=1;
    }
    else{
        cout<<rS.recursiveFlag<<"，出现错误。判断缺少标识符 或者 常数！"<<endl;
        exit(0);
        
    }
}
void RS(){
    
    cifaFenxi();
    cout<<"待判断的表达式："<<endl;
    ifstream fsource;fsource.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Csource.txt");
    if(!fsource.is_open())exit(0);
    string cSource="";getline(fsource,cSource);cout<<cSource<<endl<<"*******************"<<endl;;
    rS.recursiveFlag = 0;
    E();
    
    if(tM.operatorMap[rS.recursiveFlag]=="#"){
        cout<<endl<<"This is the RIGHT expression !"<<endl;
    }else{
        cout<<endl<<"This is the WRONG expression !"<<endl;
    }
    cout<<endl;
    
}
