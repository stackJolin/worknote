//
//  OSXSplitView.h
//  OSXSplitView
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OSXSplitView : NSSplitView

- (void)addMinEdge:(CGFloat)min index:(NSInteger)index;
- (void)addMaxEdge:(CGFloat)max index:(NSInteger)index;

- (void)addMinEdges:(NSDictionary<NSString *, NSString *> *)minDicts
           maxEdges:(NSDictionary<NSString *, NSString *> *)maxDicts;

@end

NS_ASSUME_NONNULL_END
