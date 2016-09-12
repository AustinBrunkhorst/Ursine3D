#include "GlobalShaderResource.h"
#pragma once

template <typename T, BUFFER_LIST BufferType>
inline GlobalCBuffer<T, BufferType>::GlobalCBuffer(
    SHADERTYPE shader,
    ShaderSlot shaderSlotIndex
)
    : GlobalShaderResource(shaderSlotIndex, RESOURCE_CBUFFER, shader)
{
}

template<typename T, BUFFER_LIST BufferType>
inline ursine::graphics::GlobalCBuffer<T, BufferType>::~GlobalCBuffer(void)
{
}

template <typename T, BUFFER_LIST BufferType>
inline void GlobalCBuffer<T, BufferType>::Update(const T& data, ShaderSlot slotIndex)
{
    m_bufferData = data;

    if(slotIndex != static_cast<ShaderSlot>(14))
        m_slotIndex = static_cast<ShaderSlot>(slotIndex);
}

template <typename T, BUFFER_LIST BufferType>
inline void GlobalCBuffer<T, BufferType>::Map()
{
    if( m_slotIndex == static_cast<ShaderSlot>( 14 ) )
        m_manager->bufferManager->MapBuffer<BufferType>(&m_bufferData, m_targetShader, BufferType);
    else
        m_manager->bufferManager->MapBuffer<BufferType>( &m_bufferData, m_targetShader, m_slotIndex );
}