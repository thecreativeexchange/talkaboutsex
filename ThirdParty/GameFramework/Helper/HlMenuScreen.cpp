////////////////////////////////////////////////////////////////////////////////
// HlMenuScreen

// Includes
#include "HlMenuScreen.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "ApConfig.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "HlTimer.h"
#include "BaArchive.h"
#include "ErrorLog.h"
#include "FsFile.h"
#include "BaUserData.h"
#include "MtVector2.h"
#include "HlScreenSize.h"
#include "UiKeyboard.h"
#include "RsRenderTarget.h"
#include "ImageLibrary.h"
#include "ApConfig.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlMenuScreen::Setup( BaArchive *pArchive, const BtChar *pTargetMaterial )
{
    m_pMaterial = pArchive->GetMaterial( pTargetMaterial );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlMenuScreen::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlMenuScreen::Render( const BtChar *pngName )
{
    if( strcmp( pngName, m_lastName ) != 0 )
    {
        strcpy( m_lastName, pngName );
        
        if( strstr( pngName, "kiss" ) )
        {
            int a=0;
            a++;
        }
        
        // Load the new file over our blank texture
        BtChar text[256];
        sprintf(text, "%s", ApConfig::GetResourcePath() );
        strcat( text, pngName );
        
        BtU64 originalImage = ImageLibrary::OpenImage( text, BtTrue );
        
        if( originalImage == 0 )
        {
            ErrorLog::Printf( "Could not open image %s", text );
        }
        else
        {
            BtU32 fromWidth  = ImageLibrary::GetWidth(originalImage);
            BtU32 fromHeight = ImageLibrary::GetHeight(originalImage);
            BtU32 channels   = ImageLibrary::GetNumChannels(originalImage);
            (void)fromWidth;
            (void)fromHeight;
            (void)channels;
            
            // Cache our texture
            RsTexture *pTexture = m_pMaterial->GetTexture(0);
            BtU32 width  = pTexture->GetWidth();
            BtU32 height = pTexture->GetHeight();
            
            // Create a new image
            BtU64 resizedImaged = ImageLibrary::CreateImage( width, height );
            
            BtU32 channels2 = ImageLibrary::GetNumChannels(resizedImaged);
            (void)channels2;
            
            // Resize the image
            ImageLibrary::Resize(originalImage, resizedImaged );
            
            // Get the image pixels
            BtU8 *pBytes = ImageLibrary::GetPixels( resizedImaged );
            
            // Write them to the texture
            pTexture->WriteMemory( pBytes );
            
            ImageLibrary::DeleteImage(originalImage);
            ImageLibrary::DeleteImage(resizedImaged);
        }
    }
    m_pMaterial->Render( MtVector2( 0, 0 ), RsUtil::GetDimension(), RsColour::WhiteColour(), MaxSortOrders-2 );
}
