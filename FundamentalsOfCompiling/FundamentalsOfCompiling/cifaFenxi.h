#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
using namespace std;
#define max1 1000//假设 C 源程序中，字符的个数最大不超过 max1 。
#define numOfCount 10 //假设文件中常数的个数。
#define numOfIdentifiter 100
class middleVector{
public:
    string identifier_middle_vector="";
    int flag_key_indentifiter=0;
    int flag_operator=0;
    int num_n=0;//在常数识别的时候会用到。
    int num_p=0;
    int num_m=0;
    int num_t=0;
    int num_e=1;
    int num_state;//若为0，表示正数。
    string char_num="";
    
}mV;
class outFile{//将得到的 token 序列写入到对应的 string 中。
public:
    string oFKeywords= "";//关键字
    string oFDelimiters= "";//界符
    string oFOperator = "";//运算符
    string oFIdentifiter[numOfIdentifiter];//标识符
    double oFCount[numOfCount];//以 double 的形式存储常数表
}oF;
int cifaFenxi(){
    ifstream fkey;fkey.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Ckeywords.txt");
    if(!fkey.is_open())exit(0);//如果打开失败，退出并返回0
    int num_keywords;fkey>>num_keywords;//cout<<num_keywords<<endl;
    string char_key[num_keywords];
    string str_key="";
    for (int i=0;i<num_keywords;i++){
        fkey>>char_key[i];
        //cout<<char_key[i]<<endl;//char_keys[i][0]表示每一个关键字的第一个字母。
        str_key = str_key + char_key[i]+"*";//将关键字以 string 写入 str_key
        if(8-char_key[i].length()>0){
            for(int j=0;j<8-char_key[i].length();j++){
                str_key = str_key + "*";
            }
        }
    }
    str_key="*"+str_key;
    /*for(int ffff =0;ffff<str_key.length();ffff++){
     cout<<str_key[ffff]<<endl;
     }*/
    //cout<<str_key<<" "<<str_key.length()<<endl;
    fkey.close();
    //将写在文本中的界符读入到 char_delimiters中。
    ifstream fdelimilter;fdelimilter.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Cdelimiters.txt");
    if(!fdelimilter.is_open())exit(0);
    int num_delimiters;fdelimilter>>num_delimiters;//cout<<num_delimiters<<endl;
    string char_delimiters[num_delimiters];
    string str_delimiter;
    for(int i=0;i<num_delimiters;i++){
        fdelimilter>>char_delimiters[i];
        //cout<<char_delimiters[i]<<endl;
        str_delimiter=str_delimiter+char_delimiters[i]+" ";
    }
    //cout<<str_delimiter<<str_delimiter.length()<<endl;
    fdelimilter.close();
    //将写在文本中的运算符读入到 char_operators 中。
    ifstream foperator;foperator.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Coperators.txt");
    if(!foperator.is_open())exit(0);
    int num_operators;foperator>>num_operators;//cout<<num_operators<<endl;
    string char_operators[num_operators];
    string str_operator;
    for(int i=0;i<num_operators;i++){
        foperator>>char_operators[i];
        //cout<<char_operators[i]<<endl;
        str_operator=str_operator+char_operators[i]+"$";
        if(3-char_operators[i].length()>0){
            for(int j=0;j<3-char_operators[i].length();j++){
                str_operator = str_operator + "$";
            }
        }
    }
    str_operator="$"+str_operator;
    //cout<<str_operator<<str_operator.length()<<endl;
    foperator.close();
    //开始处理，编写自动机。
    //将写在文本中的 C 源程序读入到 c_source中。
    ifstream fsource;fsource.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Csource.txt");
    if(!fsource.is_open())exit(0);
    int source_line=0;//表示：string c_source[max1]中的 max1实际最大为 source_line
    //int line_num;fsource>>line_num;//cout<<line_num<<endl;
    string c_source[max1];
    string str_source;
    for (int i=0; i<max1; i++) {//每次读取字符时，以空格和换行为结束。
        fsource>>c_source[i];
        //cout<<c_source[i]<<endl;
        /*if(c_source[i].length()!=0){
         cout<<" *"<<c_source[i].length()<<endl;
         }*/
        str_source=str_source+c_source[i]+" ";
        if(c_source[i].length()==0){
            source_line=i;
            //cout<<source_line<<endl;
            break;
        }
    }
    //对 C 源程序进行处理。
    int countFlag = 0;
    /*
     当 countFlag 为0时，表示还没有常数读入到oFCount[]中，故此时不用判断新读到的常数是否已经在oFCount[] 里面;当 countFlag 不为 0 时，需要判断一下，若 oFCount[]中没有，则将新读取到的数写入到 oFCount[] 中。
     */
    int identifiterFlag =0;//道理同上。
    for(int i=0;i<str_source.length();i++){
        //cout<<str_source[i]<<endl;
        //识别错误的标识符。识别错误的标识符。识别错误的标识符。识别错误的标识符。识别错误的标识符。
        if('0'<=str_source[i]&&str_source[i]<='9'&&str_source[i-1]==' '){
            cout<<str_source[i]<<str_source[i+1]<<" is wrong indentifiter!"<<endl;
            while (str_source[i]=='_' || ('a'<=str_source[i] && str_source[i]<='z') ||('A'<=str_source[i] && str_source[i]<='Z')||('0'<=str_source[i]&&str_source[i]<='9')) {
                i+=1;
            }
            i=i-1;
            continue;
        }
        //识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。
        {
            mV.char_num="";
            mV.num_n=0;
            mV.num_p=0;int p_flag=0;
            mV.num_m=0;
            mV.num_t=0;
            mV.num_e=1;
            mV.num_state=1;//若为1，表示正数。-1为负数。
            string char_num="";
            
            if((('0'<=str_source[i]&&str_source[i]<='9')&&str_source[i-1]=='=')||(str_source[i]=='-'&&str_source[i-1]=='=')||(('0'<=str_source[i]&&str_source[i]<='9')&&str_source[i-1]=='[')){
                
                while (('0'<=str_source[i]&&str_source[i]<='9')||str_source[i]=='e'||str_source[i]=='-'||str_source[i]=='+'||str_source[i]=='.') {
                    if(str_source[i]=='-'&&str_source[i-1]=='='){
                        mV.char_num=mV.char_num+"-";
                        mV.num_state=-1;
                        i+=1;
                    }
                    if (str_source[i]=='e') {
                        mV.char_num=mV.char_num+"e";
                        mV.num_p=1;i+=1;p_flag=1;
                    }
                    if(str_source[i]=='.'){
                        mV.char_num=mV.char_num+".";
                        mV.num_t=1;i+=1;
                    }
                    if(str_source[i]=='-'&&str_source[i-1]!='='){
                        mV.char_num=mV.char_num+"-";
                        mV.num_e=-1;i+=1;//MV.num_e 初始值为 1
                    }
                    if ('0'<=str_source[i]&&str_source[i]<='9') {
                        string str_source_one="";
                        if(mV.num_p==0&&mV.num_t==0){
                            mV.char_num=mV.char_num+str_source[i];
                            str_source_one = str_source_one + str_source[i];
                            mV.num_n=mV.num_n*10+stoi(str_source_one);
                            i+=1;
                        }
                        if(mV.num_p==0&&mV.num_t==1){
                            mV.char_num=mV.char_num+str_source[i];
                            str_source_one = str_source_one + str_source[i];
                            mV.num_n=mV.num_n*10+stoi(str_source_one);
                            mV.num_m=mV.num_m+1;
                            i+=1;
                        }
                        if(mV.num_p>0&&mV.num_t==0){
                            if(mV.num_p==1&&p_flag==1){
                                mV.num_p=mV.num_p-1;p_flag=0;
                            }
                            mV.char_num=mV.char_num+str_source[i];
                            str_source_one = str_source_one + str_source[i];
                            mV.num_p=mV.num_p*10+stoi(str_source_one);
                            i+=1;
                        }
                        if(mV.num_p>0&&mV.num_t==1){
                            if(mV.num_p==1&&p_flag==1){
                                mV.num_p=mV.num_p-1;p_flag=0;
                            }
                            mV.char_num=mV.char_num+str_source[i];
                            str_source_one = str_source_one + str_source[i];
                            mV.num_p=mV.num_p*10+stoi(str_source_one);
                            
                            i+=1;
                        }
                    }
                }//while
                
                if(countFlag == 0){
                    oF.oFCount[countFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);
                    countFlag +=1;
                    cout<<mV.char_num<<"  {c, "<<countFlag<<"} "<<mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)<<endl;
                    i=i-1;
                    continue;
                }
                if(countFlag!=0){
                    int f_l_a_g =0;
                    for(int cF = 0;cF<countFlag;cF++){
                        if (mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)==oF.oFCount[cF]) {
                            f_l_a_g +=1;break;
                        }
                    }
                    if(f_l_a_g ==0){
                        oF.oFCount[countFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);
                        countFlag +=1;
                        cout<<mV.char_num<<"  {c, "<<countFlag<<"} "<<mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)<<endl;
                        i=i-1;
                        continue;
                    }
                }
            }
            
        }
        
        //识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。
        {
            if(str_source[i]=='_' || ('a'<=str_source[i] && str_source[i]<='z') ||('A'<=str_source[i] && str_source[i]<='Z')||('0'<=str_source[i]&&str_source[i]<='9')){
                mV.flag_key_indentifiter = 1;
                mV.identifier_middle_vector=mV.identifier_middle_vector+str_source[i];
            }
            else{
                //cout<<MV.identifier_middle_vector<<endl;
                if(mV.identifier_middle_vector!=""){
                    string::size_type idx_keywords=0;
                    idx_keywords=str_key.find("*"+mV.identifier_middle_vector+"*");
                    if(0<=idx_keywords && idx_keywords<=str_key.length()){
                        cout<<mV.identifier_middle_vector<<"  {k, "<<idx_keywords/9+1<<"}"<<endl;
                    }else{
                        if(identifiterFlag==0){
                            oF.oFIdentifiter[identifiterFlag]=mV.identifier_middle_vector;
                            identifiterFlag +=1;
                            cout<<mV.identifier_middle_vector<<"  {i, "<<identifiterFlag<<"}"<<endl;
                        }
                        if(identifiterFlag!=0){
                            int fl_ag=0;
                            for(int iF =0;iF<identifiterFlag;iF++){
                                if(mV.identifier_middle_vector == oF.oFIdentifiter[iF]){
                                    fl_ag+=1;break;
                                }
                            }
                            if(fl_ag==0){
                                oF.oFIdentifiter[identifiterFlag]=mV.identifier_middle_vector;
                                identifiterFlag+=1;
                                cout<<mV.identifier_middle_vector<<"  {i, "<<identifiterFlag<<"}"<<endl;
                            }
                        }
                    }
                    mV.identifier_middle_vector="";
                }
            }
            
        }
    }
    ////识别运算符。识别运算符。识别运算符。识别运算符。识别运算符。识别运算符。识别运算符。
    for(int i=0;i<str_source.length();i++){
        string operator_middle_vector3="";
        string operator_middle_vector2="";
        string operator_middle_vector1="";
        while(str_source[i]=='-'||str_source[i]=='~'||str_source[i]=='+'||str_source[i]=='*'||str_source[i]=='/'||str_source[i]=='&'||str_source[i]=='%'||str_source[i]=='<'||str_source[i]=='>'||str_source[i]=='='||str_source[i]=='!'||str_source[i]=='^'||str_source[i]=='|'||str_source[i]=='?'){
            operator_middle_vector1=str_source[i];//判断由1个符号组成的运算符
            operator_middle_vector2=operator_middle_vector1+str_source[i+1];//判断由2个符号组成的运算符
            operator_middle_vector3=operator_middle_vector2+str_source[i+2];//判断由3个b符号组成的运算符
            if(operator_middle_vector2=="//"||operator_middle_vector2=="/*"||operator_middle_vector2=="*/"){//将界符排除,为了减少机器运行时的压力。
                i+=1;
                break;
            }
            if(0<str_operator.find("$"+operator_middle_vector3+"$")&&str_operator.find("$"+operator_middle_vector3+"$")<str_operator.length()){
                cout<<operator_middle_vector3<<"  {o, "<<str_operator.find("$"+operator_middle_vector3+"$")/4+1<<"{"<<endl;
                i=i+2;break;
            }
            if (0<str_operator.find("$"+operator_middle_vector2+"$")&&str_operator.find("$"+operator_middle_vector2+"$")<str_operator.length()) {
                cout<<operator_middle_vector2<<"  {o, "<<str_operator.find("$"+operator_middle_vector2+"$")/4+1<<"}"<<endl;
                i=i+1;break;
            }
            if (0<str_operator.find("$"+operator_middle_vector1+"$")&&str_operator.find("$"+operator_middle_vector1+"$")<str_operator.length()){
                cout<<operator_middle_vector1<<"  {o, "<<str_operator.find("$"+operator_middle_vector1+"$")/4+1<<"}"<<endl;
                break;
            }
        }
    }
    for(int i=0;i<source_line;i++){
        for(int j=0;j<c_source[i].length();j++){
            //cout<<c_source[i][j]<<endl;
            {//查找源程序中的界符。
                string::size_type idx_delimiter=0;
                //size_type类型定义为与unsigned型（unsigned int 或 unsigned long）具有相同含义。且可以保证足够大能够存储任意string对象的长度
                if(c_source[i][j]!='/'&&c_source[i][j]!='*'){//这两个符号在运算符中也有，且出现在第一个字母中
                    idx_delimiter = str_delimiter.find(c_source[i][j]);
                    if(0<=idx_delimiter && idx_delimiter<=23){
                        cout<<c_source[i][j]<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                    }
                }
                if(c_source[i][j]=='/'){
                    if(c_source[i][j+1]=='/'){
                        idx_delimiter = str_delimiter.find("//");
                        cout<<"//"<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                    }
                    if(c_source[i][j+1]=='*'){
                        idx_delimiter = str_delimiter.find("/*");
                        cout<<"/*"<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                    }
                }
                if(c_source[i][j]=='*'){
                    if(c_source[i][j+1]=='/'){
                        idx_delimiter = str_delimiter.find("*/");
                        cout<<"*/"<<"  {p, "<<(idx_delimiter+2)/2-1<<"}"<<endl;
                    }
                }
                continue;
            }
        }
    }
    fsource.close();
    return 0;
}

