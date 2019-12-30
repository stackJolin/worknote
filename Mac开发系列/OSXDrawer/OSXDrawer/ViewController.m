//
//  ViewController.m
//  OSXDrawer
//
//  Created by houlin on 2019/12/28.
//  Copyright © 2019 houlin. All rights reserved.
//

#import "ViewController.h"
#import <AppKit/AppKit.h>

@interface ViewController()<NSDrawerDelegate>

@property (nonatomic, strong) NSDrawer *drawer;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
    
    NSButton *btn = [NSButton new];
    btn.title = @"设置";
    btn.frame = NSMakeRect(100, 100, 50, 10);
    btn.target = self;
    btn.action = @selector(clickBtn);
    [self.view addSubview:btn];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


- (void)clickBtn {
    
    [self.drawer close];
    self.drawer = [[NSDrawer alloc] initWithContentSize:NSMakeSize(200, 200) preferredEdge:NSRectEdgeMinY];
//    self.drawer = drawer;
    self.drawer.delegate = self;
    NSView *v = [NSView new];
    v.wantsLayer = YES;
    v.layer.backgroundColor = NSColor.greenColor.CGColor;
    self.drawer.contentView = v;
    self.drawer.parentWindow = self.view.window;
    self.drawer.leadingOffset = 0;
    self.drawer.trailingOffset = 0;
    self.drawer.minContentSize = NSMakeSize(100, 100);
    self.drawer.maxContentSize = NSMakeSize(300, 300);
    
    [self.drawer openOnEdge:NSRectEdgeMinY];
}

@end
