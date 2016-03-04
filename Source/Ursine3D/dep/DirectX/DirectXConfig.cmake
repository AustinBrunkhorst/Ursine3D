# request D3D11
set(PACKAGE_FIND_VERSION_MAJOR 11)

find_package(DirectX)

if (NOT DirectX_D3D11_FOUND)
    message(FATAL_ERROR "Couldn't find DirectX11 dependency")
endif ()

# ------------------------------------------------------------------------------
# Directory Configurations
# ------------------------------------------------------------------------------

set(DIR_FILE ${CMAKE_CURRENT_LIST_DIR})

set(DIR_LIB ${DIR_FILE}/lib/${PLATFORM_NAME})

# ------------------------------------------------------------------------------
# Package Configuration
# ------------------------------------------------------------------------------

# DirectX graphics backend
add_definitions(-DURSINE_GRAPHICS_DIRECTX)

set(DirectX_INCLUDE_DIRS 
	${DirectX_D3D11_INCLUDE_DIR}
	${DIR_FILE}/include)

set(LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(DirectX_STATIC_LIBS 
    d3d11${LIB_EXT}
    DXGI${LIB_EXT}
    dxguid${LIB_EXT}
    D3Dcompiler${LIB_EXT}
    debug ${DIR_LIB}/Debug/DirectXTex${LIB_EXT}
    optimized ${DIR_LIB}/Release/DirectXTex${LIB_EXT}
)