#include "SoundFrameClient.h"

SoundFrameClient::SoundFrameClient() : m_pSoundFrame(NULL)
{
	Create(this, &m_pSoundFrame);

	if (m_pSoundFrame)
		m_pSoundFrame->Connect();
}

SoundFrameClient::~SoundFrameClient() 
{
	if (m_pSoundFrame)
		m_pSoundFrame->Release();
}