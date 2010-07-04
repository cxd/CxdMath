/*
 *  CxdMatrix.cp
 *  CxdMatrix
 *
 *  Created by Chris Davey on 4/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#include <iostream>
#include "CxdMatrix.h"
#include "CxdMatrixPriv.h"



void au::id::Cxd::CxdMatrix::HelloWorld(const char * s)
{
	 CxdMatrixPriv *theObj = new CxdMatrixPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void au::id::Cxd::CxdMatrixPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};
