
///////////////////////////////////////////////////////////////////////////////
// SgAnimatorWinDX.h

// Includes
#include "BtTypes.h"
#include "BtTime.h"
#include "SgAnimatorGLES.h"
#include "SgNodeGLES.h"
#include "SgBoneGLES.h"
#include "SgSkinGLES.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor

SgAnimatorWinGL::SgAnimatorWinGL()
{
	m_bSetup = BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// pDuplicate

SgAnimator *SgAnimatorWinGL::pDuplicate()
{
	// Allocate the memory
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Remove

void SgAnimatorWinGL::Remove()
{
}

///////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgAnimatorWinGL::FixPointers( BtU8* pFileData, BaArchive* pArchive )
{
	// Set the file data
	m_pFileData = (BaSgAnimationFileData*) pFileData;

	BtU8* pCurrent = (BtU8*) m_pFileData;

	// Skip the animation file data
	pCurrent+= sizeof(BaSgAnimationFileData);

	// Set the animation blocks
	m_pTimes = (BaSgAnimationTimeFileData*) pCurrent;

	// Skip the animation file data
	pCurrent+= sizeof(BaSgAnimationTimeFileData) * m_pFileData->m_maxFrames;
	
	// Set the keys
	m_pTransforms = (BaSgAnimationTransformFileData*) pCurrent;

	int a=0;
	a++;
}

///////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void SgAnimatorWinGL::CreateOnDevice()
{
}

///////////////////////////////////////////////////////////////////////////////
// Reset

void SgAnimatorWinGL::Reset()
{
	m_time = 0;
	m_bEnded  = BtFalse;
	m_bPaused = BtFalse;
}

void SgAnimatorWinGL::Pause( BtBool bPause )
{
	m_bPaused = bPause;
}

BtBool SgAnimatorWinGL::IsEnded() const
{
	return m_bEnded;
}

BtBool SgAnimatorWinGL::IsPaused() const
{
	return m_bPaused;
}

///////////////////////////////////////////////////////////////////////////////
// GetDuration

BtFloat SgAnimatorWinGL::GetDuration()
{
	return (BtFloat)m_pFileData->m_duration;
}

///////////////////////////////////////////////////////////////////////////////
// GetNumFrames

BtU32 SgAnimatorWinGL::GetNumFrames()
{
	return m_pFileData->m_maxFrames;
}

///////////////////////////////////////////////////////////////////////////////
// SetTime

void SgAnimatorWinGL::SetTime( BtFloat animTime )
{
	m_time = animTime;
}

///////////////////////////////////////////////////////////////////////////////
// GetTime

BtFloat SgAnimatorWinGL::GetTime()
{
	return m_time;
}

///////////////////////////////////////////////////////////////////////////////
// SetFrame

void SgAnimatorWinGL::SetFrame( BtU32 frame )
{
	m_time = ((BtFloat)frame) / m_pFileData->m_frameRate;
}

///////////////////////////////////////////////////////////////////////////////
// Animate

void SgAnimatorWinGL::Update( SgSkin* pSkin )
{
	if( m_bPaused == BtTrue )
	{
		return;
	}

	// Cache the windows skin
	SgSkinWin32GL *pSkinWinGL = (SgSkinWin32GL*)pSkin;

	BtU32 frame = (BtU32)( m_time * m_pFileData->m_frameRate );

	// Is this frame valid
	if( frame < m_pFileData->m_maxFrames )
	{
        // Cache each time
        const BaSgAnimationTimeFileData& time = m_pTimes[frame];
        
        for( BtU32 iTransform=time.m_iTransformStart; iTransform<time.m_iTransformEnd; iTransform++ )
        {
            // Cache each transform
            const BaSgAnimationTransformFileData& transform = m_pTransforms[iTransform];
            
            // Do it the fast way
            SgNode* pNode = (SgNode*)pSkinWinGL->m_pFileData->m_skeleton[transform.m_iBone].m_pJoint;
            pNode->SetLocalTransform(transform.m_m4Transform);
        }
	}
	else
	{
		// Flag the animation as ended
		m_bEnded = BtTrue;
	}
}
