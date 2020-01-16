#include <iostream> //includes input and output commands (cin and cout);
#include <string.h> //includes string class and string manipulation commands;
#include <fstream>  //includes file manipulation commands;
#include <stdlib.h> //includes functions to allocate memory and conversions;
#include <math.h>  //includes a set of math operations;
#include <algorithm> //includes a collection of functions designed to be used on ranges of vector elements;
#include <iterator> //includes the iterator class, to navigate through dinamic lists;
#include <sstream> //same as iostream, but for strings;
#include <time.h> //includes time verification commands;
#include <windows.h> //includes windows specific commands;
#include <psapi.h> //includes memory manipulation commands;
//To use psapi functions it's also needed to link the psapi library in the compiler;
//In codeblocks do: RightClick on project > Build options > Linker settings > Add > psapi.
#include <bitset> //allows the bitset command.

#include "MajorityFunction.h" //includes the MajorityFunction class, that defines every majority function used in the code;

using namespace std;

//Sort a PrimitiveFunction list by gates>inverters>literals;
//Receives two classes as input and returns a true (bool operator) value if class A has a lower cost than class B;
//This function only verifies if class A has lower cost than class B and is meant to be used with the c++ "sort" command.
struct sortByCost{
    bool operator()(MajorityFunction const &a, MajorityFunction const &b) const{
        if(a.getGates() < b.getGates()){
            return true;
        }else{
            if(a.getGates() == b.getGates() && a.getInverters() < b.getInverters()){
                return true;
            }else{
                if(a.getInverters() == b.getInverters() && a.getLiterals() < b.getLiterals()){
                    return true;
                }else{
                    return false;
                }
            }
        }
    }
};

//Search a truth table in an MajorityFunction list;
//Receives a string TT (truth table) as input and returns a true value (bool operator) if the minterms covered by class A is equal to TT;
//This functions encodes a condition to be used with the c++ "find_if" command.
struct findFunction{
    string TT;
    findFunction(string TT) : TT(TT) {}
    bool operator () (MajorityFunction const &a) const{
        return a.getCoveredMinterms() == TT;
    }
};

//Generates the truth table for every primitive function built in the "fillPrimitiveList" function;
string generateTruthTable(int n, int minterms, string F){
    string binary;
    int b = 0;
    string result;
    char ac;
    char ac2;

    for(int i=0;i<minterms;i++){
        b = 0;
        char vbinary[n-1];
        itoa(i,vbinary,2);
        binary = vbinary;
        while(binary.size() != n){
            binary = "0" + binary;
        }

        for(int j=0;j<F.size();j++){

            if(F[j] == 'x'){
                for(int c=1;c<=binary.size();c++){
                    ac2 = c + 64;
                    if(ac2 == F[j-1]){
                        if(binary[c-1] == '1'){
                            b = b-1;
                        }
                        if(binary[c-1] == '0'){
                            b = b+1;
                        }
                    }
                }
            }

            if(F[j] == '1'){
                b = b + 1;
            }

            for(int a=1;a<=binary.size();a++){
                 ac = a + 64;

                 if(F[j] == ac){
                    if(binary[a-1] == '1'){
                      b = b + 1;
                    }
                }
            }
        }

        if(F.size() == 2){
          if(b == 1){
            result = result + "1";
          }
          else{
            result = result + "0";
          }
        }else{
          if(b > 1){
            result = result + "1";
          }
          else{
            result = result + "0";
          }
        }
    }
    return result;
}

//Builds the primitives List:
//First the algorithm builds functions for the set C, the constants 0 and 1;
//Then the algorithm builds 1-input functions for the set V, followed by 2-input functions for the set G;
//Last, the algorithm builds 3-input functions for the set T;
//The function "generateTruthTable" is used to build the truth tables for every function built.
void fillPrimitiveList(vector<MajorityFunction>& newPrimitiveList, int n, int minterms){

    string tt;
    string tt2;

    for(int i=0;i<minterms;i++){
         tt = tt + "1";
         tt2 = tt2 + "0";
    }

    MajorityFunction newFunction("1",tt,0,0,0,0,0,0);
    newPrimitiveList.push_back(newFunction); //adds the object to the vector;
    MajorityFunction newFunction2("0",tt2,0,0,0,0,0,0);
    newPrimitiveList.push_back(newFunction2); //adds the object to the vector;

    int indice = 2;
    string F;

    //S = SINGLE INPUT/OUTPUT FUNCTIONS --- START
    for(int i=1;i<=n;i++){
        F = i + 64;

        string auxTT = generateTruthTable(n, minterms, F+" ");

        MajorityFunction newFunction(F,auxTT,0,0,1,0,0,0);
        newPrimitiveList.push_back(newFunction);
    }

    indice = 2 + n;

    for(int j=1;j<=n;j++){
        F = j + 64;
        string auxTT = generateTruthTable(n, minterms, F+"x");
        F = F + "'";

        MajorityFunction newFunction(F,auxTT,0,1,1,1,0,0);
        newPrimitiveList.push_back(newFunction);
    }

    //S = SINGLE INPUT/OUTPUT FUNCTIONS --- END
    indice = n*2 + 2;
    string output1, output2, auxTT;
    //AND/OR = AND/OR PRIMITIVE FUNCTIONS --- START
    //AND{
    for(int i=1;i<=n;i++){
        output1 = i + 64;
        for(int j=(i+1);j<=n;j++){
            string output2;
            output2 = j + 64;

            auxTT = generateTruthTable(n,minterms,output1+output2+"0");
            MajorityFunction newFunction("M(" + output1  + "," + output2 + ",0)",auxTT,1,0,2,0,0,0);
            newPrimitiveList.push_back(newFunction);

            auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"0");
            MajorityFunction newFunction1("M(" + output1  + "'," + output2 + ",0)",auxTT,1,1,2,0,0,0);
            newPrimitiveList.push_back(newFunction1);

            auxTT = generateTruthTable(n,minterms,output1+output2+"x"+"0");
            MajorityFunction newFunction2("M(" + output1  + "," + output2 + "',0)",auxTT,1,1,2,0,0,0);
            newPrimitiveList.push_back(newFunction2);

            auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"x"+"0");
            MajorityFunction newFunction3("M'(" + output1  + "," + output2 + ",1)",auxTT,1,1,2,1,0,0);
            newPrimitiveList.push_back(newFunction3);

            indice = indice+4;
        }
    }
    //}

    //OR{
    for(int i=1;i<=n;i++){
        output1 = i + 64;
        for(int j=(i+1);j<=n;j++){
            output2 = j + 64;

            auxTT = generateTruthTable(n,minterms,output1+output2+"1");
            MajorityFunction newFunction("M(" + output1  + "," + output2 + ",1)",auxTT,1,0,2,0,0,0);
            newPrimitiveList.push_back(newFunction);

            auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"1");
            MajorityFunction newFunction1("M(" + output1  + "'," + output2 + ",1)",auxTT,1,1,2,0,0,0);
            newPrimitiveList.push_back(newFunction1);

            auxTT = generateTruthTable(n,minterms,output1+output2+"x"+"1");
            MajorityFunction newFunction2("M(" + output1  + "," + output2 + "',1)",auxTT,1,1,2,0,0,0);
            newPrimitiveList.push_back(newFunction2);

            auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"x"+"1");
            MajorityFunction newFunction3("M'(" + output1  + "," + output2 + ",0)",auxTT,1,1,2,1,0,0);
            newPrimitiveList.push_back(newFunction3);

            indice = indice+4;
        }
    }
    //}
    //AND/OR = AND/OR PRIMITIVE FUNCTIONS --- END

    //T = POSSIBLE COMBINATIONS OF 3 VARIABLES --- START
    for(int a=1;a<=n;a++){
        output1 = a + 64;
        for(int i=(a+1);i<=n;i++){
            output2 = i + 64;
            for(int j=(i+1);j<=n;j++){
                char output3 = j + 64;

                auxTT = generateTruthTable(n,minterms,output1+output2+output3);
                MajorityFunction newFunction("M(" + output1  + "," + output2 + "," + output3 + ")",auxTT,1,0,3,0,0,0);
                newPrimitiveList.push_back(newFunction);

                auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"x"+output3+"x");
                MajorityFunction newFunction1("M'(" + output1  + "," + output2 + "," + output3 + ")",auxTT,1,1,3,1,0,0);
                newPrimitiveList.push_back(newFunction1);

                auxTT = generateTruthTable(n,minterms,output1+"x"+output2+output3);
                MajorityFunction newFunction2("M(" + output1  + "'," + output2 + "," + output3 + ")",auxTT,1,1,3,0,0,0);
                newPrimitiveList.push_back(newFunction2);

                auxTT = generateTruthTable(n,minterms,output1+output2+"x"+output3+"x");
                MajorityFunction newFunction3("M'(" + output1  + "'," + output2 + "," + output3 + ")",auxTT,1,2,3,1,0,0);
                newPrimitiveList.push_back(newFunction3);

                auxTT = generateTruthTable(n,minterms,output1+output2+"x"+output3);
                MajorityFunction newFunction4("M(" + output1  + "," + output2 + "'," + output3 + ")",auxTT,1,1,3,0,0,0);
                newPrimitiveList.push_back(newFunction4);

                auxTT = generateTruthTable(n,minterms,output1+"x"+output2+output3+"x");
                MajorityFunction newFunction5("M'(" + output1  + "," + output2 + "'," + output3 + ")",auxTT,1,2,3,1,0,0);
                newPrimitiveList.push_back(newFunction5);

                auxTT = generateTruthTable(n,minterms,output1+output2+output3+"x");
                MajorityFunction newFunction6("M(" + output1  + "," + output2 + "," + output3 + "')",auxTT,1,1,3,0,0,0);
                newPrimitiveList.push_back(newFunction6);

                auxTT = generateTruthTable(n,minterms,output1+"x"+output2+"x"+output3);
                MajorityFunction newFunction7("M'(" + output1  + "," + output2 + "," + output3 + "')",auxTT,1,2,3,1,0,0);
                newPrimitiveList.push_back(newFunction7);

                indice = indice+8;
            }
        }
     }
    //T = POSSIBLE COMBINATIONS OF 3 VARIABLES --- END
}

