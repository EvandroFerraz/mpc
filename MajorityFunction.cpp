#include "MajorityFunction.h"

//This class defines the structure of a majority function, a list of the used variables can be found in the following constructor:
MajorityFunction::MajorityFunction(string newPrimitiveM, string newCoveredMinterms, int newGates, int newInverters, int newLiterals, int newRootInverted, int newF1, int newF2){
    primitiveM = newPrimitiveM;
    coveredMinterms = newCoveredMinterms;
    gates = newGates;
    inverters = newInverters;
    literals = newLiterals;
    rootInverted = newRootInverted;
    F1 = newF1;
    F2 = newF2;
}

//GETTERS
string MajorityFunction::getPrimitiveM() const {
    return primitiveM;
}

string MajorityFunction::getCoveredMinterms() const {
    return coveredMinterms;
}

int MajorityFunction::getGates() const {
    return gates;
}

int MajorityFunction::getInverters() const {
    return inverters;
}

int MajorityFunction::getLiterals() const {
    return literals;
}

int MajorityFunction::getRootInverted() const {
    return rootInverted;
}

int MajorityFunction::getF1() const {
    return F1;
}

int MajorityFunction::getF2() const {
    return F2;
}

//SETTERS
void MajorityFunction::setPrimitiveM(string newPrimitiveM){
    primitiveM = newPrimitiveM;
}

void MajorityFunction::setCoveredMinterms(string newCoveredMinterms){
    coveredMinterms = newCoveredMinterms;
}

void MajorityFunction::setGates(int newGates){
    gates = newGates;
}

void MajorityFunction::setInverters(int newInverters){
    inverters = newInverters;
}

void MajorityFunction::setLiterals(int newLiterals){
    literals = newLiterals;
}

void MajorityFunction::setRootInverted(int newRootInverted){
    rootInverted = newRootInverted;
}

void MajorityFunction::setF1(int newF1){
    F1 = newF1;
}

void MajorityFunction::setF2(int newF2){
    F2 = newF2;
}
