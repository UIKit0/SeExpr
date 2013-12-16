/*
 SEEXPR SOFTWARE
 Copyright 2011 Disney Enterprises, Inc. All rights reserved
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.
 
 * The names "Disney", "Walt Disney Pictures", "Walt Disney Animation
 Studios" or the names of its contributors may NOT be used to
 endorse or promote products derived from this software without
 specific prior written permission from Walt Disney Pictures.
 
 Disclaimer: THIS SOFTWARE IS PROVIDED BY WALT DISNEY PICTURES AND
 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND TITLE ARE DISCLAIMED.
 IN NO EVENT SHALL WALT DISNEY PICTURES, THE COPYRIGHT HOLDER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND BASED ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*/
#ifndef _SeInterpreter_h_
#define _SeInterpreter_h_

#include <vector>

class SeExprLocalVar;


//! Promotes a FP[1] to FP[d]
template<int d>
struct Promote{
    // TODO: this needs a name that is prefixed by Se!
    static int f(int* opData,double* fp,char** c)
    {
        int posIn=opData[0];
        int posOut=opData[1];
        for(int k=posOut;k<posOut+d;k++) fp[k]=fp[posIn];
        return 1;
    }
};

/// Non-LLVM manual interpreter. This is a simple computation machine. There is no stack, just fixed locations, because we have no recursion!
class SeInterpreter
{
public:
    /// Double data (constants and evaluated)
    std::vector<double> d;
    /// constant and evaluated pointer data
    std::vector<char*> s;
    /// Ooperands to op
    std::vector<int> opData;

    /// Not needed for eval only building
    typedef std::map<const SeExprLocalVar*,int> VarToLoc;
    VarToLoc varToLoc;
    
    /// Op function pointer arguments are (int* currOpData,double* currD,char** currS)
    typedef int(*OpF)(int*,double*,char**);

    std::vector<std::pair<OpF,int> > ops;

private:
    bool _startedOp;
public:

    SeInterpreter()
        :_startedOp(false)
    {}

    /// Return the position that hte next instruction will be placed at
    int nextPC(){return ops.size();}

    ///! adds an operator to the program (pointing to the data at the current location)
    int addOp(OpF op){
        if(_startedOp){
            assert(false && "addOp called within another addOp");
        }
        _startedOp=true;
        int pc=ops.size();
        ops.push_back(std::make_pair(op,opData.size()));
        return pc;
    }

    void endOp(bool execute=true)
    {
        _startedOp=false;
        if(execute){
            double* fp=&d[0];
            char** str=&s[0];
            
            int pc=ops.size()-1;
            const std::pair<OpF,int>& op=ops[pc];
            int* opCurr=&opData[0]+op.second;
            pc+=op.first(opCurr,fp,str);
        }
    }

    ///! Adds an operand. Note this should be done after doing the addOp!
    int addOperand(int param){
        assert(_startedOp);
        int ret=opData.size();
        opData.push_back(param);
        return ret;
    }

    ///! Allocate a floating point set of data of dimension n
    int allocFP(int n){
        int ret=d.size();
        for(int k=0;k<n;k++) d.push_back(0);
        return ret;
    }

    /// Allocate a pointer location (can be anything, but typically space for char*)
    int allocPtr(){
        int ret=s.size();
        s.push_back(0);
        return ret;
    }

    /// Evaluate program
    void eval();
    /// Debug by printing program
    void print();
};


//! Return the function f encapsulated in class T for the dynamic i converted to a static d.
template<template<int d> class T>
SeInterpreter::OpF getTemplatizedOp(int i)
{
    switch(i){
        case 1: return T<1>::f;
        case 2: return T<2>::f;
        case 3: return T<3>::f;
        case 4: return T<4>::f;
        case 5: return T<5>::f;
        case 6: return T<6>::f;
        case 7: return T<7>::f;
        case 8: return T<8>::f;
        case 9: return T<9>::f;
        case 10: return T<10>::f;
        case 11: return T<11>::f;
        case 12: return T<12>::f;
        case 13: return T<13>::f;
        case 14: return T<14>::f;
        case 15: return T<15>::f;
        case 16: return T<16>::f;
        default: assert(false && "Invalid dynamic parameter (not supported template)");break;
    }
    return 0;
}




#endif