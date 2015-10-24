#include "UrsinePrecompiled.h"

#include "DXErrorHandling.h"

#define hde(code) \
case code:\
return #code;

const char* ursine::graphics::DXCore::GetDXErrorMessage(int code)
{
    //other I may have to think about
    /*
        DXGI_STATUS_OCCLUDED
        DXGI_STATUS_CLIPPED
        DXGI_STATUS_NO_REDIRECTION
        DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
        DXGI_STATUS_MODE_CHANGED
        DXGI_STATUS_MODE_CHANGE_IN_PROGRESS

        DXGI_STATUS_UNOCCLUDED
        DXGI_STATUS_DDA_WAS_STILL_DRAWING
        
        
            */


    switch(code)
    {
        //directx error codes
        hde(DXGI_ERROR_MODE_CHANGE_IN_PROGRESS)
        hde(DXGI_STATUS_NO_DESKTOP_ACCESS);
        hde(DXGI_ERROR_DEVICE_HUNG);
        hde(DXGI_ERROR_DEVICE_REMOVED);
        hde(DXGI_ERROR_DEVICE_RESET);
        hde(DXGI_ERROR_DRIVER_INTERNAL_ERROR);
        hde(DXGI_ERROR_FRAME_STATISTICS_DISJOINT);
        hde(DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE);
        hde(DXGI_ERROR_INVALID_CALL);
        hde(DXGI_ERROR_MORE_DATA);
        hde(DXGI_ERROR_NONEXCLUSIVE);
        hde(DXGI_ERROR_NOT_CURRENTLY_AVAILABLE);
        hde(DXGI_ERROR_NOT_FOUND);
        hde(DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED);
        hde(DXGI_ERROR_REMOTE_OUTOFMEMORY);
        hde(DXGI_ERROR_WAS_STILL_DRAWING);
        hde(DXGI_ERROR_UNSUPPORTED);
        hde(DXGI_ERROR_ACCESS_LOST);
        hde(DXGI_ERROR_WAIT_TIMEOUT);
        hde(DXGI_ERROR_SESSION_DISCONNECTED);
        hde(DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE);
        hde(DXGI_ERROR_CANNOT_PROTECT_CONTENT);
        hde(DXGI_ERROR_ACCESS_DENIED);
        hde(DXGI_ERROR_NAME_ALREADY_EXISTS);
        hde(DXGI_ERROR_SDK_COMPONENT_MISSING);

        //other error codes
        hde(E_ABORT);
        hde(E_ACCESSDENIED);
        hde(E_FAIL);
        hde(E_HANDLE);
        hde(E_INVALIDARG);
        hde(E_NOINTERFACE);
        hde(E_NOTIMPL);
        hde(E_OUTOFMEMORY);
        hde(E_POINTER);
        hde(E_UNEXPECTED);
        hde(S_OK);
    default:
        return "UNKNOWN ERROR";
    }
}