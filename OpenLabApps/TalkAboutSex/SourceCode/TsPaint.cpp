////////////////////////////////////////////////////////////////////////////////
// TsPaint.cpp

// Includes
#include "TsPaint.h"
#include "RsRenderTarget.h"
#include "HlFont.h"
#include "ShTouch.h"
#include "HlLines.h"
#include "RsUtil.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "UiKeyboard.h"
#include <stdio.h>

RsMaterial *TsPaint::m_pRenderTarget;

////////////////////////////////////////////////////////////////////////////////
// Setup

void TsPaint::Setup( BaArchive *pArchive )
{
	m_pRenderTarget = pArchive->GetMaterial( "paint" );
	m_pWhite2 = pArchive->GetMaterial( "white2" );
	m_isClear = BtFalse;
    m_isRender = BtFalse;
    m_width = m_pRenderTarget->Width();
    m_height = m_pRenderTarget->Height();
}

//////////////////////////////////////////////////////////////////
// SetupRenderToTexture

void TsPaint::SetupRenderToTexture()
{
	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the render target name
	pRenderTarget->SetName( "painting" );

	// Make a new render target
	RsCamera camera( (BtFloat)m_width, (BtFloat)m_height, 0.01f, 10000.0f, RsViewport(0, 0, m_width, m_height) );

	// Update the camera
	camera.Update();

	// Set the texture
    RsTexture *pTexture = m_pRenderTarget->GetTexture(0);
    pRenderTarget->SetTexture( pTexture );

	// Set the camera
	pRenderTarget->SetCamera(camera);

	// Clear the render target
	if( m_isClear )
	{
		m_isClear = BtFalse;
	
		pRenderTarget->SetCleared( BtTrue );
	}
	else
	{
		pRenderTarget->SetCleared( BtFalse );
	}

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtFalse );

	// Clear the screen to black
	pRenderTarget->SetClearColour( RsColour::BlackColour() );
	
	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void TsPaint::Reset()
{
	m_isClear = BtTrue;
	m_v2Last    = MtVector2( 0, 0 );
	m_isFirst = BtTrue;
    m_isPainting = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void TsPaint::Update()
{
    if( ShTouch::IsShaken() )
    {
        m_isClear = BtTrue;
    }
    if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_R ) )
    {
        Reset();
    }
    
    m_isRender = BtFalse;

    for(BtU32 i=0; i<MaxTouches; i++ )
    {
        MtVector2 v2Position = ShTouch::GetPosition(i);
        
        if( ShTouch::IsPressed(i) )
        {
            m_v2Last = v2Position;
        }
           
        if( ShTouch::IsHeld(i) )
        {
            m_isPainting = BtTrue;
            
            MtVector2 v2Delta = v2Position - m_v2Last;
            
            if (v2Delta.GetLength() > 0.01f) // 1% = 11 pixels of 1080p
            {
                v2Delta.y = -v2Delta.y;
                
                MtVector2 v2ScreenPosition = v2Position;
                
                m_v2Last = v2Position;
                
                MtVector2 v2ScreenDimension = RsUtil::GetDimension();
                
                v2ScreenPosition.x /= v2ScreenDimension.x;
                v2ScreenPosition.y /= v2ScreenDimension.y;
                
                v2ScreenPosition.x *= m_width;
                v2ScreenPosition.y *= m_height;
                
                v2ScreenPosition.y = m_height - v2ScreenPosition.y;
                
                BtFloat width = RsUtil::GetHeight() * 0.01f;
                
                MtVector3 v3Delta( v2Delta.x, v2Delta.y, 0 );
                MtVector3 v3Normalise = v3Delta.GetNormalise();
                MtVector3 v3In(0, 0, 1);
                MtVector3 v3Cross = v3Normalise.CrossProduct( v3In );
                
                v3Cross *= width;
                
                if( m_isFirst == BtTrue )
                {
                    m_v2LastR = v2ScreenPosition + MtVector2(v3Cross.x, v3Cross.y);
                    m_v2LastL = v2ScreenPosition - MtVector2(v3Cross.x, v3Cross.y);
                    m_isFirst = BtFalse;
                }
                else
                {
                    m_vertex[0].m_v2Position = m_v2LastR;
                    m_vertex[0].m_colour = RsColour::WhiteColour().asWord();
                    m_vertex[0].m_v2UV = MtVector2(1, 0.5f),
                    
                    m_vertex[1].m_v2Position = m_v2LastL;
                    m_vertex[1].m_colour = RsColour::WhiteColour().asWord();
                    m_vertex[1].m_v2UV = MtVector2(0, 0.5f),
                    
                    m_vertex[2].m_v2Position = v2ScreenPosition + MtVector2(v3Cross.x, v3Cross.y);
                    m_vertex[2].m_colour = RsColour::WhiteColour().asWord();
                    m_vertex[2].m_v2UV = MtVector2(1, 0.5f),
                    
                    m_vertex[3].m_v2Position = v2ScreenPosition - MtVector2(v3Cross.x, v3Cross.y);
                    m_vertex[3].m_colour = RsColour::WhiteColour().asWord();
                    m_vertex[3].m_v2UV = MtVector2(0, 0.5f),

                    m_v2LastR = v2ScreenPosition + MtVector2(v3Cross.x, v3Cross.y);
                    m_v2LastL = v2ScreenPosition - MtVector2(v3Cross.x, v3Cross.y);
                    
                    m_isRender = BtTrue;
                }
            }
        }
        else if(ShTouch::IsReleased(i))
        {
            m_isFirst = BtTrue;
            m_v2Last  = v2Position;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// PreRender

void TsPaint::PreRender()
{
    SetupRenderToTexture();
    
    if( m_isRender )
    {
       m_pWhite2->Render(RsPT_TriangleStrip, m_vertex, 4, MaxSortOrders - 1);
    }
}

////////////////////////////////////////////////////////////////////////////////
// IsPainting

BtBool TsPaint::IsPainting()
{
    return m_isPainting;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void TsPaint::Render()
{
    MtVector2 v2Dimension = RsUtil::GetDimension();
	m_pRenderTarget->Render( MtVector2(0, 0), v2Dimension, RsColour::WhiteColour(), MaxSortOrders - 1);
}
