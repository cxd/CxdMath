/*
 *  IFileReader.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 6/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef INCLUDE_IREADER
#define INCLUDE_IREADER
namespace au {
namespace id {
	namespace Cxd {
		namespace IO {
		 /**
		  IReader is an interface that is used to read a source.
		  **/
		 class IReader  {
		 public:
			 virtual ~IReader() {}
			 virtual bool Read() = 0;
		 };
			
		}
	}
}

}
#endif