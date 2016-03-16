////////////////////////////////////////////////////////////////////////////////
// SbSprite.h

// Include guard
#pragma once
#include "BtTypes.h"

// Class definition
class ImageLibrary
{
public:

	// Public functions
    static BtU64						   CreateImage( BtU32 width, BtU32 height );
    static void                            DeleteImage( BtU64 source );
    
    static BtU64                           OpenImage( const BtChar *filename, BtBool isAlpha );
    static BtU8*						   GetPixels( BtU64 source );
    static void							   Resize( BtU64 source, BtU64 dest );
	static BtU32						   GetWidth(BtU64 source);
	static BtU32						   GetHeight(BtU64 source);
    
	static BtU32						   GetNumChannels(BtU64 source);
	static void							   FlipX(BtU32 source);
	static void							   FlipY(BtU32 source);
	static void							   Close( BtU32 handle );
	static void							   Save( BtU32 handle, BtChar *filename );
	static void							   Copy( BtU32 source, BtU32 x, BtU32 y, BtU32 width, BtU32 height, BtU32 dest );
	static void							   Squish( const BtChar *source, const BtChar *dest );

	// Accessors

private:

	// Private functions

	// Private members
};
