//
//  OPVMGenerator.h
//  OPVM_Compiler
//
//  Created by Seán Hargadon on 14/12/2014.
//  Copyright (c) 2014 Seán Hargadon. All rights reserved.
//

#ifndef __OPVM_Compiler__OPVMGenerator__
#define __OPVM_Compiler__OPVMGenerator__

#include <stdio.h>

class OPVMGenerator {
private:
    
public:
    OPVMGenerator();
    
    void generateFunction();
    void generateMove();
    void generateAdd();

    
    
    
};

#endif /* defined(__OPVM_Compiler__OPVMGenerator__) */
