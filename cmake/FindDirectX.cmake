# - try to find DirectX include dirs and libraries

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    find_path (DirectX_DDRAW_INCLUDE_DIR ddraw.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3d.h resides")

	find_path (DirectX_D3DX_INCLUDE_DIR d3dx.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3dx.h resides")

    find_library (DirectX_DDRAW_LIBRARY ddraw
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where ddraw resides")

	find_library (DirectX_D3DX_LIBRARY d3dx
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3dx resides")

	if (DirectX_D3D_INCLUDE_DIR AND DirectX_D3D_LIBRARY)
		set (DirectX_D3D_FOUND 1)
		if (DirectX_D3DX_INCLUDE_DIR AND DirectX_D3DX_LIBRARY)
			set (DirectX_D3DX_FOUND 1)
		endif (DirectX_D3DX_INCLUDE_DIR AND DirectX_D3DX_LIBRARY)
	endif (DirectX_D3D_INCLUDE_DIR AND DirectX_D3D_LIBRARY)


	find_path (DirectX_D3D8_INCLUDE_DIR d3d8.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3d8.h resides")

	find_path (DirectX_D3DX8_INCLUDE_DIR d3dx8.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3dx8.h resides")

	find_library (DirectX_D3D8_LIBRARY d3d8
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3d8 resides")

	find_library (DirectX_D3DX8_LIBRARY d3dx8
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3dx8 resides")

	if (DirectX_D3D8_INCLUDE_DIR AND DirectX_D3D8_LIBRARY)
		set (DirectX_D3D8_FOUND 1)
		if (DirectX_D3DX8_INCLUDE_DIR AND DirectX_D3DX8_LIBRARY)
			set (DirectX_D3DX8_FOUND 1)
		endif (DirectX_D3DX8_INCLUDE_DIR AND DirectX_D3DX8_LIBRARY)
	endif (DirectX_D3D8_INCLUDE_DIR AND DirectX_D3D8_LIBRARY)


	find_path (DirectX_D3D9_INCLUDE_DIR d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3d9.h resides")

	find_path (DirectX_D3DX9_INCLUDE_DIR d3dx9.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3dx9.h resides")

	find_library (DirectX_D3D9_LIBRARY d3d9
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3d9 resides")

	find_library (DirectX_D3DX9_LIBRARY d3dx9
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3dx9 resides")

	if (DirectX_D3D9_INCLUDE_DIR AND DirectX_D3D9_LIBRARY)
		set (DirectX_D3D9_FOUND 1)
		if (DirectX_D3DX9_INCLUDE_DIR AND DirectX_D3DX9_LIBRARY)
			set (DirectX_D3DX9_FOUND 1)
		endif (DirectX_D3DX9_INCLUDE_DIR AND DirectX_D3DX9_LIBRARY)
	endif (DirectX_D3D9_INCLUDE_DIR AND DirectX_D3D9_LIBRARY)


	find_path (DirectX_D3D10_INCLUDE_DIR d3d10.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3d10.h resides")

	find_path (DirectX_D3DX10_INCLUDE_DIR d3dx10.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Include"
		DOC "The directory where d3dx10.h resides")

	find_library (DirectX_D3D10_LIBRARY d3d10
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3d10 resides")

	find_library (DirectX_D3DX10_LIBRARY d3dx10
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3dx10 resides")

	if (DirectX_D3D10_INCLUDE_DIR AND DirectX_D3D10_LIBRARY)
		set (DirectX_D3D10_FOUND 1)
		if (DirectX_D3DX10_INCLUDE_DIR AND DirectX_D3DX10_LIBRARY)
			set (DirectX_D3DX10_FOUND 1)
		endif (DirectX_D3DX10_INCLUDE_DIR AND DirectX_D3DX10_LIBRARY)
	endif (DirectX_D3D10_INCLUDE_DIR AND DirectX_D3D10_LIBRARY)

endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

mark_as_advanced (
	DirectX_D3D8_FOUND
	DirectX_D3DX8_FOUND
	DirectX_D3D9_FOUND
	DirectX_D3DX9_FOUND
	DirectX_D3D10_FOUND
	DirectX_D3DX10_FOUND
)
