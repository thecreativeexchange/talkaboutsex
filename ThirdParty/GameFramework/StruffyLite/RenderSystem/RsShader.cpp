////////////////////////////////////////////////////////////////////////////////
// RsShader.cpp

// Includes
#include "RsShader.h"

////////////////////////////////////////////////////////////////////////////////
// Statics

RsShader *RsShader::m_pCurrentShader = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Apply

void RsShader::Apply()
{
	m_pCurrentShader = this;
}

////////////////////////////////////////////////////////////////////////////////
// GetCurrent

RsShader *RsShader::GetCurrent()
{
	return m_pCurrentShader;
}
