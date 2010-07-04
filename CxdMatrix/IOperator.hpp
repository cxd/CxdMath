/*
 *  IOperator.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_IF_IOPERATOR
#define CXD_IF_IOPERATOR

namespace au {
	namespace id {
		namespace Cxd {
			namespace Operator {
			
				class IOperator  {
				public:
					
					/**
					 Perform the operation.
					 @returns
					 True if successful false otherwise.
					 **/
					virtual bool Operate() = 0;
					
				};
				
			}
		}
	}	
}
#endif