void fill2LvlTable(int n, vector<MajorityFunction>& newPrimitiveList){

  stringstream ss;
  ss << n;
  string str = ss.str();
  string inputPath;

  inputPath = "2Lvl" + str + ".txt";

  ifstream inputFile(inputPath.c_str()); //reads the input file

  string MFunction;
  int gates = 0;
  int inverters = 0;
  int literais = 0;
  string MBinary;
  int i = 0;

  while(!inputFile.eof()){//adds the informations in the input file to a vector of majority functions.
    inputFile >> i >> MFunction >> MBinary >> gates >> inverters >> literais;
    if(MFunction[1] == '\''){
        MajorityFunction newFunction(MFunction,MBinary, gates, inverters, literais,1,0,0);
        newPrimitiveList.push_back(newFunction);
    }else{
        MajorityFunction newFunction(MFunction,MBinary, gates, inverters, literais,0,0,0);
        newPrimitiveList.push_back(newFunction);
    }
  }
}

int verifyN4D4(string TT, clock_t startTime){
    int D4N4 = 0;
    if(TT == "0110100110010110"){
        cout << "Input Table: " << endl;
        cout << TT << endl;
        cout << "Majority Function: " << endl;
        cout << "M(B,M'(A,B,M(C,M'(A,C,0))),M(A,B',M(C,M'(A,C,0),M(A,C',0)))" << endl;
        cout << "Gates: 6 Inverters: 4 Literals: 10" << endl;
        D4N4 = 1;

    }else if(TT == "1001011001101001"){
        cout << "Input Table: " << endl;
        cout << TT << endl;
        cout << "Majority Function: " << endl;
        cout << "M'(B,M'(A,B,M(C,M'(A,C,0))),M(A,B',M(C,M'(A,C,0),M(A,C',0))))" << endl;
        cout << "Gates: 6 Inverters: 5 Literals: 10" << endl;
        D4N4 = 1;
    }

    if(D4N4 == 1){
        cout << "Runtime: " << endl;
        double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
        cout << time << " seconds." << endl;
        PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
        GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
        double i = (info.PeakWorkingSetSize);
        //covert bytes to KB
        i = (i/1024);
        //convert KB to MB
        i = (i/1024);
        cout << "Memory: " << endl;
        cout << i << " MB" << endl;
        cout << endl;
    }

    return D4N4;
}

//Builds the list P of valid pairs p1 + p2, selecting p1 and p2 as primitives;
//Only pairs that respect the following conditions are allowed in table P:
//  -The pair of functions p1+p2 has to cover all minterms in TT at least once;
//  -The pair of functions p1+p2 can't cover a term not covered by TT.
void fillPrimitiveListCombo(string TT, vector<MajorityFunction>& primitiveList, vector<MajorityFunction>& newPrimitiveCombo, int minterms){

    for(int i=0;i<primitiveList.size();i++){
        string TTp1 = primitiveList[i].getCoveredMinterms();
        for(int j=i+1;j<primitiveList.size();j++){
            string TTp2 = primitiveList[j].getCoveredMinterms();
            string newTT;
            for(int a=0;a<minterms;a++){//sums both truth tables to form newTT, the truth table of p1+p2.
                int c = 0;

                if(TTp1[a] == '1'){
                    c++;
                }
                if(TTp2[a] == '1'){
                    c++;
                }
                  switch(c){
                    case 2:
                    newTT =  newTT + '2';
                    break;
                    case 1:
                    newTT =  newTT + '1';
                    break;
                    case 0:
                    newTT =  newTT + '0';
                    break;
                }
            }
            int addFunction = 1;
            for(int k=0;k<minterms;k++){
                if(newTT[k] == '2' && TT[k] == '0'){ //if newTT covers a minterm not covered by TT, addFunction = 0  (false).
                    addFunction = 0;
                }
                if(newTT[k] == '0' && TT[k] == '1'){ //if newTT doesn't cover every TT minterm at least once, addFunction = 0  (false).
                    addFunction = 0;
                }
            }

            if(addFunction == 1){ //if addFunction = true, calculate parameters and add a new function to newPrimitiveCombo;
                int mGates = primitiveList[i].getGates() + primitiveList[j].getGates();
                int mLiterals = primitiveList[i].getLiterals() + primitiveList[j].getLiterals();
                int mInverters = primitiveList[i].getInverters() + primitiveList[j].getInverters();
                string M = primitiveList[i].getPrimitiveM() + "," + primitiveList[j].getPrimitiveM();
                int complementedRoot = primitiveList[i].getRootInverted() + primitiveList[j].getRootInverted();

                MajorityFunction newFunction(M,newTT,mGates,mInverters,mLiterals,complementedRoot,i,j); //adds the function in the primitiveCombo table (P).
                newPrimitiveCombo.push_back(newFunction);
            }
        }
    }

    sort(newPrimitiveCombo.begin(), newPrimitiveCombo.end(), sortByCost());

    //Deletes repeated functions, leaving only the lowest cost cover for every function in P.
    for(int i=0;i<newPrimitiveCombo.size();i++){
        for(int j=i+1;j<newPrimitiveCombo.size();j++){
            if(newPrimitiveCombo[i].getCoveredMinterms() == newPrimitiveCombo[j].getCoveredMinterms()){
                newPrimitiveCombo.erase(newPrimitiveCombo.begin() + j);
                j--;
            }
        }
    }
    for(int i=0;i<newPrimitiveCombo.size();i++){
        for(int j=i+1;j<newPrimitiveCombo.size();j++){
            if(newPrimitiveCombo[i].getCoveredMinterms() == newPrimitiveCombo[j].getCoveredMinterms()){
                newPrimitiveCombo.erase(newPrimitiveCombo.begin() + j);
                j--;
            }
        }
    }
}

