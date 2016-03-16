////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "SdSound.h"
#include "GaProject.h"
#include "SdSound.h"
#include "TsPaint.h"
#include "TsChallenge.h"
#include "HlMenuScreen.h"

class SbMain;
class RsTexture;
class RsShader;
class RsMaterial;
class SgNode;

const BtFloat WinWidth  = 800.0f;
const BtFloat WinHeight = 600.0f;
const BtFloat NormalWidth  = 960.0f;
const BtFloat NormalHeight = 640.0f;

enum TSNetworkedActionType
{
    TSNetworkedActionType_Progress           = 1001,
    TSNetworkedActionType_ChangePlayerState  = 1002,
    TSNetworkedActionType_NewChallenge       = 2003,
    TSNetworkedActionType_NextCard           = 2003,
    TSNetworkedActionType_MAX                = 2004
};

enum TSGameState
{
    TSGameState_Lobby = 1000,
    TSGameState_WaitingOnAllDown = 2000,
    TSGameState_ShowCard = 3000,
};

struct TSPlayerState
{
    TSGameState                     m_gameState;
    BtS32                           m_challengeIndex;
    BtChar                          m_name[256];
    BtFloat                         m_elapsedFaceDown;
    BtFloat                         m_elapsedStillness;
    BtBool                          m_faceUp;
};

struct TSNetworkedAction
{
    TSNetworkedAction( TSNetworkedActionType action, TSPlayerState playerState )
    {
        m_state = playerState;
        m_action = action;
    }
    
    TSNetworkedActionType           m_action;
    TSPlayerState                   m_state;
};

// Class definition
class SbMain : public GaProject
{
public:

	// Public functions
	void							Init();
	void							Reset();
	void							Create();
	void							Update();
	void							Render();
	void							PreRender();
	void							PostRender();
	void							Destroy();
   
    void                            UpdateNetwork();
    static void                     RenderMiddle( const BtChar *dest );
    
	// Accessors
    BtU32                           GetPlayerIndex();
    
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();

    static BtBool                   IsFaceUp();
    static BtBool                   IsFaceDown();
    
private:

	// Private functions
    void                            UpdateOrientation();
    
    void                            SendCommand( const BtChar *reason, TSNetworkedActionType act, TSPlayerState plState );
    void                            DecrementChallenge();
    void                            IncrementChallenge();
    
    void                            UpdateDebugKeys();
    void                            DebugRender();
    
	void							SetupRenderToScreen();

    void                            UpdateStats();
	void							UpdateGame();
	void							RenderScene();

	void							RenderGame();
	void							RenderFont();

	// Private members
	BaArchive						m_gameArchive;
	BaArchive						m_utilityArchive;

	// Resources
	RsMaterial					   *m_pHeart;
	RsMaterial					   *m_pTouch;
	RsMaterial					   *m_pSexTalk;
	RsShader					   *m_pShader;
	BtBool							m_isClosing;
	BtBool							m_isClosed;
	MtVector3						m_v3Position;
	//SbCamera						m_camera;
	RsMaterial					   *m_pWhite2;
	TsPaint							m_paint;

	// Test classes
	TSChallenge						m_challenge;
    
    static BtBool                   m_lastFaceUp;
    
    MtVector3                       m_v3Orientation;
    
    BtBool                          m_cardRead;
    
    BtU32                           m_random;
    
    BtBool                          m_hasTouchedScreen;

	BtBool                          m_hasAlarm;

	BtFloat							m_startTimer;

    static TSPlayerState            g_playerState;
    
    SdSound                        *m_pBell;
    SdSound                        *m_pStart;
    SdSound                        *m_pTurnover;
    
    HlMenuScreen                    m_menuScreen;
    HlMenuScreen                    m_menuScreen2;
};

inline void SbMain::SetClosing()
{
	m_isClosing = BtTrue;
}

inline BtBool SbMain::IsClosed()
{
	return m_isClosed;
}

inline BtBool SbMain::IsClosing()
{
	return m_isClosing;
}
