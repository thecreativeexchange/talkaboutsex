//
//  AppDelegate.m
//  TalkAboutSexApp
//
//  Created by localadmin on 04/06/2015.
//  Copyright (c) 2015 localadmin. All rights reserved.
//

#import "AppDelegate.h"
#import "ShNetwork.h"
#import <string>
#import <vector>

@interface AppDelegate ()
@property (nonatomic) UIBackgroundTaskIdentifier backgroundTask; //declaring a background task
@end

@implementation AppDelegate
@synthesize sessionController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    sessionController = [[SessionController alloc] init];
    self.sessionController.delegate = self;

    // Override point for customization after application launch.
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    
    self.backgroundTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^
                           {
                               //This is called 3 seconds before the time expires
                               //Here: Kill the session, advertisers, nil its delegates,
                               //      which should correctly send a disconnect signal to other peers
                               //      it's important if we want to be able to reconnect later,
                               //      as the MC framework is still buggy
                               [[UIApplication sharedApplication] endBackgroundTask:self.backgroundTask];
                               self.backgroundTask = UIBackgroundTaskInvalid; //Invalidate the background task
                           }];
    
    if (self.backgroundTask == UIBackgroundTaskInvalid) {
        NSLog(@"This application does not support background mode");
    } else {
        //if application supports background mode, we'll see this log.
        NSLog(@"Application will continue to run in background");
    }
    sessionController = nil;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    
    // Display a debug message
    NSLog( @"Entering background mode with peer to peer started" );
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
 
    sessionController = nil;
    sessionController = [[SessionController alloc] init];
    self.sessionController.delegate = self;
    
 
    [[UIApplication sharedApplication] endBackgroundTask:UIBackgroundTaskInvalid];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (void)sessionDidChangeState
{
    // Loop through the connected peers
    std::vector<std::string> myConnectedPeers;
    
    // Add the peers to our vector
    for( BtU32 i=0; i<self.sessionController.connectedPeers.count; i++ )
    {
        MCPeerID *peerID = self.sessionController.connectedPeers[i];
        std::string myPeerID( [[peerID displayName] UTF8String] );
        myConnectedPeers.push_back( myPeerID );
    }
    
    // Set this in the shared network class
    ShNetwork::SetConnectedPeers( myConnectedPeers );
   
    // Signal that we've had a change to the network state
    ShNetworkAction action;
    action.m_type = ShNetworkActionType_DidChangeState;
    ShNetwork::PushAlert( action );
}

-(void)sendData:(NSData*)data
{
    [self.sessionController sendData:data];
}

@end
