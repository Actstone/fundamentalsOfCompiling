#include <stack>
#include "typeinfo"
#include "cifaFenxi.h"

using namespace std;

class LL1{
public:
    stack<string> SYN;//语法栈
    stack<string> SEM;//语义栈
    string QT;//四元式区
    int analysisTable[5][8]={
        1,0,0,0,0,1,0,0,
        0,2,3,0,0,0,4,4,
        5,0,0,0,0,5,0,0,
        0,8,8,6,7,0,8,8,
        9,0,0,0,0,10,0,0
    };//文法分析表
    string VT="i+-*/()#";
    string VN="EATBF";
    
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
        ll1.SYN.push("T");
        ll1.SYN.push("+");
    }
    if(i==3){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：A T - "<<endl;
        ll1.SYN.push("A");
        ll1.SYN.push("T");
        ll1.SYN.push("-");
    }
    if(i==4){
        cout<<"查分析表，L（x，w）= "<<i<<"，无操作 "<<endl;
    }
    if(i==5){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("F");
    }
    if(i==6){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F * "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("F");
        ll1.SYN.push("*");
    }
    if(i==7){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：B F / "<<endl;
        ll1.SYN.push("B");
        ll1.SYN.push("F");
        ll1.SYN.push("/");
    }
    if(i==8){
        cout<<"查分析表，L（x，w）= "<<i<<"，无操作 "<<endl;
    }
    if(i==9){
        cout<<"查分析表，L（x，w）= "<<i<<"，将 i 压栈 "<<endl;
        ll1.SYN.push("i");
    }
    if(i==10){
        cout<<"查分析表，L（x，w）= "<<i<<"，故逆序压栈：） E （ "<<endl;
        ll1.SYN.push(")");
        ll1.SYN.push("E");
        ll1.SYN.push("(");
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
        if(w == tM.countStrMap[i]||w == tM.identifiterMap[i]){
            w="i";
        }
        
    R1:string x = ll1.SYN.top();
        ll1.SYN.pop();
        cout<<"POP(     "<<x<<"     )"<<endl;
        //x is in VT ?
        if(ll1.VT.find(x)>=0&&ll1.VT.find(x)<ll1.VT.length()&&x!="#"){
            
            if(x==w){//x == w
                continue;
            }else{
                cout<<"ERROR1"<<endl;
                exit(1);
            }
        }
        // x is in VN ?
        else if(ll1.VN.find(x)>=0&&ll1.VN.find(x)<ll1.VN.length()){
            int resultOfanalysisTable;
            if(ll1.VN.find(x)>=0&&ll1.VN.find(x)<ll1.VN.length()&&ll1.VT.find(w)>=0&&ll1.VT.find(w)<ll1.VT.length()){
                
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
    return 0;
}


