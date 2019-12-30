//
//  NSDrawerFrame.h
//  OSXDrawer
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import <AppKit/NSFrameView.h>

@class NSColor;

@interface NSDrawerFrame : NSFrameView
{
    NSColor *drawerBackground;
    long long trackingTag;
    unsigned long long drawerEdge;
    BOOL registeredForEdgeChanges;
    BOOL shouldInvalidateShadow;
}

+ (double)minFrameWidthWithTitle:(id)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGSize)minContentSizeForMinFrameSize:(struct CGSize)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGSize)minFrameSizeForMinContentSize:(struct CGSize)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGRect)contentRectForFrameRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGRect)frameRectForContentRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
- (void)dealloc;
- (BOOL)shouldBeTreatedAsInkEvent:(id)arg1;
- (void)_postFrameChangeNotification;
- (void)viewDidMoveToWindow:(id)arg1;
- (void)viewWillMoveToWindow:(id)arg1;
- (void)mouseExited:(id)arg1;
- (void)mouseEntered:(id)arg1;
- (void)mouseDown:(id)arg1;
- (void)resizeWithEvent:(id)arg1;
- (void)drawerDidClose:(id)arg1;
- (void)drawerDidOpen:(id)arg1;
- (void)setEdge:(unsigned long long)arg1;
- (void)_removeTrackingRects;
- (void)_addTrackingRects;
- (BOOL)acceptsFirstMouse:(id)arg1;
- (void)adjustHalftonePhase;
- (id)contentFill;
- (void)_drawFrameRects:(struct CGRect)arg1;
- (void)drawFrame:(struct CGRect)arg1;
- (void)drawRect:(struct CGRect)arg1;
- (struct CGRect)resizeIndicatorRect;
- (void)setFrameSize:(struct CGSize)arg1;
- (struct CGRect)contentRect;
- (struct CGSize)minFrameSize;
- (long long)_shadowTypeForActiveAppearance:(BOOL)arg1;
- (BOOL)_shouldInvalidateShadow;
- (void)_setShouldInvalidateShadow:(BOOL)arg1;
- (void)shapeWindow;
- (void)registerForEdgeChanges:(id)arg1;
- (id)initWithFrame:(struct CGRect)arg1 styleMask:(unsigned long long)arg2 owner:(id)arg3;
- (BOOL)isOpaque;

@end