//Updates the cost of the functions in the MajorityFunction vector based on repeated gates (for the 1º Loop);
//The algorith searchs the gates in X1 and X2 for every X3 possibilities and updates the possibilitie cost if a repetition is found.
void updateCostLoop1(int f1, vector<MajorityFunction>& possibilitiesFn, vector<MajorityFunction>& primitiveList, vector<MajorityFunction>& primitiveCombo){

      if(primitiveCombo[f1].getGates() >= 1){
          int p1 = primitiveCombo[f1].getF1();
          int p2 = primitiveCombo[f1].getF2();

          for(int i=0;i<possibilitiesFn.size();i++){
            string selectedFunction = possibilitiesFn[i].getPrimitiveM();
            if(primitiveList[p1].getGates() == 1){
                string sp1 = primitiveList[p1].getPrimitiveM();
                if(primitiveList[p1].getRootInverted() == 0){//M(A,B,C)
                    size_t found;
                    sp1[0] = 'x';
                    sp1.erase(std::remove(sp1.begin(), sp1.end(), 'x'), sp1.end());
                    found = selectedFunction.find(sp1);

                    if(found != string::npos){
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters(possibilitiesFn[i].getInverters() - primitiveList[p1].getInverters());
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p1].getLiterals());
                    }
                }else{//M'(A,B,C)
                    size_t found;
                    found = selectedFunction.find(sp1);
                    if(found != string::npos){
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters(possibilitiesFn[i].getInverters() - primitiveList[p1].getInverters());
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p1].getLiterals());
                    }
                    sp1[1] = 'x';
                    sp1.erase(std::remove(sp1.begin(), sp1.end(), 'x'), sp1.end());

                    found = selectedFunction.find(sp1);
                    if(found != string::npos){
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters((possibilitiesFn[i].getInverters() - primitiveList[p1].getInverters())+1);
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p1].getLiterals());
                    }
                }
            }
            if(primitiveList[p2].getGates() == 1){
                string sp2 = primitiveList[p2].getPrimitiveM();
                if(primitiveList[p2].getRootInverted() == 0){//M(A,B,C)
                    size_t found;
                    sp2[0] = 'x';
                    sp2.erase(std::remove(sp2.begin(), sp2.end(), 'x'), sp2.end());
                    found = selectedFunction.find(sp2);

                    if(found != string::npos){//procura sem negação na raiz
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters(possibilitiesFn[i].getInverters() - primitiveList[p2].getInverters());
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p2].getLiterals());
                    }
                }else{//SE M'(A,B,C)
                    size_t found;
                    found = selectedFunction.find(sp2);
                    if(found != string::npos){ //procura normal
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters(possibilitiesFn[i].getInverters() - primitiveList[p2].getInverters());
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p2].getLiterals());
                    }
                    sp2[1] = 'x';
                    sp2.erase(std::remove(sp2.begin(), sp2.end(), 'x'), sp2.end());

                    found = selectedFunction.find(sp2);
                    if(found != string::npos){//procura sem negação na raiz
                        possibilitiesFn[i].setGates(possibilitiesFn[i].getGates() - 1);
                        possibilitiesFn[i].setInverters((possibilitiesFn[i].getInverters() - primitiveList[p2].getInverters())+1);
                        possibilitiesFn[i].setLiterals(possibilitiesFn[i].getLiterals() - primitiveList[p2].getLiterals());
                    }
                }
            }
        }
    }
}

//Updates the cost of the functions in the MajorityFunction vector based on repeated gates (for the 2º Loop)
//The algorithm receives a string with the gates we already have in M(X1,X2,X3) and verifies repeated gates in a vector of possibilities for Xc
vector<string> updateCost(string selectedFunction, vector<MajorityFunction>& possibilitiesFn, vector<string> stringPiecesOld, int aux, int selectedFunctionGates){

    vector<string> stringPiecesNew = stringPiecesOld;

    int possibilitiesFnSize = possibilitiesFn.size();
    int selectedFunctionSize = selectedFunction.size();
    size_t foundStart = 0;

    foundStart = selectedFunction.find('(');
    selectedFunction[foundStart] = 'x';
    for(int i=0;i<foundStart;i++){
        selectedFunction[i] = 'x';
    }

    if(aux == 1){
        selectedFunction[selectedFunctionSize-1] = 'x';
    }

    for(int i=foundStart;i<selectedFunctionSize;i++){
        if(selectedFunction[i] == 'M'){
            selectedFunction[i] = 'x';
            if(selectedFunction[i+1] == '\''){
                selectedFunction[i+1] = 'x';
                i++;
            }
        }
    }

    selectedFunction.erase(std::remove(selectedFunction.begin(), selectedFunction.end(), 'x'), selectedFunction.end());

    int auxGates = selectedFunctionGates-1;

    for(int i=0;i<auxGates;i++){
        int Start;
        int End;
        string aux;
        for(int x=0;x<selectedFunction.size();x++){
            if(selectedFunction[x] == '('){
                Start = x;
                break;
            }
        }

        for(int x=Start;x<selectedFunction.size();x++){
            aux = aux + selectedFunction[x];
            if(selectedFunction[x] == ')'){
                End = x;
                break;
            }
        }

        stringPiecesNew.push_back(aux);

        selectedFunction[End] = 'x';
        selectedFunction[Start] = 'x';
    }

    for(int i=0;i<stringPiecesNew.size();i++){
        int inverters = 0;
        int literals = 0;
        string auxPieces = stringPiecesNew[i];

        for(int x=0;x<auxPieces.size();x++){
            if(auxPieces[x] == '\''){
                inverters = inverters + 1;
            }
            if(auxPieces[x] == 'A' || auxPieces[x] == 'B' || auxPieces[x] == 'C' || auxPieces[x] == 'D'){
                literals = literals + 1;
            }
        }

        for(int j=0;j<stringPiecesNew.size();j++){
            for(int k=j+1;k<stringPiecesNew.size();k++){
                if(stringPiecesNew[j] == stringPiecesNew[k]){
                    stringPiecesNew.erase(stringPiecesNew.begin()+k);
                    k--;
                }
            }
        }

        for(int j=0;j<possibilitiesFnSize;j++){
            string possibilitiesFnFunction = possibilitiesFn[j].getPrimitiveM();
            size_t found = possibilitiesFnFunction.find(stringPiecesNew[i]);

            if(found != string::npos){
                possibilitiesFn[j].setGates(possibilitiesFn[j].getGates() - 1);
                possibilitiesFn[j].setInverters(possibilitiesFn[j].getInverters() - inverters);
                possibilitiesFn[j].setLiterals(possibilitiesFn[j].getLiterals() - literals);
            }
        }
    }
    return stringPiecesNew;
}

