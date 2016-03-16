////////////////////////////////////////////////////////////////////////////////
// SbWaxModel.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsCamera.h"
#include "MtMatrix4.h"
#include "HlMenuScreen.h"

class RsModel;
class BaArchive;
class RsMaterial;
class SgNode;

// Class definition
class TSChallenge
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							Render( HlMenuScreen &menuScreen, HlMenuScreen &menuScreen2 );

	// Accessors
    void                            SetTimer( BtFloat timer );
	void							SetChallenge( BtS32 index );
	BtBool							IsPaintingChallenge();

private:

	// Private functions
	// Private members
	RsShader					   *m_pShader;
	RsMaterial					   *m_pRenderTarget;
	BtU32							m_index;
    BtFloat                         m_currentTime;
	BtBool							m_isFaceUp;
    BtBool                          m_lastFaceUp;
    BtFloat                         m_timer;
};
