////////////////////////////////////////////////////////////////////////////////
// TsPaint.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "MtMatrix4.h"
#include "BtArray.h"
#include "RsCamera.h"
#include "MtVector3.h"
#include "RsMaterial.h"
#include "RsVertex.h"

class RsMaterial;

// Class definition
class TsPaint
{
public:

	// Public function
	void				Reset();
	void				Setup( BaArchive *pArchive );
	void				Update();
	void				Render();
	void				PreRender();

	// Accessors
    BtBool              IsPainting();
    
private:
	
	void				SetupRenderToTexture();
   
    static RsMaterial  *m_pRenderTarget;
    BtBool              m_isRender;
	BtBool				m_isClear;
	MtVector2			m_v2Last;
	MtVector2			m_v2LastL;
	MtVector2			m_v2LastR;
	BtBool				m_isFirst;
	RsMaterial		   *m_pWhite2;
    BtBool              m_isPainting;
    RsVertex2           m_vertex[4];
    BtU32               m_width;
    BtU32               m_height;
};

