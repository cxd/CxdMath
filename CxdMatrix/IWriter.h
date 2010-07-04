/*
 *  IWriter.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 6/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */
#ifndef INCLUDE_IWRITER
#define INCLUDE_IWRITER
namespace au {
	namespace id {
		namespace Cxd {
			namespace IO {
				/**
				 IWriter is an interface that is used to write a source.
				 **/
				class IWriter  {
				public:
					virtual ~IWriter() {}
					virtual bool Write() = 0;
				};
			}
		}
	}
}
#endif