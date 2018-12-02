#include <stack>
#include "typeinfo"
#include "cifaFenxi.h"

using namespace std;
#define sYsOfNum 10//假设四元式的数目不超过 sYsOfNum

class LL1{
public:
    stack<string> SYN;//语法栈
    stack<string> SEM;//语义栈
    stack<int> SEM_num;
    string QT[sYsOfNum]={"t0","t1","t2","t3","t4","t5","t6","t7","t8","t9"};//四元式区
    int indicator = 0;//相当于 QT 的索引。
    int analysisTable[5][8]={
        1,0,0,0,0,1,0,0,
        0,2,3,0,0,0,4,4,
        5,0,0,0,0,5,0,0,
        0,8,8,6,7,0,8,8,
        9,0,0,0,0,10,0,0
    };//文法分析表
    string VT="i+-*/()#";
    string VN="EATBF";
    string action="?@$`_";
    //？= GEQ（+），@ = GEQ（-），$ = GEQ（*），` = GEQ（/）_=PUSH(i)。
    //后续在使用这些符号时，应该注意。
    
}ll1;

void pressStack(int i){//查询LL（1）分析表，根据不同的值进行压栈。
    if(i==1){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：A T "<<endl;
        ll1.SYN.push("A");
        ll1.SYN.push("T");
    }
    if(i==2){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：A T + "<<endl;
        ll1.SYN.push("A");
        ll1.SYN.push("?");
        ll1.SYN.push("T");
        ll1.SYN.push("+");
    }
    if(i==3){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：A T - "<<endl;
        ll1.SYN.push("A");
        ll1.SYN.push("@");
        ll1.SYN.push("T");
        ll1.SYN.push("-");
    }
    if(i==4){
        //cout<<"查分析表，L（x，w）= "<<i<<"，无操作 "<<endl;
    }
    if(i==5){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("F");
    }
    if(i==6){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F * "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("$");
        ll1.SYN.push("F");
        ll1.SYN.push("*");
    }
    if(i==7){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F / "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("`");
        ll1.SYN.push("F");
        ll1.SYN.push("/");
    }
    if(i==8){
        cout<<"查分析表，L（x，w）= "<<i<<"，无操作 "<<endl;
    }
    if(i==9){
        cout<<"查分析表，L（x，w）= "<<i<<"，将 i 压栈 "<<endl;
        ll1.SYN.push("_");
        //ll1.SYN.push("i");
        
        ll1.SYN.push(tM.strSource[ll1.SEM_num.top()]);//将表达式中的标识符压栈
        
        
        
    }
    if(i==10){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：） E （ "<<endl;
        ll1.SYN.push(")");
        ll1.SYN.push("E");
        ll1.SYN.push("(");
    }
}

void act(string str,int i){
    if(str=="@"){//-
        string a=ll1.SEM.top();ll1.SEM.pop();
        
        string b=ll1.SEM.top();ll1.SEM.pop();
        ll1.SEM.push(ll1.QT[i]);
        ll1.QT[i]="(-  "   + b + "  " + a + "  " + ll1.QT[i]+")";
        ll1.indicator+=1;
    }
    if(str=="?"){//+
        string a=ll1.SEM.top();ll1.SEM.pop();
        string b=ll1.SEM.top();ll1.SEM.pop();
        ll1.SEM.push(ll1.QT[i]);
        ll1.QT[i]="(+  "   + b + "  " + a + "  " + ll1.QT[i]+")";
        ll1.indicator+=1;
    }
    if(str=="$"){//*
        string a=ll1.SEM.top();ll1.SEM.pop();
        string b=ll1.SEM.top();ll1.SEM.pop();
        ll1.SEM.push(ll1.QT[i]);
        ll1.QT[i]="(*  "   + b + "  " + a + "  " + ll1.QT[i]+")";
        ll1.indicator+=1;
    }
    if(str=="`"){// /
        string a=ll1.SEM.top();ll1.SEM.pop();
        string b=ll1.SEM.top();ll1.SEM.pop();
        ll1.SEM.push(ll1.QT[i]);
        ll1.QT[i]="(/  "   + b + "  " + a + "  " + ll1.QT[i]+")";
        ll1.indicator+=1;
        
    }
    if(str=="_"){// push(i)
        //ll1.SEM.push("i");
        ll1.SEM.push(tM.strSource[ll1.SEM_num.top()]);
        ll1.SEM_num.pop();
        
    }
}



int LL1(){
    cifaFenxi();
    ifstream fll1;fll1.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Csource.txt");
    if(!fll1.is_open())exit(0);
    string ll1Csource="";
    getline(fll1,ll1Csource);
    fll1.close();
    cout<<"*****************表达式****************"<<endl;
    cout<<ll1Csource<<endl;
    cout<<"**************************************"<<endl;
    cout<<"PUSH(    #   )"<<endl;
    cout<<"PUSH(    E   )"<<endl;
    ll1.SYN.push("#");ll1.SYN.push("E");
    // begin
    for(int i=0;i<tM.mapFlag;i++){
        string w = tM.strSource[i];
        // 将读入的常数和标识符否转化为文法中的 i
        if(w == tM.countStrMap[i]||
           w == tM.identifiterMap[i]){
            w="i";
            ll1.SEM_num.push(i);
        }
        
    R1:string x = ll1.SYN.top();
        
        if(ll1.VN.find(x)==string::npos
           &&ll1.VT.find(x)==string::npos
           &&ll1.action.find(x)==string::npos){
            
            x="i";//判断 x 在不在 VN VT action中，如果不在，则默认为是表达式中的标识符。
            
        }
        ll1.SYN.pop();
        cout<<"POP(     "<<x<<"     )"<<endl;
        
        
        if(ll1.action.find(x)>=0
           && ll1.action.find(x)<ll1.action.length()){
            act(x,ll1.indicator);
            goto R1;
        }
        
        //x is in VT ?
        if(ll1.VT.find(x)>=0
           &&ll1.VT.find(x)<ll1.VT.length()
           &&x!="#"){
            
            if(x==w){//x == w
                continue;
            }else{
                cout<<"ERROR1"<<endl;
                exit(1);
            }
        }
        // x is in VN ?
        else if(ll1.VN.find(x)>=0
                &&ll1.VN.find(x)<ll1.VN.length()){
            int resultOfanalysisTable;
            if(ll1.VN.find(x)>=0
               &&ll1.VN.find(x)<ll1.VN.length()
               &&ll1.VT.find(w)>=0&&ll1.VT.find(w)<ll1.VT.length()){
                
                resultOfanalysisTable =ll1.analysisTable[ll1.VN.find(x)][ll1.VT.find(w)];
                
                pressStack(resultOfanalysisTable);
                goto R1;
                
            }else{
                cout<<"ERROR3"<<endl;
                exit(3);
            }
        }else if(w=="#"){
            cout<<"OK"<<endl;
            cout<<endl<<"This is a RIGHT expression!"<<endl<<endl;
            
        }else{
            cout<<"ERROR2"<<endl;
            exit(2);
        }
    }
    
    for(int i=0;i<ll1.indicator;i++){//输出四元式
        if(ll1.QT[i].length()>2){
            cout<<ll1.QT[i]<<endl;
        }
    }
    
    return 0;
}

