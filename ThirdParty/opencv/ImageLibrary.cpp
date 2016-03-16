////////////////////////////////////////////////////////////////////////////////
// ImageLibrary.cpp

// Includes
#include "BaArchive.h"
#include "ImageLibrary.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsSprite.h"
#include "ErrorLog.h"
#include "BtMemory.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "BtBase.h"
//#include "squish.h"

using namespace cv;
std::vector<char> buff;


////////////////////////////////////////////////////////////////////////////////
// OpenImage

BtU64 ImageLibrary::OpenImage( const BtChar *filename, BtBool isAlpha )
{
    Mat opencvMat;
    Mat *opencvMat2;
    
	FILE *f = fopen( filename, "rb" );
    if( f )
    {
        // Close the file
        fclose(f);
    
        // Read in the image
        opencvMat = imread(filename, CV_LOAD_IMAGE_UNCHANGED );
        
        // Create a temporary matrix
        BtU32 width = opencvMat.cols;
        BtU32 height = opencvMat.rows;
        opencvMat2 = new Mat( Size( width, height ), CV_8UC4 );
        *opencvMat2 = opencvMat;
        
        // Return the image
        return (BtU64)opencvMat2;
    }
    return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// CreateImage

BtU64 ImageLibrary::CreateImage( BtU32 width, BtU32 height )
{
    Mat *pImage = new Mat( Size( width, height ), CV_8UC4 );
    return (BtU64)pImage;
}

////////////////////////////////////////////////////////////////////////////////
// DeleteImage

void ImageLibrary::DeleteImage( BtU64 source )
{
    Mat* pSource = (Mat*)source;
    delete pSource;
}

////////////////////////////////////////////////////////////////////////////////
// GetPixels

BtU8 *ImageLibrary::GetPixels( BtU64 source )
{
    Mat* pSource = (Mat*)source;
    return pSource->data;
}

////////////////////////////////////////////////////////////////////////////////
// GetWidth

BtU32 ImageLibrary::GetWidth(BtU64 source)
{
	Mat* pSource = (Mat*)source;
	return pSource->cols;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumChannels

BtU32 ImageLibrary::GetNumChannels(BtU64 source)
{
	Mat* pSource = (Mat*)source;
	BtU32 channels = pSource->channels();
	return channels;
}

////////////////////////////////////////////////////////////////////////////////
// GetHeight

BtU32 ImageLibrary::GetHeight(BtU64 source)
{
	Mat* pSource = (Mat*)source;
	return pSource->rows;
}

////////////////////////////////////////////////////////////////////////////////
// FlipX

void ImageLibrary::FlipX(BtU32 source)
{
	Mat* pSource = (Mat*)source;
	Mat dest;
	cv::flip( *pSource, dest, 0 );
	*pSource = dest;
}

////////////////////////////////////////////////////////////////////////////////
// FlipY

void ImageLibrary::FlipY(BtU32 source)
{
	Mat* pSource = (Mat*)source;
	Mat dest;
	cv::flip(*pSource, dest, 1 );
	*pSource = dest;
}

////////////////////////////////////////////////////////////////////////////////
// Resize

void ImageLibrary::Resize( BtU64 source, BtU64 dest )
{
	Mat* pSource = (Mat*)source;
	Mat* pDest   = (Mat*)dest;
	resize( *pSource, *pDest, pDest->size(), 0, 0, INTER_LINEAR );
}

////////////////////////////////////////////////////////////////////////////////
// Copy

void ImageLibrary::Copy( BtU32 source, BtU32 x, BtU32 y, BtU32 width, BtU32 height, BtU32 dest)
{
	Mat* pSource = (Mat*)source;
	Mat* pDest   = (Mat*)dest;

	BtAssert( pDest->rows == width );
	BtAssert( pDest->cols == height );

	// Copy a rectangle part of it
	Mat m  = *pSource;
	*pDest = m( Rect( x, y, width, height ) );
}

////////////////////////////////////////////////////////////////////////////////
// Save

void ImageLibrary::Save( BtU32 handle, BtChar *filename )
{
	Mat *pImage = (Mat*)handle;
	imwrite( filename, *pImage );
}

////////////////////////////////////////////////////////////////////////////////
// Squish

void ImageLibrary::Squish( const BtChar *source, const BtChar *dest )
{
	//extern void Compress(std::string const& sourceFileName, std::string const& targetFileName, int flags);
	//Compress( source, dest, squish::kDxt1 );
}

////////////////////////////////////////////////////////////////////////////////
// Close

void ImageLibrary::Close( BtU32 handle )
{
	Mat* pSource = (Mat*)handle;
	pSource->release();
	delete pSource;
}
