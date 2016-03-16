////////////////////////////////////////////////////////////////////////////////
// HlMenuScreen.h

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector2.h"

class BaArchive;
class RsMaterial;

// Class definition
class HlMenuScreen
{
public:
	
	// Public functions		
    void									Setup( BaArchive *pArchive, const BtChar *targetMaterial );
    void                                    Update();
    void                                    Render( const BtChar *pngName );
    
	// Accessors
    
protected:
	
	// Background
	RsMaterial							   *m_pMaterial;
    BtChar                                  m_lastName[256];
};
