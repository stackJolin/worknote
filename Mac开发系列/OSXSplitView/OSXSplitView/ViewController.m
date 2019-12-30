//
//  ViewController.m
//  OSXSplitView
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import "ViewController.h"
#import "OSXSplitView.h"

@interface ViewController()

@property (nonatomic, strong) OSXSplitView *vSpliit;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
    OSXSplitView *vSplit = [OSXSplitView new];
    self.vSpliit = vSplit;
    vSplit.vertical = YES;
    vSplit.dividerStyle = NSSplitViewDividerStyleThin;
    
    NSView *vLeft = [NSView new];
    vLeft.frame = NSMakeRect(0, 0, 50, 50);
    vLeft.autoresizingMask = 0;
    vLeft.autoresizesSubviews = YES;
    vLeft.wantsLayer = YES;
    vLeft.layer.backgroundColor = NSColor.redColor.CGColor;
    
    NSView *vRight = [NSView new];
    vRight.autoresizingMask = 0;
    vRight.autoresizesSubviews = YES;
    vRight.wantsLayer = YES;
    vRight.layer.backgroundColor = NSColor.blueColor.CGColor;
    
    NSView *vRight1 = [NSView new];
    vRight1.autoresizingMask = 0;
    vRight1.autoresizesSubviews = YES;
    vRight1.wantsLayer = YES;
    vRight1.layer.backgroundColor = NSColor.yellowColor.CGColor;
    
    [vSplit addSubview:vLeft];
    [vSplit addSubview:vRight];
    [vSplit addSubview:vRight1];
    
    [vSplit addMinEdges:@{@"0" : @"80", @"1" : @"180"} maxEdges:@{@"0" : @"100", @"1" : @"260"}];
    [self.view addSubview:vSplit];
//    [vSplit setPosition: ofDividerAtIndex:0];
}

- (void)viewDidLayout {
    [super viewDidLayout];
    
    self.vSpliit.frame = self.view.bounds;
}

@end
