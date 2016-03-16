////////////////////////////////////////////////////////////////////////////////
// SbMain.cpp

// Includes
#include <stdio.h>
#include "SbMain.h"
#include "ApConfig.h"
#include "RsUtil.h"
#include "RsShader.h"
#include "HlMaterial.h"
#include "HlFont.h"
#include "HlLines.h"
#include "HlMouse.h"
#include "UiKeyboard.h"
#include "RsMaterial.h"
#include "BtTime.h"
#include "ShTouch.h"
#include "ShNetwork.h"
#include "ErrorLog.h"
#include "ShIMU.h"
#include "ShVibration.h"
#include "SdSound.h"
#include "RsViewport.h"
#include "ShLabel.h"
#include <map>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Consts
const BtU32 MaxCards = 25;

////////////////////////////////////////////////////////////////////////////////
// Statics

BtBool SbMain::m_lastFaceUp = BtTrue;
TSPlayerState SbMain::g_playerState;

////////////////////////////////////////////////////////////////////////////////
// Globals

std::map <std::string, TSPlayerState> g_playerStates;
//const BtFloat IsDebug = BtTrue;
const BtBool IsDebug = BtFalse;
const BtFloat CardRestTime = 5.0f;

////////////////////////////////////////////////////////////////////////////////
// Init

void SbMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	MtVector2 v2Dimension;

	ApConfig::SetTitle( "Talk About Sex" );
    ApConfig::SetDebug( BtFalse );
	
    RdRandom::SetRandomSeed();

    // Reset our player
    g_playerState.m_gameState = TSGameState_Lobby;
    g_playerState.m_challengeIndex = 0;
    //g_playerState.m_challengeIndex = 14;
    g_playerState.m_faceUp = BtTrue;
    m_cardRead = BtTrue;
    
	int test = 1;
	int one  = 1;
	if( one && test )
	{
		int a=0;
		a++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsFaceUp

BtBool SbMain::IsFaceUp()
{
    return g_playerState.m_faceUp;
}

////////////////////////////////////////////////////////////////////////////////
// IsFaceDown

BtBool SbMain::IsFaceDown()
{
    return !g_playerState.m_faceUp;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void SbMain::Create()
{
	m_gameArchive.LoadDictionary();
	m_gameArchive.Load( "game" );

	m_utilityArchive.LoadDictionary();
	m_utilityArchive.Load("utility");

	m_pBell = m_gameArchive.GetSound("churchbell");
	m_pStart = m_gameArchive.GetSound("turnover");
	m_pTurnover = m_gameArchive.GetSound("start");

	m_pWhite2 = m_gameArchive.GetMaterial( "white2" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	m_pHeart = m_gameArchive.GetMaterial( "heart" );
	m_pSexTalk = m_gameArchive.GetMaterial( "sextalk" );
	 
	m_challenge.Setup( &m_gameArchive );
	
	RsMaterial *pMaterial2 = m_gameArchive.GetMaterial("white2");

	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlLines::Setup( pMaterial2, BtNull, BtNull );

	m_paint.Setup( &m_gameArchive );

    m_menuScreen.Setup( &m_gameArchive, "blankcard" );
    m_menuScreen2.Setup( &m_gameArchive, "blankcard2" );
    
    Reset();
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMain::Reset()
{
    UpdateNetwork();
    m_paint.Reset();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateDebugKeys

void SbMain::UpdateDebugKeys()
{
    BtBool isPressed = BtFalse;
    BtBool insideLeft = BtFalse;
    BtBool insideRight = BtFalse;
    
    for( BtU32 touchIndex = 0; touchIndex < MaxTouches; touchIndex ++ )
    {
        if( ShTouch::IsPressed(touchIndex) )
        {
            BtFloat xgap = RsUtil::GetWidth()  * 0.2f;
            BtFloat ygap = RsUtil::GetHeight() * 0.2f;
            
            isPressed = BtTrue;
            
            MtVector2 v2Position = ShTouch::GetPosition( touchIndex );
            if(v2Position.y < ygap )
            {
                if(v2Position.x < xgap )
                {
                    insideLeft = BtTrue;
                }
            }
            if(v2Position.y > RsUtil::GetHeight() - ygap )
            {
                if(v2Position.x < xgap )
                {
                    insideRight = BtTrue;
                }
            }
        }
    }
    
    if( isPressed )
    {
        if(insideLeft)
        {
            if( IsDebug )
                ApConfig::SetDebug( BtTrue );
            
            g_playerState.m_faceUp = !g_playerState.m_faceUp;
            g_playerState.m_elapsedStillness = 0;
            g_playerState.m_elapsedFaceDown  = 0;
            SendCommand( "Debug press top left", TSNetworkedActionType_ChangePlayerState, g_playerState );
            m_lastFaceUp = g_playerState.m_faceUp;
        }
        else if( insideRight )
        {
            IncrementChallenge();
            SendCommand( "Debug press bottom right", TSNetworkedActionType_ChangePlayerState, g_playerState  );
        }
        else
        {
            static BtU32 count = 0;
            ++count;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SendCommand

void SbMain::SendCommand( const BtChar *reason, TSNetworkedActionType gameNetworkAction, TSPlayerState playerState )
{
    ErrorLog::Printf( "SendCommand %s", reason );
    TSNetworkedAction gameNetwork( gameNetworkAction, g_playerState );
    
    // Send the network game command wrapped in a shared network action
    ShNetworkAction action;
    action.m_type   = ShNetworkActionType_Send;
    memcpy( action.m_buffer, (void*)&gameNetwork, sizeof(TSNetworkedAction) );
    action.m_size   = sizeof(action.m_buffer);
    ShNetwork::PushAction( action );
}

////////////////////////////////////////////////////////////////////////////////
// DecrementChallenge

void SbMain::DecrementChallenge()
{
    g_playerState.m_challengeIndex = g_playerState.m_challengeIndex - 1;
    if( g_playerState.m_challengeIndex < 0)
    {
        g_playerState.m_challengeIndex = MaxCards - 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
// IncrementChallenge

void SbMain::IncrementChallenge()
{
    g_playerState.m_challengeIndex = g_playerState.m_challengeIndex + 1;
    if( g_playerState.m_challengeIndex > MaxCards - 1)
    {
        g_playerState.m_challengeIndex = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
// UpdateOrientation

void SbMain::UpdateOrientation()
{
	if( ApConfig::IsWin() )
	{
		if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_SPACE ) )
		{
			g_playerState.m_faceUp = !g_playerState.m_faceUp;
		}
	}
    else if( BtTrue )
	{
        m_v3Orientation = MtVector3( 0, 0, 1 ) * ShIMU::GetTransform(0).GetTranspose();
 
        g_playerState.m_faceUp = BtTrue;
   
        if( m_v3Orientation.GetLength() )
        {
            if( m_v3Orientation.z < -0.90f )    // -0.99 is fully face down
            {
                g_playerState.m_faceUp = BtFalse;
            }
        }
        if( g_playerState.m_faceUp != m_lastFaceUp )
        {
            // Setup our debug string
            SendCommand( "Phone tuned over", TSNetworkedActionType_ChangePlayerState, g_playerState );
            m_lastFaceUp = g_playerState.m_faceUp;
        }
	}
}

////////////////////////////////////////////////////////////////////////////////
// UpdateStats

void SbMain::UpdateStats()
{
    // Cache the accelerometer
    MtVector3 v3Accelerometer = ShIMU::GetAccelerometer(0);

    // Get the size of the acceleration
    BtFloat movementSpeed = v3Accelerometer.GetLength();

    // Cache our peer name
    std::string peerName = ShNetwork::GetPeerName();
   
    // Copy the player state across
    strcpy( g_playerState.m_name, peerName.c_str() );

    // Look how look we have been stationary
    if( movementSpeed < 0.1f )
    {
        g_playerState.m_elapsedStillness += BtTime::GetTick();
    }
    else
    {
        g_playerState.m_elapsedStillness = 0;
    }
    
    BtFloat lastTime = g_playerState.m_elapsedFaceDown;
    
    // Look how look we have been face down
    if( g_playerState.m_faceUp == BtFalse )
    {
        g_playerState.m_elapsedFaceDown += BtTime::GetTick();
    }
    else
    {
        g_playerState.m_elapsedFaceDown = 0;
    }
    
    // Phone has been rested for 5 seconds
    if( ( lastTime < CardRestTime ) &&
        ( g_playerState.m_elapsedFaceDown > CardRestTime ) )
    {
        SendCommand( "Rested for 5 seconds", TSNetworkedActionType_ChangePlayerState, g_playerState );
    }
    
    // Update the player state array
    g_playerStates[peerName] = g_playerState;
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void SbMain::UpdateGame()
{
    // Update the games
	if( g_playerState.m_gameState == TSGameState_Lobby )
	{
        BtU32 numTouches = ShTouch::GetNumberOfTouches();
        if( numTouches >= 1 )
        {
            BtU32 numPeers = ShNetwork::GetNumConnectedPeers();
            (void)numPeers;
            //if( numPeers >= 1 )
            {
                g_playerState.m_gameState = TSGameState_WaitingOnAllDown;
                m_hasTouchedScreen = BtFalse;
				m_hasAlarm = BtFalse;
                SendCommand( "Exit lobby", TSNetworkedActionType_Progress, g_playerState );

                m_pStart->Play();
            }
        }
        
        m_menuScreen.Update();
	}
    else if( g_playerState.m_gameState == TSGameState_WaitingOnAllDown )
    {
        if( m_challenge.IsPaintingChallenge() && IsFaceUp() )
        {
            m_paint.Update();
        }
           
        // Problem
        // A Faceup
        // B Facedown - receives new index - doesn't know
        BtU32 networkedPeers = ShNetwork::GetNumConnectedPeers() + 1;
        m_random = g_playerState.m_challengeIndex % networkedPeers;
        
        if( GetPlayerIndex() == m_random )
        {
            BtBool allWaited = BtTrue;
            
            // Has everyone else waited five seconds
            for(auto const &it1 : g_playerStates)
            {
                TSPlayerState state = g_playerStates[it1.first];
                
                if( state.m_elapsedFaceDown < 5.0f )
                {
                    allWaited = BtFalse;
                }
            }
            
            if( allWaited )
            {
                ErrorLog::Printf( "%s's phone vibrated: New card ready", ShNetwork::GetPeerName().c_str() );

                // Set the new challenge and make sure we don't immediately go to the next game
                g_playerState.m_elapsedFaceDown = 0;
                g_playerState.m_elapsedStillness = 0;
                g_playerState.m_gameState = TSGameState_ShowCard;
                SendCommand( "New card to be revealed", TSNetworkedActionType_NewChallenge, g_playerState );
                
				// Vibrate the phone
                ShVibrationAction action;
                action.m_type = ShVibrationActionType_Start;
                ShVibration::PushAction( action );
                
                m_pTurnover->Play();
            }
        }
    }
    else if( g_playerState.m_gameState == TSGameState_ShowCard )
    {
        BtU32 networkedPeers = ShNetwork::GetNumConnectedPeers() + 1;
        m_random = g_playerState.m_challengeIndex % networkedPeers;
        
		// Reset
		m_paint.Reset();

        if( GetPlayerIndex() == m_random )
        {
            if( IsFaceUp() )
            {
                IncrementChallenge();
                
                // Set the new challenge and make sure we don't immediately go to the next game
                g_playerState.m_gameState = TSGameState_WaitingOnAllDown;
                SendCommand( "Go back to waiting on phones down", TSNetworkedActionType_NextCard, g_playerState );
                m_hasTouchedScreen = BtFalse;
				m_hasAlarm = BtFalse;
				m_startTimer = BtTime::GetElapsedTimeInSeconds();
            }
        }
    }
    
    // Have we touched the screen
    if( ShTouch::GetNumberOfTouches() )
    {
        m_hasTouchedScreen = BtTrue;
    }
    
	if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_ESCAPE))
	{
		g_playerState.m_gameState = TSGameState_Lobby;
		m_paint.Reset();
	}

    // Pop from the network if there is data available
    BtU32 numAlerts = ShNetwork::GetNumAlerts();
    if( numAlerts )
    {
        //ErrorLog::Printf( "Received network command" );
        ShNetworkAction alert = ShNetwork::PopAlert();
        
        if( alert.m_type == ShNetworkActionType_Received )
        {
            // Make a peer name
            std::string peerName = std::string( alert.m_peerID );
            
            // Cache the player index
            BtU32 playerIndex = (int)distance( g_playerStates.begin(), g_playerStates.find( peerName ) );
            
            TSNetworkedAction recvPlayerState( TSNetworkedActionType_MAX, g_playerState );    // To be written over
            memcpy( (void*)&recvPlayerState, alert.m_buffer, sizeof(TSNetworkedAction) );
            
            // Respond to the network state
            if( recvPlayerState.m_action == TSNetworkedActionType_Progress )
            {
                ErrorLog::Printf("Received TSNetworkedAction_Progress for %d:%s state %d",
                                 playerIndex, peerName.c_str(), recvPlayerState.m_state.m_gameState );
                g_playerStates[peerName] = recvPlayerState.m_state;
                
                // Set our new game state
                g_playerState.m_gameState = recvPlayerState.m_state.m_gameState;
            }
            if( recvPlayerState.m_action == TSNetworkedActionType_NewChallenge )
            {
                ErrorLog::Printf("Received TSNetworkedAction_NewChallenge for %d:%s state %d",
                                 playerIndex, peerName.c_str(), recvPlayerState.m_state.m_challengeIndex );

                // Show the card
                g_playerState.m_gameState = TSGameState_ShowCard;
                
                // Set our new challenge index
                g_playerState.m_challengeIndex = recvPlayerState.m_state.m_challengeIndex;
            
                // Set the state for this peer
                g_playerStates[peerName] = recvPlayerState.m_state;
            }
            if( recvPlayerState.m_action == TSNetworkedActionType_NextCard )
            {
                ErrorLog::Printf("Received TSNetworkedAction_NextCard for %d:%s state %d",
                                 playerIndex, peerName.c_str(), recvPlayerState.m_state.m_gameState );
                
                g_playerStates[peerName] = recvPlayerState.m_state;
                
                // Set our new game state
                g_playerState.m_gameState = recvPlayerState.m_state.m_gameState;
            }
            if( recvPlayerState.m_action == TSNetworkedActionType_ChangePlayerState )
            {
                ErrorLog::Printf("Received TSNetworkedAction_ChangePlayerState for %d:%s faceup %d",
                                playerIndex, peerName.c_str(), recvPlayerState.m_state.m_faceUp );
                g_playerStates[peerName] = recvPlayerState.m_state;
            }
        }
        else if( alert.m_type == ShNetworkActionType_DidChangeState )
        {
            ErrorLog::Printf( "Networked changed state" );
            UpdateNetwork();
        }
    }
    
	// Always set the challenge from the player
	m_challenge.SetChallenge(g_playerState.m_challengeIndex);
	m_challenge.Update();
}

////////////////////////////////////////////////////////////////////////////////
// UpdateNetwork

void SbMain::UpdateNetwork()
{
    // We are creating a map indexed by peer name that contains the player state
    std::string peerName = ShNetwork::GetPeerName();
    g_playerStates[peerName] = g_playerState;
    
    // Add the other peers to our map
    BtU32 peers = ShNetwork::GetNumConnectedPeers();
    for( BtU32 i=0; i<peers; i++ )
    {
        TSPlayerState playerState;
        std::string name = ShNetwork::GetName( i );
        strcpy( playerState.m_name, name.c_str() );
        playerState.m_elapsedStillness = 0;
        playerState.m_elapsedFaceDown = 0;
        playerState.m_faceUp = BtTrue;
        g_playerStates[name] = playerState;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMain::Update()
{
	// Are we closing
	if( m_isClosing == BtTrue )
	{
		// Unload the archive
		m_gameArchive.Unload();

		// Unload the archive
		m_utilityArchive.Unload();

		// Read to close
		m_isClosed = BtTrue;
	}
	else
	{
		if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_F1))
		{
			ApConfig::SetDebug(!ApConfig::IsDebug());
		}

        if( ApConfig::IsDebug() )
        {
            UpdateDebugKeys();
        }
        else
        {
            UpdateOrientation();
        }
		UpdateStats();
		UpdateGame();
	}
}

//////////////////////////////////////////////////////////////////
// GetPlayerIndex

BtU32 SbMain::GetPlayerIndex()
{
    return (int)distance( g_playerStates.begin(), g_playerStates.find( ShNetwork::GetPeerName() ) );
}

//////////////////////////////////////////////////////////////////
// SetupRenderToScreen

void SbMain::SetupRenderToScreen()
{
    // Make a camera
    BtFloat width = (BtFloat)RsUtil::GetWidth();
    BtFloat height = (BtFloat)RsUtil::GetHeight();
    RsCamera camera = RsCamera( 0.1f, 10000.0f, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), MtDegreesToRadians( 60.0f ) );
    camera.SetDimension( RsUtil::GetDimension() );
    camera.SetPerspective( BtFalse );
    camera.Update();
    
	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( camera );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Clear the screen to black
	pRenderTarget->SetClearColour( RsColour::BlackColour() );
	//pRenderTarget->SetClearColour( RsColour( 0.1f, 0.1f, 0.1f, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RenderMiddle

void SbMain::RenderMiddle( const BtChar *dest )
{
    ShLabel::PushAction( dest );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMain::Render()
{
	if( m_isClosing == BtTrue )
	{
		return;
	}
    
    RenderMiddle( "" );

	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();

	// Do the paint to texture first
	if( m_challenge.IsPaintingChallenge() )
	{
		m_paint.PreRender();
	}

	SetupRenderToScreen();

	// Render the title
	if( g_playerState.m_gameState == TSGameState_Lobby )
	{
        m_menuScreen.Render( "landing.png" );
        
        BtU32 numPeers = ShNetwork::GetNumConnectedPeers();
        if( numPeers == 0 )
        {
            BtChar dest[128];
            sprintf( dest, "You must have at least two players to start" );
            ShLabel::PushAction( dest, 2 );
        }
        else
        {
            BtChar dest[128];
            sprintf( dest, "Touch screen to continue" );
            ShLabel::PushAction( dest, 2 );
        }
    }
    else if( g_playerState.m_gameState == TSGameState_WaitingOnAllDown )
    {
        BtChar text[512];

        if( IsFaceUp() )
        {
            if( g_playerState.m_challengeIndex == 0 )
            {
                m_challenge.Render( m_menuScreen, m_menuScreen2 );
            }
            else
            {
                // Cache the number of networked peers
                BtU32 networkedPeers = ShNetwork::GetNumConnectedPeers() + 1;
                
                // What random number is it?
                BtU32 random = ( g_playerState.m_challengeIndex - 1 ) % networkedPeers;
                
                // Writes the text for each challenge
                if( random == GetPlayerIndex() )
                {
					if( m_challenge.IsPaintingChallenge() && m_hasTouchedScreen )
					{
						m_paint.Render();
					}
					else if( g_playerState.m_challengeIndex == 14 )
					{
						BtFloat current = BtTime::GetElapsedTimeInSeconds();
						BtFloat elapsedTime = current - m_startTimer;

                        m_challenge.SetTimer( 60.0f - elapsedTime );
                        
						if( elapsedTime > 60.0f )
						{
							sprintf(text, "Come back!" );
							
							SbMain::RenderMiddle(text);

							if( m_hasAlarm == BtFalse )
							{
								m_pBell->Play();
								m_hasAlarm = BtTrue;
							}
						}
						else
						{
							m_challenge.Render( m_menuScreen, m_menuScreen2 );
						}
					}
                    else if( g_playerState.m_challengeIndex == 3 )
                    {
                        if( m_hasTouchedScreen )
                        {
                            sprintf( text, "Alexis brought me close to their neck, and I smiled as I took in the smell of their sweet aroma, once more. I let out a contented sigh as my thoughts irrevocably slipped to my Skye. What would they make of our blossoming relationship?" );
                            
                            ShLabel::PushAction( text, 1 );
                            //SbMain::RenderMiddle( text, 1 );
                        }
                        else
                        {
                            m_challenge.Render( m_menuScreen, m_menuScreen2 );
                        }
                    }
                    else
                    {
                        m_challenge.Render( m_menuScreen, m_menuScreen2  );
                    }
	            }
                else
                {
                    sprintf( text, "This one is a secret" );
                    SbMain::RenderMiddle( text );
                }
            }
        }
        else
        {
            sprintf( text, "Waiting for everyone\nto place down phones (%.2f)", g_playerState.m_elapsedFaceDown );
            SbMain::RenderMiddle( text );
        }
    }
    else if( g_playerState.m_gameState == TSGameState_ShowCard )
    {
        if( IsFaceUp() )
        {
            // This won't be seen as the state changes quickly
            
            // if we show something here make sure we don't make a mistake and show the challenge
            // to the person not meant to see it
        }
        else
        {
            BtChar text[512];
            
            BtU32 networkedPeers = ShNetwork::GetNumConnectedPeers() + 1;
            m_random = g_playerState.m_challengeIndex % networkedPeers;
            
            if( GetPlayerIndex() == m_random )
            {
                sprintf( text, "Card revealed. You must turn over for the game to progress" );
                SbMain::RenderMiddle( text );
            }
            else
            {
                sprintf( text, "Waiting for everyone\nto be rest the phones (%.2f)", g_playerState.m_elapsedFaceDown );
                SbMain::RenderMiddle( text );
            }
        }
    }

    DebugRender();

    { // Draw hearts to indicate the number of connections
        
        // Get the number of connected peers including this one
        BtU32 connectedPeers = ShNetwork::GetNumConnectedPeers() + 1;
       
		// Render
		MtVector2 v2Dimension = RsUtil::GetDimension() * 0.05f;
		MtVector2 v2Position = RsUtil::GetDimension();
		v2Position.x -= v2Dimension.x * connectedPeers;
		v2Position.y = 0;
		for( BtU32 i=0; i<connectedPeers; i++ )
		{
			m_pHeart->Render( v2Position, v2Dimension, RsColour::WhiteColour(), MaxSortOrders-1 );
			v2Position.x += v2Dimension.x;
		}
    }

    // Render the mouse
	HlMouse::Render();
}

////////////////////////////////////////////////////////////////////////////////
// DebugRender

void SbMain::DebugRender()
{
    if( ApConfig::IsDebug() )
    {
        BtChar text[1024];
        sprintf(text, "PREV Face up=%d Random %d\n", g_playerState.m_faceUp, m_random );
        MtVector2 v2Pos = RsUtil::GetDimension();
        v2Pos.x = 0;
        v2Pos.y = 0;
        HlFont::RenderHeavy(v2Pos, text, MaxSortOrders - 1);

        sprintf( text, "NEXT\n%d", g_playerState.m_challengeIndex );
        MtVector2 v2Dimension = HlFont::GetDimension( text );
        v2Pos = RsUtil::GetDimension();
        v2Pos.x = 0;
        v2Pos.y -= v2Dimension.y;
        HlFont::RenderHeavy( v2Pos, text, MaxSortOrders-1 );
        
        BtU32 index = 0;
        
        // Printout the game player states
        strcpy( text, "" );
        for(auto const &it1 : g_playerStates)
        {
            TSPlayerState state = g_playerStates[it1.first];
            sprintf( text + strlen(text), "%d: Peer name: %s %d %.2f",
                    index, state.m_name, state.m_faceUp, state.m_elapsedFaceDown );
            strcat( text, "\n" );
            
            ++index;
        }
        HlFont::RenderHeavy( MtVector2( 0, 100 ), text, MaxSortOrders-1 );
    }
    
    // Lobby
    /*
    if( g_playerState.m_gameState != TSGameState_Lobby )
    {
        //m_challenge.SetChallenge(3);
        m_challenge.Render( m_menuScreen, m_menuScreen2  );
    }
    */
}

////////////////////////////////////////////////////////////////////////////////
// PreRender

void SbMain::PreRender()
{
}

////////////////////////////////////////////////////////////////////////////////
// PostRender

void SbMain::PostRender()
{
	// Render the mouse
	//m_renderMouse.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMain::Destroy()
{
}
