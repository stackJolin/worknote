//
//  OSXSplitView.m
//  OSXSplitView
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import "OSXSplitView.h"

@interface OSXSplitView()<NSSplitViewDelegate>

@property (nonatomic, strong) NSMutableDictionary<NSString *, NSString *> *minEdgeMaps;
@property (nonatomic, strong) NSMutableDictionary<NSString *, NSString *> *maxEdgeMaps;

@end

@implementation OSXSplitView

- (instancetype)initWithFrame:(NSRect)frameRect {
    if (self = [super initWithFrame:frameRect]) {
        self.delegate = self;
    }
    return self;
}

- (void)addMinEdge:(CGFloat)min
             index:(NSInteger)index {
    self.minEdgeMaps[@(index).stringValue] = @(min).stringValue;
}

- (void)addMaxEdge:(CGFloat)max
             index:(NSInteger)index {
    self.maxEdgeMaps[@(index).stringValue] = @(max).stringValue;
}

- (void)addMinEdges:(NSDictionary<NSString *, NSString *> *)minDicts
           maxEdges:(NSDictionary<NSString *, NSString *> *)maxDicts {
    
    [self.minEdgeMaps setValuesForKeysWithDictionary:minDicts];
    [self.maxEdgeMaps setValuesForKeysWithDictionary:maxDicts];
}

//- (CGFloat)minPossiblePositionOfDividerAtIndex:(NSInteger)dividerIndex {
//    if (self.minEdgeMaps[@(dividerIndex).stringValue]) {
//        return self.minEdgeMaps[@(dividerIndex).stringValue].floatValue;
//    }
//    else {
//        return [super minPossiblePositionOfDividerAtIndex:dividerIndex];
//    }
//}
//
//- (CGFloat)maxPossiblePositionOfDividerAtIndex:(NSInteger)dividerIndex {
//    if (self.maxEdgeMaps[@(dividerIndex).stringValue]) {
//        return self.maxEdgeMaps[@(dividerIndex).stringValue].floatValue;
//    }
//    else {
//        return [super maxPossiblePositionOfDividerAtIndex:dividerIndex];
//    }
//}

- (NSMutableDictionary<NSString *,NSString *> *)minEdgeMaps {
    if (!_minEdgeMaps) {
        _minEdgeMaps = [NSMutableDictionary<NSString *,NSString *> new];
    }
    return _minEdgeMaps;
}

- (NSMutableDictionary<NSString *,NSString *> *)maxEdgeMaps {
    if (!_maxEdgeMaps) {
        _maxEdgeMaps = [NSMutableDictionary<NSString *,NSString *> new];
    }
    return _maxEdgeMaps;
}

//**********************************************************
// MARK : - NSSplitViewDelegate
//**********************************************************

- (CGFloat)splitView:(NSSplitView *)splitView constrainMinCoordinate:(CGFloat)proposedMinimumPosition ofSubviewAt:(NSInteger)dividerIndex {
    
    NSLog(@"proposedMaximumPosition: %lf",proposedMinimumPosition);
    
    if (self.minEdgeMaps[@(dividerIndex).stringValue]) {
        return self.minEdgeMaps[@(dividerIndex).stringValue].floatValue;
    }

    return proposedMinimumPosition;
}

- (CGFloat)splitView:(NSSplitView *)splitView constrainMaxCoordinate:(CGFloat)proposedMaximumPosition ofSubviewAt:(NSInteger)dividerIndex {
    
    if (self.maxEdgeMaps[@(dividerIndex).stringValue]) {
        return self.maxEdgeMaps[@(dividerIndex).stringValue].floatValue;
    }
    
    NSLog(@"proposedMaximumPosition: %lf",proposedMaximumPosition);
    return proposedMaximumPosition;
}

@end
