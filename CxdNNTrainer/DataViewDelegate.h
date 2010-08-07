//
//  DataView.h
//  CxdNNTrainer
//
//  Created by Chris Davey on 8/05/10.
//  Copyright 2010 none. All rights reserved.
//

#ifdef __cplusplus
#include "CxdMath.h"
#endif

#import <Cocoa/Cocoa.h>
#import "IDataViewDelegate.h"

@interface DataViewDelegate : NSObject<NSTableViewDataSource, NSTableViewDelegate, IDataViewDelegate> {
	
	NSString *resourcePath;

#ifdef __cplusplus
	arma::mat matrix;
#endif
}

@property(retain) NSString *resourcePath;

-(NSInteger)numRows;

-(NSInteger)numCols;

#ifdef __cplusplus
-(arma::mat) getMatrix ;
#endif

-(BOOL)loadDataAtPath:(NSString *)dataPath;

-(BOOL)saveDataToPath:(NSString *)dataPath;

/**
 Copy a submatrix from the supplied delegates matrix.
 **/
-(BOOL)copyDataSliceFrom:(DataViewDelegate *) source startRow:(NSInteger)rowIdx startColumn:(NSInteger) colIdx endRow:(NSInteger) endRowIdx endCol:(NSInteger)endColIdx normalise:(BOOL)fromSource;


/**
 Copy a submatrix from the supplied delegates matrix and insert it at the last column.
 **/
-(BOOL)appendDataSliceFrom:(DataViewDelegate *) source startRow:(NSInteger)rowIdx startColumn:(NSInteger) colIdx endRow:(NSInteger) endRowIdx endCol:(NSInteger)endColIdx normalise:(BOOL)fromSource;

/**
 Normalise the current matrix.
 **/
-(void)normaliseAndCentre;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView;

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

-(void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;


-(NSCell *)tableView:(NSTableView *)tableView dataCellForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;


-(BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;


@end
