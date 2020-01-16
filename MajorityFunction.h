#ifndef MAJORITYFUNCTION_H_INCLUDED
#define MAJORITYFUNCTION_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Header for the MajorityFunction class
class MajorityFunction {
public:
    //Overload Constructor
    MajorityFunction(string, string, int, int, int, int, int, int);

    //Acess Functions
    string getPrimitiveM() const;

    string getCoveredMinterms() const;

    int getGates() const;

    int getInverters() const;

    int getLiterals() const;

    int getRootInverted() const;

    int getF1() const;

    int getF2() const;

    //Mutator Functions
    void setPrimitiveM(string);

    void setCoveredMinterms(string);

    void setGates(int);

    void setInverters(int);

    void setLiterals(int);

    void setRootInverted(int);

    void setF1(int);

    void setF2(int);

private: //Variables can only be assigned by the get/set functions;
    string primitiveM;
    string coveredMinterms;
    int gates;
    int inverters;
    int literals;
    int rootInverted;
    int F1;
    int F2;
};

#endif // MAJORITYFUNCTION_H_INCLUDED
