////////////////////////////////////////////////////////////////////////////////
// TSChallenge.cpp

// Includes
#include "BaArchive.h"
#include "TSChallenge.h"
#include "MtVector2.h"
#include "MtVector3.h"
#include "RsCamera.h"
#include "RsColour.h"
#include "RsFont.h"
#include "RsShader.h"
#include "RsUtil.h"
#include "RsRenderTarget.h"
#include "SgNode.h"
#include "MtAABB.h"
#include "SgMesh.h"
#include "SgAnimator.h"
#include "BtTime.h"
#include <stdio.h>
#include "UiKeyboard.h"
#include "HlLines.h"
#include "ApConfig.h"
#include "SgBone.h"
#include "HlModel.h"
#include "ShWAX9.h"
#include "HlFont.h"
#include "BtCollection.h"
#include "ShIMU.h"
#include "BtString.h"
#include "ShVibration.h"
#include "SbMain.h"
#include "ShTouch.h"

// unused
// "Pass this sealed condom to another player", // 28

/*
"Turn your phone\nover to begin", - linesun1
"Blow a kiss to\nanother player", - dots2
"Mark on Google maps where\nyou've had a 'moment'", - linesun2
"Take a photo of a body part", - dots2
"Write the name of\nyour first kiss", - halftone3
"Hold your phone and\ndraw a love heart\nin the air", - lines2
"Get everyone to leave the room\nthen describe a poignant\nor daring intimate moment\nto another player", - lines4
"Draw a GENDERbody part", - halftone4 + diamonds2 for the drawing
"Use a Google image search to find\na photo of a romantic location", - lines2
"Shout a pet or slang name\nfor a GENDERbody part", - linesun4
"Wink at one of the\nother players", - halftone3
"Choose a friend(s) then place your phones\nin your pockets and swing together\nto an imaginary beat", - lines4
"Sing, hum or whistle your best sexy theme tune", - dots3
"Stop playing the game.\nReturn in one minute", - diamonds2
"Choose a song from your mobile that you\nassociate with someone or romance", - halftone2
"Draw some tickly bits on your phone", - lines2
"Simulate a massage with your phone", - linesun3
"Read this bad sex paragraph", - diamonds1
"Tilt the screen on your mobile and watch\nan imaginary sperm win the race to the egg",
"Start at one end of the group and\ndraw a part of an intimate picture.\nPass to each player and let them continue", - lines4
"Pass this sealed condom to another player", - halftone4
"Draw something to do with sex, intimacy,\nor sexuality, NOW and quickly", -lines4
"Swap phones with another player and\ndon't give it back to them until the end of the game", - linesun2
"Draw something NOW", - lines3
"Take someone else's phone and\nrecord a message private message for them", - linesum4
"Take a selfie on someone else's phone", - dots3
"Hold hands with another player clasping the phone\nand swing your arms together", - linesum3
"Shine the light to illuminate part of your body", - halftone2
"Write a message to someone important in your life", - diamonds2
"Whisper a message into someone elses ear" - lines1
*/


const char *backgroundNames[] =
{
    "0facedown.png", // 0
    "linesun1.png",
    "1firstkiss.png", // 1
    "halftone3.png",
    "2blowkiss.png", // 2
    "dots2.png",
    "3paragraph.png", // 3
    "diamonds1.png",
    "4google.png", // 4
    "linesun2.png",
    "5photobodypart.png", // 5
    "dots2.png",
    "6heart.png", // 6
    "lines2.png",
    "7room.png", // 7
    "lines4.png",
    "8drawbodypart.png", // 8
    "diamonds2.png",
    "9imagesearch.png", // 9
    "lines2.png",
    "10slang.png", // 10
    "linesun4.png",
    "11wink.png", // 11
    "halftone3.png",
    "12swing.png", // 12
    "lines4.png",
    "13sing.png", // 13
    "dots3.png",
    "14justaminute.png", // 14
    "diamonds2.png",
    "15song.png", // 15
    "halftone2.png",
    "16tickly.png", // 16
    "lines2.png",
    "17massage.png", // 17
    "linesun3.png",
    "18drawsomething.png", // 18
    "lines3.png",
    "19swap.png", // 19
    "linesun2.png",
    "20record.png", // 20
    "linesun4.png",
    "21selfie.png", // 21
    "dots3.png",
    "22holdhands.png", // 22
    "linessun3.png",
    "23torch.png", // 25
    "halftone2.png",
    "24text.png", // 26
    "diamonds2.png",
    "25whisper.png", // 27
    "lines1.png",
};