//Builds the list P of valid pairs p1 + p2, selecting p1 as a primitive and p2 from the M2 table;
//Only pairs that respect the following conditions are allowed in table P:
//  -The pair of functions p1+p2 has to cover all minterms in TT at least once;
//  -The pair of functions p1+p2 can't cover a term not covered by TT.
void fillPrimitiveListM2Combo(string TT, vector<MajorityFunction>& primitiveList, vector<MajorityFunction>& M2Table, vector<MajorityFunction>& newPrimitiveCombo, int minterms){

    for(int i=0;i<primitiveList.size();i++){
        string TTp1 = primitiveList[i].getCoveredMinterms();
        for(int j=0;j<M2Table.size();j++){
            string TTp2 = M2Table[j].getCoveredMinterms();
            string newTT;
            for(int a=0;a<minterms;a++){//sums both truth tables to form newTT, the truth table of p1+p2.
                int c = 0;

                if(TTp1[a] == '1'){
                    c++;
                }
                if(TTp2[a] == '1'){
                    c++;
                }
                  switch(c){
                    case 2:
                    newTT =  newTT + '2';
                    break;
                    case 1:
                    newTT =  newTT + '1';
                    break;
                    case 0:
                    newTT =  newTT + '0';
                    break;
                }
            }
            int addFunction = 1;
            for(int k=0;k<minterms;k++){
                if(newTT[k] == '2' && TT[k] == '0'){ //if newTT covers a minterm not covered by TT, addFunction = 0 (false).
                    addFunction = 0;
                }
                if(newTT[k] == '0' && TT[k] == '1'){ //if newTT doesn't cover every TT minterm at least once, addFunction = 0 (false).
                    addFunction = 0;
                }
            }

             if(addFunction == 1){//if addFunction = true, calculate parameters and add a new function to newPrimitiveCombo.
                int mGates = primitiveList[i].getGates() + M2Table[j].getGates();
                int mLiterals = primitiveList[i].getLiterals() + M2Table[j].getLiterals();
                int mInverters = primitiveList[i].getInverters() + M2Table[j].getInverters();
                string M = primitiveList[i].getPrimitiveM() + "," + M2Table[j].getPrimitiveM();
                int complementedRoot = primitiveList[i].getRootInverted() + M2Table[j].getRootInverted();
                //Searchs for repeated X1 gates in X2 -- START{
                if(primitiveList[i].getGates() == 1){
                    size_t found;
                    string sp1 = primitiveList[i].getPrimitiveM();
                    string sp2 = M2Table[j].getPrimitiveM();
                    if(primitiveList[i].getRootInverted() == 0){//If M(A,B,C)
                        sp1[0] = 'x';
                        sp1.erase(std::remove(sp1.begin(), sp1.end(), 'x'), sp1.end());

                        found = sp2.find(sp1);

                        if(found != string::npos){
                            mGates = mGates - 1;
                            mLiterals = mLiterals - primitiveList[i].getLiterals();
                            mInverters = mInverters - primitiveList[i].getInverters();
                         }
                    }else{//If M'(A,B,C)
                        found = sp2.find(sp1);
                        if(found != string::npos){
                           mGates = mGates - 1;
                           mInverters = mInverters - primitiveList[i].getInverters();
                           mLiterals = mLiterals - primitiveList[i].getLiterals();
                        }
                        sp1[1] = 'x';
                        sp1.erase(std::remove(sp1.begin(), sp1.end(), 'x'), sp1.end());

                        found = sp2.find(sp1);
                        if(found != string::npos){
                            mGates = mGates - 1;
                            mInverters = (mInverters - primitiveList[i].getInverters()) + 1;
                            mLiterals = mLiterals - primitiveList[i].getLiterals();
                        }
                    }
                }
                //Searchs for repeated X1 gates in X2 -- END}
                //Adds valid function found in the newPrimitiveCombo vector (Table P).
                MajorityFunction newFunction(M,newTT,mGates,mInverters,mLiterals,complementedRoot,i,j);
                newPrimitiveCombo.push_back(newFunction);
            }
        }
    }

    sort(newPrimitiveCombo.begin(), newPrimitiveCombo.end(), sortByCost());

    //Deletes repeated functions, leaving only the lowest cost cover for every function in P.
    for(int i=0;i<newPrimitiveCombo.size();i++){
        for(int j=i+1;j<newPrimitiveCombo.size();j++){
            if(newPrimitiveCombo[i].getCoveredMinterms() == newPrimitiveCombo[j].getCoveredMinterms()){
                newPrimitiveCombo.erase(newPrimitiveCombo.begin() + j);
                j--;
            }
        }
    }
    for(int i=0;i<newPrimitiveCombo.size();i++){
        for(int j=i+1;j<newPrimitiveCombo.size();j++){
            if(newPrimitiveCombo[i].getCoveredMinterms() == newPrimitiveCombo[j].getCoveredMinterms()){
                newPrimitiveCombo.erase(newPrimitiveCombo.begin() + j);
                j--;
            }
        }
    }
}

