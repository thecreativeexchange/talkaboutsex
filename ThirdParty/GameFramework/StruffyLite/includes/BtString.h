////////////////////////////////////////////////////////////////////////////////
// BtString.h

#pragma once
#include "BtTypes.h"
#include "BtMemory.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
// BtStrEmpty

void BtStrEmpty( BtChar* pString, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrLower

void BtStrLower( BtChar* pString, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrUpper

void BtStrUpper( BtChar* pString, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrLength

BtU32 BtStrLength( const BtChar* pString );

////////////////////////////////////////////////////////////////////////////////
// BtStrCompare

BtBool BtStrCompare( const BtChar* pString1, const BtChar* pString2 );

BtBool BtStrCompare( const BtChar* pString1, const BtChar* pString2, BtU32 length );

BtBool BtStrCompareNoCase( const BtChar* pString1, const BtChar* pString2 );

////////////////////////////////////////////////////////////////////////////////
// BtStrCopy

void BtStrCopy( BtChar* szDestination, BtU32 nDestSize, const BtChar* szSource );

////////////////////////////////////////////////////////////////////////////////
// BtStrCopy

void BtStrCopy( BtChar* szDestination, BtU32 nDestSize, const BtChar* szSource, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrTrimNewLines

void BtStrTrimNewLines( BtChar* szSource, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrTrimSpaces

void BtStrTrimSpaces( BtChar* szSource, BtU32 nSize );

////////////////////////////////////////////////////////////////////////////////
// BtStrCat

void BtStrCat( BtChar* pDestation, BtU32 nDestSize, const BtChar* pSource );

////////////////////////////////////////////////////////////////////////////////
// BtStrCatCharacter

void BtStrCatCharacter( BtChar* pDestation, BtU32 nDestSize, BtChar source );

////////////////////////////////////////////////////////////////////////////////
// BtStrStr

BtChar* BtStrStr( const BtChar* pSource, const BtChar* pToFind );

////////////////////////////////////////////////////////////////////////////////
// BtStringReplace

BtChar* BtStringReplace( const BtChar* szSource,
						BtChar* szDest,
						BtU32 nDestSize,
						const BtChar* szSearchString,
						const BtChar *szReplaceString );
