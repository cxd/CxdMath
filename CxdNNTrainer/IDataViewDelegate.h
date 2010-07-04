//
//  IDataViewDelegate.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 9/05/10.
//  Copyright 2010 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@protocol IDataViewDelegate<NSTableViewDelegate>

-(NSInteger)numRows;

-(NSInteger)numCols;


-(BOOL)loadDataAtPath:(NSString *)dataPath;

-(BOOL)saveDataToPath:(NSString *)dataPath;

-(BOOL)displayDataSliceStartRow:(NSInteger)rowIdx startColumn:(NSInteger) colIdx endRow:(NSInteger) endRowIdx endCol:(NSInteger)endColIdx;


@end
