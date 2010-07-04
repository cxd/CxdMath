//
//  CDGraph.m
//  Untitled
//
//  Created by Chris Davey on 7/08/08.
//  Copyright 2008 none. All rights reserved.
//

#import "CDGraph.h"
#import "CDEdge.h"

@implementation CDGraph


@synthesize nodes;

@synthesize edges;

@synthesize isBidirectional;

-(id)init {
	[super init];
	nodes = [[NSMutableArray alloc] init];
	edges = [[NSMutableArray alloc] init];
	[nodes retain];
	[edges retain];
	return self;
}

-(void)dealloc 
{
	for(CDEdge* edge in edges)
	{
		[edge autorelease];	
	}
	[edges autorelease];
	for(CDNode* node in nodes)
	{
		[node autorelease];	
	}
	[nodes autorelease];
	[super dealloc];
	
}


-(CDNode *)add:(NSObject *)objData {
	id node = [[CDNode alloc] init];
	[node retain];
	[node setData:(id) objData];
	// add it to the array.
	[nodes addObject:node];
	return node;
}

-(CDNode *)remove:(NSPredicate *) predicate {
	CDNode* match = [self find:predicate];
	if (match != nil) {
		[match autorelease];
		[nodes removeObject:(id)match];
	}
	return match;
}

-(CDNode*)find:(NSPredicate *)predicate {
	CDNode *test;
	for(test in nodes) {
		if ([predicate evaluateWithObject:(id)test.data]) {
			return test;
		}
	}
	return nil;
}


-(CDEdge *)connect:(CDNode *)nodeFrom to:(CDNode *)nodeTo {
	CDEdge *edge = [[CDEdge alloc] init];
	[edge retain];
	edge.source = nodeFrom;
	edge.target = nodeTo;
	[edges addObject:(id)edge];
	[nodeFrom addNeighbour:(CDNode *) nodeTo];
	CDEdge* tmp =(CDEdge*) edge;
	if (isBidirectional) {
		edge = [[CDEdge alloc] init];
		edge.source = nodeTo;
		edge.target = nodeFrom;
		[edges addObject:(id)edge];
		[nodeTo addNeighbour:(CDNode*)nodeFrom];
	}
	return (CDEdge*)tmp;
}

-(CDEdge*)disconnect:(CDNode *)nodeFrom to:(CDNode *)nodeTo {
	CDEdge *edge = [self findEdge:nodeFrom to:nodeTo];
	if (edge == nil) return nil;
	CDEdge* tmp = (CDEdge*)edge;
	[nodeFrom.neighbours removeObject:(id)nodeTo];
	[edges removeObject:(id)edge];
	[edge release];
	if (isBidirectional) {
		edge = [self findEdge:nodeTo to:nodeFrom];
		if (edge == nil) return tmp;
		[nodeTo.neighbours removeObject:(id)nodeFrom];
		[edges removeObject:(id)edge];
		[edge release];
	}
	return tmp;
}

-(CDEdge*)findEdge:(CDNode *)nodeFrom to:(CDNode *)nodeTo {
	for(CDEdge *edge in edges) {
		if (([edge.source isEqual:(id)nodeFrom])&&
			([edge.target isEqual:(id)nodeTo])) {
			return edge;	
		}
	}
	return nil;
}

/**
 Find all edges from this node.
 **/
-(NSMutableArray *)findEdges:(CDNode *)nodeFrom
{
	NSMutableArray *set = [[NSMutableArray alloc] init];
	for(CDEdge * edge in edges)
	{
		if ([edge.source isEqual:(id)nodeFrom])
		{
			[set addObject:edge];	
		}
	}
	return set;
}


// NSCoding protocol implementation.
-(void) encodeWithCoder: (NSCoder *) encoder
{
	[encoder encodeBool: isBidirectional forKey: @"isBidirectional"];
	[encoder encodeObject: (id) nodes forKey: @"nodes"];
	[encoder encodeObject: (id) edges forKey: @"edges"];
}

-(id) initWithCoder: (NSCoder *) decoder 
{

	isBidirectional = [decoder decodeBoolForKey: @"isBidirectional"];
	nodes = [[decoder decodeObjectForKey: @"nodes"] retain];
	edges = [[decoder decodeObjectForKey: @"edges"] retain];
	return self;
}

// implementation of NSCopying using archiver to create a deep copy.
-(id)copyWithZone: (NSZone *) zone
{
	NSData *archive = [NSKeyedArchiver archivedDataWithRootObject: self];
	CDGraph *copy = [NSKeyedUnarchiver unarchiveObjectWithData:archive];
	return copy;
}

@end



