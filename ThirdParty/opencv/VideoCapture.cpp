////////////////////////////////////////////////////////////////////////////////
// ImageLibrary.cpp

// Includes
#include "BaArchive.h"
#include "VideoCapture.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsSprite.h"
#include "ErrorLog.h"
#include "BtMemory.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "BtBase.h"
#include "ApConfig.h"

using namespace cv;

// http://thefreecoder.wordpress.com/2012/09/11/opencv-c-video-capture/
Mat cameraFrame;
Mat cameraProcessed;
VideoCapture *pStream = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Start

BtU32 CVVideoCapture::Start()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
        pStream = new VideoCapture(0);   //0 is the id of video device.0 if you have only one camera.
        
        if( !pStream->isOpened() )
        {
            ErrorLog::Fatal_Printf( "Could not open camera" );
        }

		//pStream->set( CV_CAP_PROP_POS_MSEC, 300 );
		//pStream->set( CV_CAP_PROP_FRAME_WIDTH, 320 );
		//pStream->set( CV_CAP_PROP_FRAME_HEIGHT, 256 );

        cameraProcessed = Mat( Size( 640, 480 ), CV_8UC4 );
    }
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Stop

void CVVideoCapture::Stop()
{
    if( ApConfig::GetDevice() == ApDevice_WIN )
    {
		if( pStream )
		{
			delete pStream;

			pStream = BtNull;
		}
    }
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CVVideoCapture::Update()
{
    if( pStream )//ApConfig::GetDevice() == ApDevice_Win32 )
    {
        // Read a camera frame
		BtBool success = pStream->read(cameraFrame);

		if( success )
		{
	        // Copy a rectangle part of it
			cameraProcessed = cameraFrame( Rect( 0, 0, 640, 480 ) );

		    // Convert from 3 bytes per channel to 4 bytes per channel
			cvtColor( cameraFrame, cameraProcessed, CV_BGR2BGRA );
		}
		else
		{
			Stop();
		}
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetMemory

BtU8* CVVideoCapture::GetMemory()
{
	return cameraProcessed.data;
}