const char *gameChallenges[] =
{
    "Turn your phone over to begin",                   // 0
    "Write the name of your first kiss",               // 1
    "Blow a kiss to another player",                   // 2
    "Read this bad sex paragraph",                      // 3
    "Mark on Google maps where you've had a 'moment'", // 4
    "Take a photo of a body part",                      // 5
    "Hold your phone and draw a love heart in the air", // 6
    "Get everyone to leave the room then describe a poignant or daring intimate moment to another player", // 7
    "Draw a GENDERbody part", // 8
    "Use a Google image search to find a photo of a romantic location", // 9
    "Shout a pet or slang name for a GENDERbody part", // 10
    "Wink at one of the other players", // 11
    "Choose a friend(s) then place your phones in your pockets and swing together to an imaginary beat", // 12
    "Sing, hum or whistle your best sexy theme tune", // 13
    "Stop playing the game. Return in one minute", // 14
    "Choose a song from your mobile that you associate with someone or romance", // 15
    "Draw some tickly bits on your phone", // 16
    "Simulate a massage with your phone", // 17
    "Draw something to do with sex, intimacy, or sexuality, NOW and quickly", // 18
    "Swap phones with another player and don't give it back to them until the end of the game", // 19
    "Take someone else's phone and record a message private message for them", // 20
    "Take a selfie on someone else's phone", // 21
    "Hold hands with another player clasping the phone and swing your arms together", // 22
    "Shine the light to illuminate part of your body", // 23
    "Write a message to someone important in your life", // 24
    "Whisper a message into someone elses ear" // 25
    
  //  "Tilt the screen on your mobile and watch an imaginary sperm win the race to the egg", // 18
  //  "Start at one end of the group and draw a part of an intimate picture. Pass to each player and let them continue", // 19
    
};

////////////////////////////////////////////////////////////////////////////////
// Setup

void TSChallenge::Setup( BaArchive *pGameArchive )
{
	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
	m_pShader->SetDirectionalLight( MtVector3( 0, 0, 0 ) );
	m_pShader->SetAmbient( RsColour( 1.0f, 1.0f, 1.0f, 1.0f ) );

	m_pRenderTarget = pGameArchive->GetMaterial( "rendertarget" );
    m_currentTime = 0;
	m_isFaceUp = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void TSChallenge::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void TSChallenge::Render( HlMenuScreen &menuScreen, HlMenuScreen &menuScreen2 )
{
    // MATT CHANGE THIS
    BtBool useText = false;
    //BtBool useText = BtTrue;
    
    if( useText == false )
    {
        if( SbMain::IsFaceUp() )
        {
            const BtChar *challengeBackground = backgroundNames[ ( m_index * 2 ) + 1 ];
            if( strlen( challengeBackground ) )
            {
                menuScreen.Render( challengeBackground );
            }
         
            const BtChar *challengeBackground2 = backgroundNames[ ( m_index * 2 ) + 0 ];
            
            if( strlen( challengeBackground2 ) )
            {
                menuScreen2.Render( challengeBackground2 );
            }
            
            if( m_index == 14 )
            {
                BtChar dest[256];
                sprintf( dest, "\n\n\n%.0f seconds", m_timer );
                SbMain::RenderMiddle( dest );
            }
        }
    }
    else
    {
        //static BtS32 lastIndex = -1;
        static BtU32 diceThrow = 0;
        
        // Cache the challenge text
        const BtChar *challengeText = gameChallenges[m_index];
        BtChar dest[256];
        
        if( ( diceThrow % 3 ) == 1 )
        {
            BtStringReplace( challengeText, dest, 256, "GENDER", "male " );
        }
        else if( ( diceThrow % 3 ) == 2 )
        {
            BtStringReplace( challengeText, dest, 256, "GENDER", "female " );
        }
        else
        {
            BtStringReplace( challengeText, dest, 256, "GENDER", "" );
        }
        if( m_index == 14 )
        {
            sprintf( dest, "Stop playing the game. Return in one %.0f seconds", m_timer );
        }
        
        // Render the challenge text
        if( SbMain::IsFaceUp() )
        {
            SbMain::RenderMiddle( dest );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SetChallenge

void TSChallenge::SetChallenge( BtS32 index )
{
    m_index = index;
}

////////////////////////////////////////////////////////////////////////////////
// SetTimer

void TSChallenge::SetTimer( BtFloat timer )
{
    m_timer = timer;
}

////////////////////////////////////////////////////////////////////////////////
// IsPaintingChallenge

BtBool TSChallenge::IsPaintingChallenge()
{
    if( m_index == 1 )
    {
        return BtTrue;
    }
	if( m_index == 8 )
	{
		return BtTrue;
	}
    if( m_index == 16 )
    {
        return BtTrue;
    }
    if( m_index == 18 )
    {
        return BtTrue;
    }
	return BtFalse;
}