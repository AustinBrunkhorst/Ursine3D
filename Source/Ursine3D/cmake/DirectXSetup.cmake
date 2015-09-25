# request D3D11
set(PACKAGE_FIND_VERSION_MAJOR 11)

find_package(DirectX)

if (NOT DirectX_D3D11_FOUND)
    message(FATAL_ERROR "Couldn't find DirectX11 dependency")
endif ()

set(DirectX_INCLUDE_DIRS ${DirectX_D3D11_INCLUDE_DIR})

set(DirectX_STATIC_LIBS 
    d3d11.lib
    DXGI.lib
    dxguid.lib
    D3Dcompiler.lib
)