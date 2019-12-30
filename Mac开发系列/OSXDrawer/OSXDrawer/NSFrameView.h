//
//  NSFrameView.h
//  OSXDrawer
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import <AppKit/NSView.h>

@class NSButton, NSCell, NSMutableArray, NSString;

@interface NSFrameView : NSView
{
    unsigned long long styleMask;
    NSString *_title;
    NSCell *titleCell;
    NSButton *closeButton;
    NSButton *zoomButton;
    NSButton *minimizeButton;
    BOOL resizeByIncrement;
    BOOL ___available_for_whatever;
    unsigned char tabViewCount;
    struct CGSize resizeParameter;
    int __unusedShadowState;
    NSMutableArray *edgeResizingTrackingAreas;
}

+ (unsigned long long)_validateStyleMask:(unsigned long long)arg1;
+ (double)minFrameWidthWithTitle:(id)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGSize)minContentSizeForMinFrameSize:(struct CGSize)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGSize)minFrameSizeForMinContentSize:(struct CGSize)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGRect)contentRectForFrameRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
+ (struct CGRect)frameRectForContentRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
+ (id)_defaultTitleFontForWindowWithStyleMask:(unsigned long long)arg1;
+ (void)initTitleCell:(id)arg1 styleMask:(unsigned long long)arg2;
+ (void)initialize;
- (BOOL)shouldUseStyledTextInTitleCell:(id)arg1;
- (struct __CFString *)customizedBackgroundTypeForTitleCell:(id)arg1;
- (id)_edgeResizingTrackingAreas;
- (void)updateTrackingAreas;
- (struct CGSize)minFrameSize;
- (struct CGRect)contentRect;
- (struct CGRect)dragRectForFrameRect:(struct CGRect)arg1;
- (struct CGSize)minFrameSizeForMinContentSize:(struct CGSize)arg1 styleMask:(unsigned long long)arg2;
- (struct CGRect)contentRectForFrameRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
- (struct CGRect)frameRectForContentRect:(struct CGRect)arg1 styleMask:(unsigned long long)arg2;
- (void)setTitle:(id)arg1 andDefeatWrap:(BOOL)arg2;
- (void)addSubview:(id)arg1;
- (void)_resetDragMargins;
- (struct CGRect)_draggableFrame;
- (void)_clearDragMargins;
- (struct CGSize)miniaturizedSize;
- (id)zoomButton;
- (id)minimizeButton;
- (id)closeButton;
- (void)setStyleMask:(unsigned long long)arg1;
- (void)setIsResizable:(BOOL)arg1;
- (void)setIsClosable:(BOOL)arg1;
- (void)_setNonactivatingPanel:(BOOL)arg1;
- (void)_setUtilityWindow:(BOOL)arg1;
- (struct CGRect)titlebarRect;
- (struct CGRect)_maxTitlebarTitleRect;
- (id)titleFont;
- (id)defaultTitleFont;
- (BOOL)frameNeedsDisplay;
- (void)_setFrameNeedsDisplay:(BOOL)arg1;
- (void)setDocumentEdited:(BOOL)arg1;
- (void)setRepresentedFilename:(id)arg1;
- (id)representedFilename;
- (unsigned long long)styleMask;
- (struct CGSize)aspectRatio;
- (void)setAspectRatio:(struct CGSize)arg1;
- (struct CGSize)resizeIncrements;
- (void)setResizeIncrements:(struct CGSize)arg1;
- (void)titleBarHiddenChanged;
- (void)initTitleCell:(id)arg1;
- (id)titleCell;
- (void)setTitle:(id)arg1;
- (id)title;
- (void)tabViewRemoved;
- (void)tabViewAdded;
- (id)contentFill;
- (id)frameColor;
- (void)systemColorsDidChange:(id)arg1;
- (void)adjustHalftonePhase;
- (void)setUpGState;
- (void)_drawFrameShadowAndFlushContext:(id)arg1;
- (void)_setShadowParameters;
- (struct CGSize)_shadowOffsetForActiveAppearance:(BOOL)arg1;
- (unsigned long long)_shadowFlags;
- (long long)_shadowTypeForActiveAppearance:(BOOL)arg1;
- (long long)_shadowType;
- (double)_distanceFromToolbarBaseToTitlebar;
- (void)_hideToolbarWithAnimation:(BOOL)arg1;
- (void)_showToolbarWithAnimation:(BOOL)arg1;
- (BOOL)_toolbarIsManagedByExternalWindow;
- (BOOL)_toolbarIsHidden;
- (BOOL)_toolbarIsShown;
- (BOOL)_toolbarIsInTransition;
- (BOOL)_canHaveToolbar;
- (long long)shadowState;
- (void)setShadowState:(long long)arg1;
- (BOOL)_isHUDWindow;
- (BOOL)_isUtility;
- (BOOL)_isSheet;
- (void)_updateButtonState;
- (double)contentAlpha;
- (void)drawWindowBackgroundRegion:(struct CGSRegionObject *)arg1;
- (void)drawWindowBackgroundRect:(struct CGRect)arg1;
- (void)drawThemeContentFill:(struct CGRect)arg1 inView:(id)arg2;
- (BOOL)usesCustomDrawing;
- (void)drawFrame:(struct CGRect)arg1;
- (void)_drawFrameRects:(struct CGRect)arg1;
- (void)drawRect:(struct CGRect)arg1;
- (void)tile;
- (void)tileAndSetWindowShape:(BOOL)arg1;
- (void)shapeWindow;
- (void)dealloc;
- (id)initWithFrame:(struct CGRect)arg1;
- (id)initWithFrame:(struct CGRect)arg1 styleMask:(unsigned long long)arg2 owner:(id)arg3;

@end
