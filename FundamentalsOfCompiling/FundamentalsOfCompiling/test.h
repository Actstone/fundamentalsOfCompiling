#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
#include <map>
using namespace std;
#define max1 100//假设 C 源程序中，不超过 lineNum 行。
#define numOfCount 10 //假设文件中常数的个数。
#define numOfIdentifiter 100
class tokenMap{
public:
    int mapFlag = 0;
    
    map<int,char>charMap;
    map<int,char>::iterator iterChar;
    
    map<int,string>keyMap;//完成
    map<int, string>::iterator iterKeywords;
    
    map<int,double>countMap;//完成
    map<int, double>::iterator iterCount;
    
    map<int,string>operatorMap;//operator + delimiters 完成
    map<int, string>::iterator iterOper;
    
    map<int,string>identifiterMap;//完成
    map<int, string>::iterator iterIden;
    
    map<int,string>strMap;
    map<int, string>::iterator iterStr;
}tM;

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

int test(){
    map<int,string>mapStu;
    ofstream oftoken;oftoken.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/token.txt",ios::out);
    if(!oftoken.is_open())exit(0);
    
    ifstream fkey;fkey.open("/Users/shiyi/Desktop/Actstone/FundamentalsOfCompiling/FundamentalsOfCompiling/Ckeywords.txt");
    if(!fkey.is_open())exit(0);//如果打开失败，退出并返回0
    int num_keywords;fkey>>num_keywords;//cout<<num_keywords<<endl;
    string char_key[num_keywords];
    string str_key="";
    for (int i=0;i<num_keywords;i++){
        fkey>>char_key[i];
        //cout<<char_key[i]<<endl;//char_keys[i][0]表示每一个关键字的第一个字母。
        str_key = str_key + char_key[i]+"^";//将关键字以 string 写入 str_key
        if(8-char_key[i].length()>0){
            for(int j=0;j<8-char_key[i].length();j++){
                str_key = str_key + "^";
            }
        }
    }
    str_key="^"+str_key;
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
    
    //int source_line=0;//表示：string c_source[max1]中的 max1实际最大为 source_line
    string c_source[max1];
    string str_source="";
    int lineNum = 0;
    for (int i=0; i<max1; i++) {//每次读取一行字符
        getline(fsource,c_source[i]);
        if(c_source[i].empty()){
            lineNum = i;
            break;
        }//cout<<lineNum<<endl;
        c_source[i]=c_source[i]+'\n';
        str_source=str_source+c_source[i];
        
    }
    
    int countFlag = 0;
    /*
     当 countFlag 为0时，表示还没有常数读入到oFCount[]中，故此时不用判断新读到的常数是否已经在oFCount[] 里面;当 countFlag 不为 0 时，需要判断一下，若 oFCount[]中没有，则将新读取到的数写入到 oFCount[] 中。
     */
    int identifiterFlag =0;//道理同上。
    int strFlag=0;
    int doublequotesIndicator=0;
    int charFlag=0;
    
    tM.mapFlag = 0;
    
    for(int i=0;i<str_source.length();i++){
        //cout<<str_source[i]<<endl;
        
        if(str_source[i]=='/'&&str_source[i+1]=='/'){// 若遇到 // ,直接跳过该行。
            i=i+2;
            while(str_source[i]!='\n'){
                i+=1;
            }i+=1;
        }
        
        if(str_source[i]=='/'&&str_source[i+1]=='*'){
            //如果遇到/* */，则将符号内的内容直接略过
            i+=2;
            while(!(str_source[i]=='*'&&str_source[i+1]=='/')){
                i+=1;
            }i+=2;
        }
        
        if(str_source[i]=='\''){
            i+=1;
            if(charFlag==0){
                tM.charMap[charFlag]=str_source[i];
                //cout<<tM.charMap[charFlag]<<" {ch, "<<charFlag<<"}"<<endl;
                charFlag+=1;
            }
            if(charFlag!=0){
                int Char_Flag_MIddle =0;
                for(int c_f =0;c_f<charFlag;c_f++){
                    if(tM.charMap[c_f]==str_source[i]){
                        Char_Flag_MIddle +=1;break;
                    }
                }
                if(Char_Flag_MIddle==0){
                    tM.charMap[charFlag]=str_source[i];
                    //cout<<tM.charMap[charFlag]<<" {ch, "<<charFlag<<"}"<<endl;
                    charFlag+=1;
                }
            }i+=1;continue;
        }
        //识别字符串识别字符串识别字符串识别字符串识别字符串
        string strMiddle="";
        if(str_source[i]=='"'){
            doublequotesIndicator+=1;
            if(doublequotesIndicator%2!=0){
                i+=1;
                while(str_source[i]!='"'){
                    strMiddle=strMiddle+str_source[i];i+=1;
                    if(str_source[i]=='\0'){
                        break;
                    }
                }doublequotesIndicator+=1;//此时的 i 指示的是字符串后面的引号，所以将 yinhaoZhishi 加1。当 i 返回的 for 循环时，直接加1，只是引号后面的字符。
                if(strFlag==0){
                    tM.strMap[strFlag]=strMiddle;
                    //cout<<tM.strMap[strFlag]<<" {s, "<<strFlag<<"}"<<endl;
                    strFlag+=1;
                }
                if(strFlag!=0){
                    int str_fl_ag =0;
                    for(int sF =0;sF<strFlag;sF++){
                        if(strMiddle==tM.strMap[sF]){
                            str_fl_ag+=1;break;
                        }
                    }
                    if(str_fl_ag==0){
                        tM.strMap[strFlag]=strMiddle;
                        //cout<<tM.strMap[strFlag]<<" {s, "<<strFlag<<"}"<<endl;
                        strFlag+=1;
                    }
                }
            }
        }
        
        
        //识别错误的标识符。识别错误的标识符。识别错误的标识符。识别错误的标识符。识别错误的标识符。
        /*if('0'<=str_source[i]&&str_source[i]<='9'&&(str_source[i-1]==' '||str_source[i-1]=='+'||str_source[i-1]=='-'||str_source[i-1]=='*'||str_source[i-1]=='/')&&((str_source[i+1]<'e'&&str_source[i+1]>='a')||(str_source[i+1]>'e'&&str_source[i+1]<='z')||(str_source[i+1]>='A'&&str_source[i+1]<='Z'))){
            
            cout<<str_source[i]<<str_source[i+1]<<" is wrong indentifiter!"<<endl;
            
            while (str_source[i]=='_' || ('a'<=str_source[i] && str_source[i]<='z') ||('A'<=str_source[i] && str_source[i]<='Z')||('0'<=str_source[i]&&str_source[i]<='9')) {
                i+=1;
            }
            i=i-1;
            continue;
        }*/
        //识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。识别常数。
        
            mV.char_num="";
            mV.num_n=0;
            mV.num_p=0;int p_flag=0;
            mV.num_m=0;
            mV.num_t=0;
            mV.num_e=1;
            mV.num_state=1;//若为1，表示正数。-1为负数。
            string char_num="";
            int minusNum = 0;//判断第几次出现负号
            if(('0'<=str_source[i]&&str_source[i]<='9')||(str_source[i]=='-'))
            //当扫描到一个字符为0~9或者为-，进入常数判断。
           /* if((('0'<=str_source[i]&&str_source[i]<='9')&&str_source[i-1]=='=')||(str_source[i]=='-'&&(str_source[i-1]=='='||str_source[i-1]=='\n'))||(('0'<=str_source[i]&&str_source[i]<='9')&&str_source[i-1]=='['))*/
            
            {
                if(str_source[i]=='-'){
                    if((str_source[i+1]<='z'&&str_source[i+1]>='a')||(str_source[i+1]<='Z'&&str_source[i+1]>='A')){
                        continue;
                    }
                }
                
                while (('0'<=str_source[i]&&str_source[i]<='9')||str_source[i]=='e'||str_source[i]=='-'||str_source[i]=='.') {
                    
                    if(str_source[i]=='-'){
                        if(minusNum == 0){
                            mV.char_num=mV.char_num+"-";
                            mV.num_state=-1;
                            i+=1;
                            minusNum +=1;
                        }
                        if(minusNum == 1){
                            mV.char_num=mV.char_num+"-";
                            mV.num_e=-1;i+=1;//MV.num_e 初始值为 1
                            minusNum +=1;
                        }
                        if(minusNum>1){
                            break;
                        }
                    }
                    if (str_source[i]=='e') {
                        mV.char_num=mV.char_num+"e";
                        mV.num_p=1;i+=1;p_flag=1;
                    }
                    if(str_source[i]=='.'){
                        mV.char_num=mV.char_num+".";
                        mV.num_t=1;i+=1;
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
                if ((str_source[i] == ','||str_source[i]==';'||str_source[i] == ')'||str_source[i] == '+'||str_source[i] == '-'||str_source[i] == '*'||str_source[i] == '/'||str_source[i] == ']'||str_source[i] == '#'||str_source[i] == '%'||str_source[i]==' ')&&(str_source[i-2]!='_')&&(str_source[i-2]<'a'||str_source[i-2]>'z')&&(str_source[i-2]<'A'||str_source[i-2]>'Z')) {
                    tM.countMap[tM.mapFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);tM.mapFlag +=1;
                   /*
                    if(countFlag == 0){
                        tM.countMap[tM.mapFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);
                        //oF.oFCount[countFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);
                        countFlag +=1;tM.mapFlag +=1;
                        //cout<<mV.char_num<<"  {c, "<<countFlag<<"} "<<mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)<<endl;
                        i=i-1;
                        continue;
                    }
                    if(countFlag!=0){
                        int f_l_a_g =0;
                        for(int cF = 0;cF<=tM.mapFlag;cF++){
                            if (mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)==tM.countMap[cF]) {
                                
                                f_l_a_g +=1;break;
                            }
                        }
                        if(f_l_a_g ==0){
                            tM.countMap[tM.mapFlag]=mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m);
                            tM.mapFlag +=1;
                            countFlag +=1;
                            
                            //cout<<mV.char_num<<"  {c, "<<countFlag<<"} "<<mV.num_state*mV.num_n*pow(10, mV.num_e*mV.num_p-mV.num_m)<<endl;
                            i=i-1;
                            continue;
                        }
                    }*/
                }
                
            }
            
        
        //识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。识别关键字和标识符。
        {
            if(str_source[i]=='_' || ('a'<=str_source[i] && str_source[i]<='z') ||('A'<=str_source[i] && str_source[i]<='Z')||('0'<=str_source[i]&&str_source[i]<='9'&&(str_source[i-1]=='_'||(('a'<=str_source[i-1] && str_source[i-1]<='z') ||('A'<=str_source[i-1] && str_source[i-1]<='Z'))))){
                mV.flag_key_indentifiter = 1;
                mV.identifier_middle_vector=mV.identifier_middle_vector+str_source[i];
            }
            else{
                //cout<<MV.identifier_middle_vector<<endl;
                if(mV.identifier_middle_vector!=""){
                    string::size_type idx_keywords=0;
                    idx_keywords=str_key.find("^"+mV.identifier_middle_vector+"^");
                    if(0<=idx_keywords && idx_keywords<=str_key.length()){
                        tM.keyMap[(int)idx_keywords/9+1]=mV.identifier_middle_vector;
                        //cout<<mV.identifier_middle_vector<<"  {k, "<<idx_keywords/9+1<<"}"<<endl;
                    }else{
                        tM.identifiterMap[tM.mapFlag]=mV.identifier_middle_vector;
                        tM.mapFlag +=1;
                        /*
                        if(identifiterFlag==0){
                            tM.identifiterMap[tM.mapFlag]=mV.identifier_middle_vector;
                            tM.mapFlag +=1;
                            identifiterFlag +=1;
                            //cout<<mV.identifier_middle_vector<<"  {i, "<<identifiterFlag<<"}"<<endl;
                        }
                        if(identifiterFlag!=0){
                            int fl_ag=0;
                            for(int iF =0;iF<=tM.mapFlag;iF++){
                                if(mV.identifier_middle_vector == tM.identifiterMap[iF]){
                                    fl_ag+=1;break;
                                }
                            }
                            if(fl_ag==0){
                                tM.identifiterMap[tM.mapFlag]=mV.identifier_middle_vector;
                                tM.mapFlag+=1;
                                identifiterFlag+=1;
                                //cout<<mV.identifier_middle_vector<<"  {i, "<<identifiterFlag<<"}"<<endl;
                            }
                        }*/
                    }
                    mV.identifier_middle_vector="";
                }
            }
        }
    
        string operator_middle_vector3="";
        string operator_middle_vector2="";
        string operator_middle_vector1="";
        while(str_source[i]=='-'||str_source[i]=='~'||str_source[i]=='+'||str_source[i]=='*'||str_source[i]=='/'||str_source[i]=='&'||str_source[i]=='%'||str_source[i]=='<'||str_source[i]=='>'||str_source[i]=='='||str_source[i]=='!'||str_source[i]=='^'||str_source[i]=='|'||str_source[i]=='?'){
            operator_middle_vector1=str_source[i];//判断由1个符号组成的运算符
            operator_middle_vector2=operator_middle_vector1+str_source[i+1];//判断由2个符号组成的运算符
            operator_middle_vector3=operator_middle_vector2+str_source[i+2];//判断由3个b符号组成的运算符
            if(operator_middle_vector2=="//"||operator_middle_vector2=="/*"||operator_middle_vector2=="*/"||operator_middle_vector2=="\n"){//将界符排除,为了减少机器运行时的压力。
                i+=1;
                break;
            }
            if(0<str_operator.find("$"+operator_middle_vector3+"$")&&str_operator.find("$"+operator_middle_vector3+"$")<str_operator.length()){
                tM.operatorMap[tM.mapFlag]=operator_middle_vector3;
                //cout<<operator_middle_vector3<<"  {o, "<<str_operator.find("$"+operator_middle_vector3+"$")/4+1<<"{"<<endl;
                tM.mapFlag +=1;
                i=i+2;break;
            }
            if (0<str_operator.find("$"+operator_middle_vector2+"$")&&str_operator.find("$"+operator_middle_vector2+"$")<str_operator.length()) {
                tM.operatorMap[tM.mapFlag]=operator_middle_vector2;
                //cout<<operator_middle_vector2<<"  {o, "<<str_operator.find("$"+operator_middle_vector2+"$")/4+1<<"}"<<endl;
                tM.mapFlag+=1;
                i=i+1;break;
            }
            if (0<str_operator.find("$"+operator_middle_vector1+"$")&&str_operator.find("$"+operator_middle_vector1+"$")<str_operator.length()){
                tM.operatorMap[tM.mapFlag]=operator_middle_vector1;
                //cout<<operator_middle_vector1<<"  {o, "<<str_operator.find("$"+operator_middle_vector1+"$")/4+1<<"}"<<endl;
                tM.mapFlag += 1;
                break;
            }
        }
    
        //cout<<c_source[i][j]<<endl;
        {//查找源程序中的界符。
            string::size_type idx_delimiter=0;
            //size_type类型定义为与unsigned型（unsigned int 或 unsigned long）具有相同含义。且可以保证足够大能够存储任意string对象的长度
            if(str_source[i]!='/'&&str_source[i]!='*'&&str_source[i]!=' '){
                //这两个符号在运算符中也有，且出现在第一个字母中。而判断不等于空格是因为在 str_delimiter 中存在空格。
                idx_delimiter = str_delimiter.find(str_source[i]);
                
                if(0<=idx_delimiter && idx_delimiter<=33){
                    //cout<<"idx_delimiter="<<idx_delimiter<<endl;
                    tM.operatorMap[tM.mapFlag]=str_source[i];tM.mapFlag+=1;
                    //cout<<c_source[i][j]<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                }
            }
            if(str_source[i]=='/'){
                if(str_source[i+1]=='/'){
                    idx_delimiter = str_delimiter.find("//");
                    tM.operatorMap[tM.mapFlag]="//";tM.mapFlag+=1;
                    //cout<<"//"<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                }
                if(str_source[i+1]=='*'){
                    idx_delimiter = str_delimiter.find("/*");
                    tM.operatorMap[tM.mapFlag]="/*";tM.mapFlag+=1;
                    //cout<<"/*"<<"  {p, "<<(idx_delimiter+2)/2<<"}"<<endl;
                }
            }
            if(str_source[i]=='*'){
                if(str_source[i+1]=='/'){
                    idx_delimiter = str_delimiter.find("*/");
                    tM.operatorMap[tM.mapFlag]="*/";tM.mapFlag+=1;
                    //cout<<"*/"<<"  {p, "<<(idx_delimiter+2)/2-1<<"}"<<endl;
                }
            }
            continue;
        }
    
    
    }
    

    
    
    fsource.close();
    
    /*
    cout<<"-----------字符串-----------"<<endl;
    for(tM.iterStr=tM.strMap.begin();tM.iterStr != tM.strMap.end();tM.iterStr++){
        cout<<tM.iterStr->first<<" "<<tM.iterStr->second<<endl;
        oftoken<<tM.iterStr->second<<"  { s, "<<tM.iterStr->first<<" }"<<endl;
    }
    
    cout<<"-----------字符------------"<<endl;
    for(tM.iterChar=tM.charMap.begin();tM.iterChar != tM.charMap.end();tM.iterChar++){
        cout<<tM.iterChar->first<<" "<<tM.iterChar->second<<endl;
        oftoken<<tM.iterChar->second<<"  { ch, "<<tM.iterChar->first<<" }"<<endl;
    }
    
    cout<<"-----------关键字-----------"<<endl;
    for(tM.iterKeywords=tM.keyMap.begin();tM.iterKeywords!= tM.keyMap.end();tM.iterKeywords++){
        cout<<tM.iterKeywords->first<<" "<<tM.iterKeywords->second<<endl;
        oftoken<<tM.iterKeywords->second<<"  { k, "<<tM.iterKeywords->first<<" }"<<endl;
    }
    
    cout<<"-----------界符------------"<<endl;
    for(tM.iterOper=tM.operatorMap.begin();tM.iterOper!=tM.operatorMap.end();tM.iterOper++){
        cout<<tM.iterOper->first<<" "<<tM.iterOper->second<<endl;
        oftoken<<tM.iterOper->second<<"  { p, "<<tM.iterOper->first<<" }"<<endl;
    }
    cout<<"-----------标识符-----------"<<endl;
    for(tM.iterIden=tM.identifiterMap.begin();tM.iterIden!=tM.identifiterMap.end();tM.iterIden++){
        cout<<tM.iterIden->first<<" "<<tM.iterIden->second<<endl;
        oftoken<<tM.iterIden->second<<"  { i, "<<tM.iterIden->first<<" }"<<endl;
    }
    cout<<"-----------常数------------"<<endl;
    for(tM.iterCount=tM.countMap.begin();tM.iterCount!=tM.countMap.end();tM.iterCount++){
        cout<<tM.iterCount->first<<" "<<tM.iterCount->second<<endl;
        oftoken<<tM.iterCount->second<<"  { c, "<<tM.iterCount->first<<" }"<<endl;
    }
    */
    oftoken.close();
    return 0;
}

