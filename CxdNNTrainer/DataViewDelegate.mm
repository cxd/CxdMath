//
//  DataView.m
//  CxdNNTrainer
//
//  Created by Chris Davey on 8/05/10.
//  Copyright 2010 none. All rights reserved.
//

#include "CxdMath.h"
#import "DataViewDelegate.h"


using namespace arma;
using namespace au::id::Cxd::IO;
using namespace au::id::Cxd::Operator;



@implementation DataViewDelegate


@synthesize resourcePath;

-(id)init
{
	self = [super init];	
	
	matrix = arma::mat(0,0);
	
	return self;
}

-(void)dealloc
{
	if (self.resourcePath != nil)
	{
	[self.resourcePath autorelease];	
	}
	
	[super dealloc];
}


-(NSInteger)numRows
{
	return matrix.n_rows;	
}

-(NSInteger)numCols
{
	return matrix.n_cols;	
}

-(arma::mat) getMatrix {
	return matrix;
}

-(BOOL)loadDataAtPath:(NSString *)dataPath
{
	if ( ([[dataPath lowercaseString]  hasSuffix:@".csv"]) || 
		([[dataPath lowercaseString] hasSuffix:@".txt"]))
	{
		// try to load as CSV.	
		MatrixReader reader(std::string([dataPath UTF8String]));
		reader.Read();
		matrix = reader.getMatrix();
		return YES;
	} else if ([[dataPath lowercaseString]  hasSuffix:@".cxdmat"]) {
		// load using the built in functions of matrix lib.
		try {
			matrix.load(std::string([dataPath UTF8String]), arma::raw_ascii);
			return YES;
		} catch(std::exception &e) {
			if (e.what() != NULL)
			{
				NSLog(@"Load data at path failed %@", dataPath);
				
				cerr << e.what() << endl;
			}
		}
	} else {
		// load using the built in functions of matrix lib.
		try {
			cout << "Load matrix from: " << [dataPath UTF8String] << endl;
			matrix.load(std::string([dataPath UTF8String]), auto_detect);
			return YES;
		} catch(std::exception &e) {
			if (e.what() != NULL)
			{
				NSLog(@"Load data at path failed %@", dataPath);

				cerr << e.what() << endl;
			}
		}
	}
	return NO;	
}

-(BOOL)saveDataToPath:(NSString *)dataPath
{
	if ( ([[dataPath lowercaseString] hasSuffix:@".csv"]) || 
		([[dataPath lowercaseString] hasSuffix:@".txt"]))
	{
		// try to save as CSV.	
		
		
	} else {
		// save using the built in functions of matrix lib.
		try {
			matrix.save(std::string([dataPath UTF8String]), arma_ascii);
			return YES;
		} catch(std::exception &e) {
			NSLog(@"Save data to path failed %@", dataPath);
			cerr << e.what() << endl;	
		}
	}
	return NO;	
}

/**
 Copy a submatrix from the supplied delegates matrix.
 **/
-(BOOL)copyDataSliceFrom:(DataViewDelegate *) source startRow:(NSInteger)rowIdx startColumn:(NSInteger) colIdx endRow:(NSInteger) endRowIdx endCol:(NSInteger)endColIdx normalise:(BOOL)fromSource
{
	// copy the source matrix so as not to modify it unexpectedly.
	std::stringstream s;
	[source getMatrix].save(s);
	arma::mat sourceMat;
	sourceMat.load(s);
	matrix = sourceMat.submat(rowIdx, colIdx, endRowIdx, endColIdx);
	NSLog(@"Original Size: [%d, %d] New size: [%d %d]",
		  sourceMat.n_rows, sourceMat.n_cols, matrix.n_rows, matrix.n_cols);
	if (fromSource) {
		[self normalise];	
	}
	return YES;
}

/**
 Copy a submatrix from the supplied delegates matrix and insert it at the last column.
 **/
-(BOOL)appendDataSliceFrom:(DataViewDelegate *) source startRow:(NSInteger)rowIdx startColumn:(NSInteger) colIdx endRow:(NSInteger) endRowIdx endCol:(NSInteger)endColIdx normalise:(BOOL)fromSource
{
	// copy the source matrix so as not to modify it unexpectedly.
	std::stringstream s;
	[source getMatrix].save(s);
	arma::mat sourceMat;
	sourceMat.load(s);
	
	sourceMat = sourceMat.submat(rowIdx, colIdx, endRowIdx, endColIdx);
	arma::mat newMat(matrix.n_rows, matrix.n_cols + sourceMat.n_cols);
	
	newMat.cols(0, matrix.n_cols - 1) = matrix;
	newMat.cols(matrix.n_cols, newMat.n_cols - 1) = sourceMat;
	matrix = newMat;
	if (fromSource) {
		[self normalise];	
	}
	return YES;
}


/**
 Normalise the current matrix.
 **/
-(void)normalise
{
	au::id::Cxd::Operator::MinMaxNormaliseMatrix<double> normOp(matrix);
	normOp.Operate();
	matrix = normOp.getResult();	
}


- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return matrix.n_rows;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if (rowIndex >= matrix.n_rows)
		return nil;
	
	if (aTableColumn == nil)
		return nil;
	
	
	// find the table column position using a predefined identifier.
	
	NSInteger col = [[aTableColumn identifier] intValue];
	
	if ( (rowIndex < 0) || (rowIndex > matrix.n_rows) )
		return nil;
	
	if ( (col < 0) || (col > matrix.n_cols) )
		return nil;
	
	double val = matrix(rowIndex, col);
	
	NSNumber *num = [NSNumber numberWithDouble:val];
	
	return num;
}

-(void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	
}


-(BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	
	return NO;	
}


@end