//finds X3 for n=4
void findX3(int combo, string TT, string comboTT, int minterms, vector<MajorityFunction>& M2Table, vector<MajorityFunction>& primitiveCombo, vector<MajorityFunction>& newPossibilities, vector<MajorityFunction>& primitiveList, int aux){
    string f3TT;
    vector<int> dontCarePositions;

    for(int i=0;i<minterms;i++){//Generates X3f based on X1 and X3
        if(comboTT[i] == '1'){
            if(TT[i] == '0'){
                f3TT = f3TT + '0';
            }
            if(TT[i] == '1'){
                f3TT = f3TT + '1';
            }
        }
        if(comboTT[i] == '2' || comboTT[i] == '0'){
            f3TT = f3TT + '0';
            dontCarePositions.push_back(i);
        }
    }

    int N = dontCarePositions.size();

    //Generation of all possible f3 -- START{
    vector<string> possibilitiesF3TT;
    possibilitiesF3TT.push_back(f3TT); //All don'tcares = 0

    for(int K=1;K<=N;K++){
        string bitmask(K, 1); // K leading 1's (K is the amount of dont cares combined at the moment, using N!/(K!*(N-K)!), from 1 to N
        bitmask.resize(N, 0); // N-K trailing 0's (N is the total of don't care states)
        do {
            string auxf3TT = f3TT; //Keeps N "clean" with 0's
            for (int i=0;i<N;++i){ // [0..N-1] integers
                if (bitmask[i]){
                    int index = dontCarePositions[i]; //receives the position that has to be valued as '1'
                    auxf3TT[index] = '1';
                }
            }
            possibilitiesF3TT.push_back(auxf3TT); //Adds the function in the vector;
        }while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    }
    //Generation of all possible f3 -- END}

    vector<string>::iterator it;
    vector<MajorityFunction> possibilitiesF3;

    if(!possibilitiesF3TT.empty()){
        int counter = 0;
        for(int i=0;i<M2Table.size();i++){//Searchs every possibilitie of X3 in the M2 table and stores the functions found.
            it = find(possibilitiesF3TT.begin(), possibilitiesF3TT.end(), M2Table[i].getCoveredMinterms());
            if(it != possibilitiesF3TT.end()){
                MajorityFunction newFunction(M2Table[i].getPrimitiveM(),M2Table[i].getCoveredMinterms(),M2Table[i].getGates(),M2Table[i].getInverters(),M2Table[i].getLiterals(),M2Table[i].getRootInverted(),0,0);
                possibilitiesF3.push_back(newFunction);
            }
            if(counter >= possibilitiesF3TT.size()){
                break;
            }
        }
    }

    if(!possibilitiesF3.empty()){//If possibilities not empty we have X3.
        int f1 = primitiveCombo[combo].getF1();
        int f2 = primitiveCombo[combo].getF2();
        int countRootInverters;
        string X1 = primitiveList[f1].getPrimitiveM();
        string X2,X3,M;
        vector<string> stringPieces;

        if(aux == 1){ //If Loop1
            updateCostLoop1(combo,possibilitiesF3,primitiveList,primitiveCombo);
            X2 = primitiveList[f2].getPrimitiveM();
        }else if(aux == 2){ //If Loop2
            stringPieces = updateCost(primitiveCombo[combo].getPrimitiveM(), possibilitiesF3, stringPieces, 0, primitiveCombo[combo].getGates());
            X2 = M2Table[f2].getPrimitiveM();
        }

        sort(possibilitiesF3.begin(), possibilitiesF3.end(), sortByCost());//Sort to get the lowest cost X3.

        if(aux == 1){ //If Loop1
            countRootInverters = primitiveList[f1].getRootInverted() + primitiveList[f2].getRootInverted() + possibilitiesF3[0].getRootInverted();
        }else if(aux == 2){ //If Loop2
            countRootInverters = primitiveList[f1].getRootInverted() + M2Table[f2].getRootInverted() + possibilitiesF3[0].getRootInverted();
        }

        X3 = possibilitiesF3[0].getPrimitiveM();

        //Inverters minimization -- START{
        int IsConstant = 0;

        if(countRootInverters == 3){
            X1[1] = 'x';
            X2[1] = 'x';
            X3[1] = 'x';
            M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
        }else{
            M = "M(" + X1 + "," + X2 + "," + X3 + ")";
        }

        if(countRootInverters == 2){
            if(X1 == "0"){
                X1 = "1";
                X2[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }else if(X1 == "1"){
                X1 = "0";
                X2[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }
            if(X2 == "0"){
                X1[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }else if(X2 == "1"){
                X2 = "0";
                X1[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }
            if(X3 == "0"){
                X3 = "1";
                X1[1] = 'x';
                X2[1] = 'x';
                IsConstant = 1;
            }else if(X3 == "1"){
                X3 = "0";
                X1[1] = 'x';
                X2[1] = 'x';
                IsConstant = 1;
            }
        }

        if(IsConstant == 1){
            M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
        }

        M.erase(std::remove(M.begin(), M.end(), 'x'), M.end());
        //Inverters minimization -- END}

        //Calc inverters
        int mInverters = 0;
        for(int x=0;x<M.size();x++){
            if(M[x] == '\''){
                mInverters++;
            }
        }
        int mGates = 1 + primitiveCombo[combo].getGates() + possibilitiesF3[0].getGates();
        int mLiterals = primitiveCombo[combo].getLiterals() + possibilitiesF3[0].getLiterals();
        MajorityFunction newFunction(M,TT,mGates,mInverters,mLiterals,0,0,0);
        newPossibilities.push_back(newFunction);//Adds M(X1,X2,X3) in Z.
        }
    }

//finds X3 for n=5
void findX3N5(int combo, string TT, string comboTT, vector<MajorityFunction>& M2Table, vector<MajorityFunction>& primitiveCombo, vector<MajorityFunction>& primitiveList, vector<MajorityFunction>& newPossibilities){

   for(int i=0;i<M2Table.size();i++){//Searchs X3 in M2.
      int valid = 1;
      string sf3 = M2Table[i].getCoveredMinterms();

      for(int j=0;j<32;j++){
        if(sf3[j] != '1' && comboTT[j] == '1' && TT[j] == '1'){
            valid = 0;
            break;
        }
        if(sf3[j] == '1' && comboTT[j] == '1' && TT[j] == '0'){
            valid = 0;
            break;
        }
     }

     if(valid == 1){//If a valid X3 was found, we have M(X1,X2,X3)
        int f1 = primitiveCombo[combo].getF1();
        int f2 = primitiveCombo[combo].getF2();
        string X1 = primitiveList[f1].getPrimitiveM();
        string X2 = primitiveList[f2].getPrimitiveM();
        int countRootInverters = primitiveList[f1].getRootInverted() + primitiveList[f2].getRootInverted() + M2Table[i].getRootInverted();
        string X3 = M2Table[i].getPrimitiveM();
        int IsConstant = 0;
        string M = "M(" + X1 + "," + X2 + "," + X3 + ")";

        //Inverters minimization -- START{
        if(countRootInverters == 3){
            X1[1] = 'x';
            X2[1] = 'x';
            X3[1] = 'x';
            M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
        }else{
            M = "M(" + X1 + "," + X2 + "," + X3 + ")";
        }

        if(countRootInverters == 2){
            if(X1 == "0"){
                X1 = "1";
                X2[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }else if(X1 == "1"){
                X1 = "0";
                X2[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }
            if(X2 == "0"){
                X1[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }else if(X2 == "1"){
                X2 = "0";
                X1[1] = 'x';
                X3[1] = 'x';
                IsConstant = 1;
            }
            if(X3 == "0"){
                X3 = "1";
                X1[1] = 'x';
                X2[1] = 'x';
                IsConstant = 1;
            }else if(X3 == "1"){
                X3 = "0";
                X1[1] = 'x';
                X2[1] = 'x';
                IsConstant = 1;
            }
        }

        if(IsConstant == 1){
            M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
        }
        M.erase(std::remove(M.begin(), M.end(), 'x'), M.end());
        //Inverters minimization -- END}

        int mInverters = 0;
        for(int x=0;x<M.size();x++){
            if(M[x] == '\''){
                mInverters++;
            }
        }
        int mGates = 1 + primitiveCombo[combo].getGates() + M2Table[i].getGates();
        int mLiterals = primitiveCombo[combo].getLiterals() + M2Table[i].getLiterals();
        MajorityFunction newFunction(M,TT,mGates,mInverters,mLiterals,0,0,0);
        newPossibilities.push_back(newFunction); //Defines M(X1,X2,X3) as the output function.
        i = M2Table.size();
    }
   }
}

//finds X2 and X3 for n=4 and n=5
int Loop2(int minterms, clock_t startTime, int f1, string sf1, string TT, vector<MajorityFunction>& M2Table, vector<MajorityFunction>& primitiveList, int aux){

    //Updates v for X2
    int M2TableSize = M2Table.size();
    int v[minterms];

    for(int i=0;i<minterms;i++){
        v[i] = 0;
    }

    for(int i=0;i<TT.size();i++){
        if(sf1[i] == '1' && TT[i] == '1'){
            v[i] = 1;
        }else if(sf1[i] == '1' && TT[i] == '0'){
            v[i] = 0-1;
        }
    }

    for(int i=0;i<M2Table.size();i++){//Searchs for a valid X2 in M2.
        int valid = 1;
        string sf2 = M2Table[i].getCoveredMinterms();

        for(int j=0;j<TT.size();j++){
            if(sf2[j] == '1' && v[j] < 0){
                valid = 0;
            }
            if(sf2[j] != '1' && (v[j] == 0 && TT[j] == '1')){
                valid = 0;
            }
        }
        if(valid == 1){//If found a valid X2 then:

            //Updates v for X3.
            int auxV[minterms];
            for(int a=0;a<minterms;a++){
               auxV[a] = v[a];
            }
            for(int j=0;j<TT.size();j++){
                if(sf2[j] == '1' && TT[j] == '1'){
                    auxV[j] = auxV[j] + 1;
                }else if(sf2[j] == '1' && TT[j] == '0'){
                    auxV[j] = 0-1;
                }
            }

            for(int k=0;k<M2Table.size();k++){//Searchs for a valid X3 in M2.
                int validf3 = 1;
                string sf3 = M2Table[k].getCoveredMinterms();
                for(int j=0;j<TT.size();j++){
                    if(sf3[j] == '1' && auxV[j] < 0){
                        validf3 = 0;
                    }
                    if(sf3[j] != '1' && (auxV[j] == 1 && TT[j] == '1')){
                        validf3 = 0;
                    }
                }
                if(validf3 == 1){//If a valid X3 is found then:
                    string M, X1;
                    int countRootInverters, mGates, mLiterals;
                    if(aux==1){
                        mGates = 1 + M2Table[f1].getGates() + M2Table[i].getGates() + M2Table[k].getGates();
                        mLiterals = M2Table[f1].getLiterals() + M2Table[i].getLiterals() + M2Table[k].getLiterals();
                        M = "M(" + M2Table[f1].getPrimitiveM() + "," + M2Table[i].getPrimitiveM() + "," + M2Table[k].getPrimitiveM() + ")";
                        X1 = M2Table[f1].getPrimitiveM();
                        countRootInverters = M2Table[f1].getRootInverted() + M2Table[i].getRootInverted() + M2Table[k].getRootInverted();
                    }else{
                        mGates = 1 + primitiveList[f1].getGates() + M2Table[i].getGates() + M2Table[k].getGates();
                        mLiterals = primitiveList[f1].getLiterals() + M2Table[i].getLiterals() + M2Table[k].getLiterals();
                        M = "M(" + primitiveList[f1].getPrimitiveM() + "," + M2Table[i].getPrimitiveM() + "," + M2Table[k].getPrimitiveM() + ")";
                        X1 = primitiveList[f1].getPrimitiveM();
                        countRootInverters = primitiveList[f1].getRootInverted() + M2Table[i].getRootInverted() + M2Table[k].getRootInverted();
                    }
                    string X2 = M2Table[i].getPrimitiveM();
                    string X3 = M2Table[k].getPrimitiveM();
                    int IsConstant = 0;
                    //Invertes minimization -- START {
                    if(countRootInverters == 3){
                        X1[1] = 'x';
                        X2[1] = 'x';
                        X3[1] = 'x';
                        M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
                    }else{
                        M = "M(" + X1 + "," + X2 + "," + X3 + ")";
                    }

                    if(countRootInverters == 2){
                        if(X1 == "0"){
                            X1 = "1";
                            X2[1] = 'x';
                            X3[1] = 'x';
                            IsConstant = 1;
                        }else if(X1 == "1"){
                            X1 = "0";
                            X2[1] = 'x';
                            X3[1] = 'x';
                            IsConstant = 1;
                        }
                        if(X2 == "0"){
                            X1[1] = 'x';
                            X3[1] = 'x';
                            IsConstant = 1;
                        }else if(X2 == "1"){
                            X2 = "0";
                            X1[1] = 'x';
                            X3[1] = 'x';
                            IsConstant = 1;
                        }
                        if(X3 == "0"){
                            X3 = "1";
                            X1[1] = 'x';
                            X2[1] = 'x';
                            IsConstant = 1;
                        }else if(X3 == "1"){
                            X3 = "0";
                            X1[1] = 'x';
                            X2[1] = 'x';
                            IsConstant = 1;
                        }
                    }

                    if(IsConstant == 1){
                        M = "M'(" + X1 + "," + X2 + "," + X3 + ")";
                    }

                    M.erase(std::remove(M.begin(), M.end(), 'x'), M.end());
                    //Inverters minimization -- END}

                    int mInverters = 0;
                    for(int x=0;x<M.size();x++){
                        if(M[x] == '\''){
                            mInverters++;
                        }
                    }
                    //system("cls");
                    cout << "Y" << endl;
                    cout << "Input Table: " << endl;
                    cout << TT << endl;
                    cout << "Majority Function: " << endl;
                    cout << M << endl;
                    cout << "Gates: " << mGates << " Inverters: " << mInverters << " Literals: " << mLiterals << endl;
                    cout << "Runtime: " << endl;
                    double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
                    cout << time << " seconds." << endl;
                    PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
                    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
                    double i = (info.PeakWorkingSetSize);
                    //covert bytes to KB
                    i = (i/1024);
                    //convert KB to MB
                    i = (i/1024);
                    cout << "Memory: " << endl;
                    cout << i << " MB" << endl;
                    cout << endl;
                    string aswer;
                    do{
                        cout << "Save results in a file? (Type y for Yes/Type n for No)" << endl;
                        cin >> aswer;
                        if(aswer == "y"){
                            string outputPath = TT + ".txt";
                            ofstream outputFile(outputPath.c_str());
                            outputFile << TT << "\n";
                            outputFile << M << "\n";
                            outputFile << mGates << "\n";
                            outputFile << mInverters << "\n";
                            outputFile << mLiterals << "\n";
                            outputFile << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds" << "\n";
                            PROCESS_MEMORY_COUNTERS info;
                            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
                            double i = (info.PeakWorkingSetSize);
                            //covert bytes to KB
                            i = (i/1024);
                            //convert KB to MB
                            i = (i/1024);
                            outputFile << i << " MB" << "\n";

                            system("cls");
                            cout << "File saved in mpc's directory." << endl;
                            cout << endl;
                        }else{
                            system("cls");
                        }
                    }while(aswer != "y" && aswer != "n");

                    return 1;
                }
            }
        }
        if(i == M2TableSize - 1){
            return 0;
        }
    }
}

//Writes the output function in a file saved in the same directory of the algorithm
void saveInFile(string TT, int index, vector<MajorityFunction>& possibilities, double time){

    string aswer;
    do{
        cout << "Save results in a file? (Type y for Yes/Type n for No)" << endl;
        cin >> aswer;
        if(aswer == "y"){
            string outputPath = TT + ".txt";

            ofstream outputFile(outputPath.c_str());

            outputFile << possibilities[index].getCoveredMinterms() << "\n";
            outputFile << possibilities[index].getPrimitiveM() << "\n";
            outputFile << possibilities[index].getGates() << "\n";
            outputFile << possibilities[index].getInverters() << "\n";
            outputFile << possibilities[index].getLiterals() << "\n";
            outputFile << time << " seconds" << "\n"; //Calculates the runtime.
            PROCESS_MEMORY_COUNTERS info;
            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)); //Gets the used memory of the current process, in bytes.
            double i = (info.PeakWorkingSetSize);
            //covert bytes to KB
            i = (i/1024);
            //convert KB to MB
            i = (i/1024);
            outputFile << i << " MB" << "\n";

            system("cls");
            cout << "File saved in mpc's directory." << endl;
        }else{
            system("cls");
        }
    }while(aswer != "y" && aswer != "n");
}

int main(){

    int start = 1;
    while(start == 1){
        string TT;
        bool validTT = false;
        int n = 0;

        //Input Data Validation
        while(validTT == false){
            cout << "Type a input truth table(3<=n<=5) in binary or hexadecimal format: " << endl;
            cout << "Example: 01001111 or 4F." << endl;
            cin >> TT;
            bool isBinary = true;

            system("cls");

            //verifies if TT is in binary format.
            for(int i=0;i<TT.size();i++){
                if(TT[i] != '1' && TT[i] != '0'){
                    isBinary = false;
                    break;
                }
            }

            //verifies if TT has a valid binary size
            if(isBinary == true && TT.size() != 16 && TT.size() != 8 && TT.size() != 32){
                cout << "Error: Invalid binary size." << endl;
                cout << endl;
                validTT = false;
            }else{
                validTT = true;
            }

            //If not binary
            if(isBinary == false){
                bool isHexa = true;
                //Checks if is in hexadecimal format
                for(int i=0;i<TT.size();i++){
                    if(!isxdigit(TT[i])){
                        isHexa = false;
                    }
                }
                if(isHexa == false){
                    cout << "Error: Invalid format." << endl;
                    cout << endl;
                    validTT = false;
                }else{
                    if(TT.size() != 2 && TT.size() != 4 && TT.size() != 8){
                        cout << "Error: Invalid hexadecimal size." << endl;
                        cout << endl;
                        validTT = false;
                    }else{
                        //Convert hex TT to binary TT
                        stringstream ss;
                        ss << hex << TT;
                        unsigned x;
                        ss >> x;
                        if(TT.size() == 2){//If size=2, TT has 8 terms
                            bitset<8> b(x);
                            TT = b.to_string();
                        }else if(TT.size() == 4){//If size=4, TT has 16 terms
                            bitset<16> b(x);
                            TT = b.to_string();
                        }else if(TT.size() == 8){//If size=8, TT has 32 terms
                            bitset<32> b(x);
                            TT = b.to_string();
                        }
                        validTT = true;
                    }
                }
            }
        }

        int minterms = TT.size();

        //Defines the number of inputs (n) based in the number of terms.
        switch(minterms){
           case 32:
           n = 5;
           break;
           case 16:
           n = 4;
           break;
           case 8:
           n = 3;
           break;
        }

        //Run Time Calculation
        clock_t startTime = clock();

        int D4N4 = verifyN4D4(TT, startTime);

        if(D4N4 == 0){
            vector<MajorityFunction> primitiveList;

            fillPrimitiveList(primitiveList, n, minterms); //Fills the primitive table, with optimized 1Lvl functions;

            vector<MajorityFunction>::iterator it;
            int index;

            it = find_if(primitiveList.begin(), primitiveList.end(), findFunction(TT));

            system("cls");
            cout << "Please, wait while the function is being built..." << endl;

            if(it != primitiveList.end()){//if covered by primitiveTable = true
                system("cls");
                index = distance(primitiveList.begin(), it);
                cout << "Input Table: " << endl;
                cout << primitiveList[index].getCoveredMinterms() << endl;
                cout << "Majority Function: " << endl;
                cout << primitiveList[index].getPrimitiveM() << endl;
                cout << "Gates: " << primitiveList[index].getGates() << " Inverters: " << primitiveList[index].getInverters() << " Literals: " << primitiveList[index].getLiterals() << endl;
                cout << "Runtime: " << endl;
                double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
                cout << time << " seconds." << endl;
                PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
                GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
                double i = (info.PeakWorkingSetSize);
                //covert bytes to KB
                i = (i/1024);
                //convert KB to MB
                i = (i/1024);
                cout << "Memory: " << endl;
                cout << i << " MB" << endl;
                cout << endl;
                string aswer;
                saveInFile(TT,index,primitiveList,time);
            }else{
                vector<MajorityFunction> M2LvlTable;

                fill2LvlTable(n,M2LvlTable);

                it = find_if(M2LvlTable.begin(), M2LvlTable.end(), findFunction(TT));

                if(it != M2LvlTable.end()){
                    system("cls");
                    index = distance(M2LvlTable.begin(), it);
                    cout << "Input Table: " << endl;
                    cout << M2LvlTable[index].getCoveredMinterms() << endl;
                    cout << "Majority Function: " << endl;
                    cout << M2LvlTable[index].getPrimitiveM() << endl;
                    cout << "Gates: " << M2LvlTable[index].getGates() << " Inverters: " << M2LvlTable[index].getInverters() << " Literals: " << M2LvlTable[index].getLiterals() << endl;
                    cout << "Runtime: " << endl;
                    double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
                    cout << time << " seconds." << endl;
                    PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
                    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
                    double i = (info.PeakWorkingSetSize);
                    //covert bytes to KB
                    i = (i/1024);
                    //convert KB to MB
                    i = (i/1024);
                    cout << "Memory: " << endl;
                    cout << i << " MB" << endl;
                    cout << endl;
                    saveInFile(TT,index,M2LvlTable,time);
                }else{
                    //if couldn't be covered by a single M2 function
                    vector<MajorityFunction> primitiveCombo;
                    fillPrimitiveListCombo(TT, primitiveList, primitiveCombo, minterms);

                    //LOOP 1 -> START{ M(PL,PL,M2)
                    vector<MajorityFunction> possibilitiesLoop1;
                    //Finds every valid output for M(X1,X2,X3), using 2 primitives and one M2 function.
                    if(n == 4){
                        for(int i=0;i<primitiveCombo.size();i++){
                            findX3(i, TT, primitiveCombo[i].getCoveredMinterms(), minterms, M2LvlTable, primitiveCombo, possibilitiesLoop1, primitiveList, 1);
                        }
                    }else if(n == 5){
                        for(int i=0;i<primitiveCombo.size();i++){
                            findX3N5(i, TT, primitiveCombo[i].getCoveredMinterms(), M2LvlTable, primitiveCombo, primitiveList, possibilitiesLoop1);
                        }
                    }

                    if(!possibilitiesLoop1.empty()){
                        if(possibilitiesLoop1.size() > 1){
                            //Returns the lowest cost valid M(X1,X2,X3)
                            sort(possibilitiesLoop1.begin(),possibilitiesLoop1.end(),sortByCost());
                        }
                        system("cls");
                        cout << "Input Table: " << endl;
                        cout << possibilitiesLoop1[0].getCoveredMinterms() << endl;
                        cout << "Majority Function: " << endl;
                        cout << possibilitiesLoop1[0].getPrimitiveM() << endl;
                        cout << "Gates: " << possibilitiesLoop1[0].getGates() << " Inverters: " << possibilitiesLoop1[0].getInverters() << " Literals: " << possibilitiesLoop1[0].getLiterals() << endl;
                        cout << "Runtime: " << endl;
                        double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
                        cout << time << " seconds." << endl;
                        PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
                        GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
                        double i = (info.PeakWorkingSetSize);
                        //covert bytes to KB
                        i = (i/1024);
                        //convert KB to MB
                        i = (i/1024);
                        cout << "Memory: " << endl;
                        cout << i << " MB" << endl;
                        cout << endl;
                        saveInFile(TT,0,possibilitiesLoop1,time);
                    }else{
                        //LOOP 1,5 -> START{ M(PL,M2,M2)
                        //Deletes from PL the functions that doesn't cover any minterms of TT;
                        for(int i=0;i<primitiveList.size();i++){
                            string auxTT = primitiveList[i].getCoveredMinterms();
                            int coversMinterm = 0;
                            for(int j=0;j<minterms;j++){
                                if(TT[j] == '1' && auxTT[j] == '1'){
                                    coversMinterm = 1;
                                }
                            }
                            if(coversMinterm == 0){
                                primitiveList.erase(primitiveList.begin() + i);
                                i--;
                            }
                        }

                        int resultFound = 0;

                        if(n==4){
                            vector<MajorityFunction> possibilitiesLoop15;
                            vector<MajorityFunction> primitiveM2Combo;
                            fillPrimitiveListM2Combo(TT, primitiveList, M2LvlTable, primitiveM2Combo, minterms); //Fills primitives&M2 combo
                            for(int i=0;i<primitiveM2Combo.size();i++){
                                findX3(i, TT, primitiveM2Combo[i].getCoveredMinterms(), minterms, M2LvlTable, primitiveM2Combo, possibilitiesLoop15, primitiveList, 2);
                            }
                            if(!possibilitiesLoop15.empty()){
                                if(possibilitiesLoop15.size() > 1){
                                    sort(possibilitiesLoop15.begin(),possibilitiesLoop15.end(),sortByCost());
                                }
                                system("cls");
                                cout << "Input Table: " << endl;
                                cout << possibilitiesLoop15[0].getCoveredMinterms() << endl;
                                cout << "Majority Function: " << endl;
                                cout << possibilitiesLoop15[0].getPrimitiveM() << endl;
                                cout << "Gates: " << possibilitiesLoop15[0].getGates() << " Inverters: " << possibilitiesLoop15[0].getInverters() << " Literals: " << possibilitiesLoop15[0].getLiterals() << endl;
                                cout << "Runtime: " << endl;
                                double time = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
                                cout << time << " seconds." << endl;
                                PROCESS_MEMORY_COUNTERS info; //Pra essa função funcionar é necessário que se link a biblioteca psapi
                                GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));  //RightClick no projeto (MPC) > Build options > Linker settings > Add > escreve psapi > OK!!
                                double i = (info.PeakWorkingSetSize);
                                //covert bytes to KB
                                i = (i/1024);
                                //convert KB to MB
                                i = (i/1024);
                                cout << "Memory: " << endl;
                                cout << i << " MB" << endl;
                                cout << endl;
                                saveInFile(TT,0,possibilitiesLoop15,time);
                            }else{
                               //LOOP2 - START{
                                for(int i=0;i<M2LvlTable.size();i++){
                                    resultFound = Loop2(minterms, startTime, i, M2LvlTable[i].getCoveredMinterms(), TT, M2LvlTable, primitiveList, 1);
                                    if(resultFound == 1){
                                        i = M2LvlTable.size();
                                    }
                                }
                            }
                        }else if(n==5){
                            for(int i=0;i<primitiveList.size();i++){
                                resultFound = Loop2(minterms, startTime, i, primitiveList[i].getCoveredMinterms(), TT, M2LvlTable, primitiveList, 0);
                                if(resultFound == 1){
                                    i = primitiveList.size();
                                }
                            }
                        }

                        if(resultFound == 0){// needs 4lvls to be covered
                            string TT2 = TT;
                            string TT3 = TT;

                            vector<MajorityFunction> primitiveCombo2;
                            vector<MajorityFunction> primitiveCombo3;

                            vector<MajorityFunction> possibilitiesTT2;
                            vector<MajorityFunction> possibilitiesTT3;

                            if(n == 4){
                                for(int t=0;t<4;t++){
                                    TT2[t] = '1';
                                }
                                for(int t=4;t<8;t++){
                                    TT3[t] = '0';
                                }
                                for(int t=8;t<12;t++){
                                    TT3[t] = '1';
                                }
                                for(int t=12;t<16;t++){
                                    TT2[t] = '0';
                                }

                                fillPrimitiveListCombo(TT2, primitiveList, primitiveCombo2, minterms);
                                fillPrimitiveListCombo(TT3, primitiveList, primitiveCombo3, minterms);

                                for(int t=0;t<primitiveCombo2.size();t++){
                                    findX3(t, TT2, primitiveCombo2[t].getCoveredMinterms(), minterms, M2LvlTable, primitiveCombo2, possibilitiesTT2, primitiveList,1);
                                }
                                for(int t=0;t<primitiveCombo3.size();t++){
                                    findX3(t, TT3, primitiveCombo3[t].getCoveredMinterms(), minterms, M2LvlTable, primitiveCombo3, possibilitiesTT3, primitiveList,1);
                                }
                            }else if(n == 5){
                                for(int t=0;t<8;t++){
                                    TT2[t] = '1';
                                }
                                for(int t=8;t<16;t++){
                                    TT3[t] = '0';
                                }
                                for(int t=16;t<24;t++){
                                    TT3[t] = '1';
                                }
                                for(int t=24;t<32;t++){
                                    TT2[t] = '0';
                                }

                                fillPrimitiveListCombo(TT2, primitiveList, primitiveCombo2, minterms);
                                fillPrimitiveListCombo(TT3, primitiveList, primitiveCombo3, minterms);

                                for(int t=0;t<primitiveCombo2.size();t++){
                                    findX3N5(t, TT2, possibilitiesTT2[t].getCoveredMinterms(), M2LvlTable, primitiveCombo2, primitiveList, possibilitiesTT2);
                                }
                                for(int t=0;t<primitiveCombo3.size();t++){
                                    findX3N5(t, TT3, possibilitiesTT3[t].getCoveredMinterms(), M2LvlTable, primitiveCombo3, primitiveList, possibilitiesTT3);
                                }
                            }
                            sort(possibilitiesTT2.begin(),possibilitiesTT2.end(),sortByCost());
                            sort(possibilitiesTT3.begin(),possibilitiesTT3.end(),sortByCost());
                            system("cls");
                            cout << "Input Table: " << endl;
                            cout << TT << endl;
                            cout << "Majority Function: " << endl;
                            cout << "M(B," + possibilitiesTT2[0].getPrimitiveM() << "," << possibilitiesTT3[0].getPrimitiveM() << ")" << endl;
                            cout << "Runtime: " << endl;
                            cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
                            PROCESS_MEMORY_COUNTERS info;
                            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
                            double i = (info.PeakWorkingSetSize);
                            //covert bytes to KB
                            i = (i/1024);
                            //convert KB to MB
                            i = (i/1024);
                            cout << i << " MB" << endl;
                        }
                    }
                }
            }
            cout << endl;
            cout << endl;
            start = 1;
        }
    }
}


