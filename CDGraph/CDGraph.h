//
//  CDGraph.h
//  Untitled
//
//  Created by Chris Davey on 7/08/08.
//  Copyright 2008 none. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "CDNode.h"
#import "CDEdge.h"

/*
 TODO: implement alloc and dealloc methods
 to initialise arrays and release arrays containing existing data.
 */

@interface CDGraph : NSObject <NSCoding, NSCopying> {

	NSMutableArray *nodes;
	
	NSMutableArray *edges;
	
	BOOL isBidirectional;
	
}

@property(assign) NSMutableArray *nodes;

@property(assign) NSMutableArray *edges;

@property(assign) BOOL isBidirectional;

-(void)dealloc;

-(CDNode *)add:(NSObject *)objData;

-(CDNode *)remove:(NSPredicate *)predicate;

-(CDNode*)find:(NSPredicate *)predicate;

-(CDEdge*)connect:(CDNode *)nodeFrom to:(CDNode *)nodeTo;

-(CDEdge*)disconnect:(CDNode *)nodeFrom to:(CDNode *)nodeTo;

-(CDEdge*)findEdge:(CDNode *)nodeFrom to:(CDNode *)nodeTo;

/**
 Find all edges from this node.
 **/
-(NSMutableArray *)findEdges:(CDNode *)nodeFrom;

-(void) encodeWithCoder: (NSCoder *) encoder;
-(id) initWithCoder: (NSCoder *) decoder;
-(id) copyWithZone: (NSZone *)zone;

@end