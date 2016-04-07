!include FileFunc.nsh
!include LogicLib.nsh
!include MUI2.nsh
!include InstallOptions.nsh

SetCompressor lzma

# -----------------------------------------------------------------------------
# General Configuration
# -----------------------------------------------------------------------------

!define CONFIG_BUILD_DIRECTORY "{{configBuildDirectory}}"
!define CONFIG_APPNAME "{{configGameName}}"
!define CONFIG_PUBLISHER "{{configPublisher}}"
!define CONFIG_DISPLAYNAME "{{configDisplayName}}"
!define CONFIG_BRANDING_TEXT "{{configBrandingText}}"
!define CONFIG_VERSION_MAJOR {{configVersionMajor}}
!define CONFIG_VERSION_MINOR {{configVersionMinor}}
!define CONFIG_VERSION_BUILD {{configVersionBuild}}
!define CONFIG_VERSION_DISPLAYNAME "{{configVersionDisplayName}}"
!define CONFIG_HELPURL ""
!define CONFIG_UPDATEURL ""
!define CONFIG_ABOUTURL ""
!define CONFIG_DEFAULT_STARTMENU_FOLDER "{{configDefaultStartMenu}}"
!define CONFIG_LICENSE_FILE "License.txt"
!define CONFIG_ICON_FILE "Icon.ico"
!define CONFIG_INSTALL_OPTIONS_FILE "InstallOptions.ini"
!define CONFIG_EXECUTABLE_FILE "{{executableFile}}"
!define CONFIG_OUTPUT_INSTALLER "{{outputInstaller}}"
!define CONFIG_UNINSTALLER_FILE "uninstall.exe"

# -----------------------------------------------------------------------------
# GUI Configuration
# -----------------------------------------------------------------------------

!define MUI_ABORTWARNING
!define MUI_ICON "${CONFIG_ICON_FILE}"
!define MUI_UNICON "${CONFIG_ICON_FILE}"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${CONFIG_DEFAULT_STARTMENU_FOLDER}"
!define MUI_FINISHPAGE_RUN "$INSTDIR\${CONFIG_EXECUTABLE_FILE}"

# -----------------------------------------------------------------------------
# GUI Variables
# -----------------------------------------------------------------------------

Var STARTMENU_FOLDER
Var INSTALL_DESKTOP

# -----------------------------------------------------------------------------
# Page Configuration
# -----------------------------------------------------------------------------

# Installer Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${CONFIG_LICENSE_FILE}
Page custom InstallOptionsPage
!insertmacro MUI_PAGE_DIRECTORY

!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

# Un-Installer Pages

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

# -----------------------------------------------------------------------------
# Installer Setup
# -----------------------------------------------------------------------------

!define UNINSTALL_REGISTRY_FOLDER "Software\Microsoft\Windows\CurrentVersion\Uninstall\${CONFIG_PUBLISHER} ${CONFIG_APPNAME}"

# Require admin rights on NT6+ (When UAC is turned on)
RequestExecutionLevel admin 

InstallDir "$PROGRAMFILES\${CONFIG_PUBLISHER}\${CONFIG_APPNAME}"
Name "${CONFIG_DISPLAYNAME}"
OutFile "${CONFIG_OUTPUT_INSTALLER}"
BrandingText "${CONFIG_BRANDING_TEXT}"

# -----------------------------------------------------------------------------
# Install Functions
# -----------------------------------------------------------------------------

Function InstallOptionsPage
	!insertmacro MUI_HEADER_TEXT "Install Options" "Choose installation options"
	!insertmacro INSTALLOPTIONS_DISPLAY "${CONFIG_INSTALL_OPTIONS_FILE}"
FunctionEnd
 
Function .onInit
	SetShellVarContext all
	!insertmacro INSTALLOPTIONS_EXTRACT "${CONFIG_INSTALL_OPTIONS_FILE}"
FunctionEnd

Section "install"
	SetOutPath $INSTDIR

	# copy icon file
	File "${CONFIG_ICON_FILE}"

	# copy all files in the build directory
	File /r "${CONFIG_BUILD_DIRECTORY}\*.*"

	WriteUninstaller "$INSTDIR\${CONFIG_UNINSTALLER_FILE}"

	WriteRegStr HKLM "Software\${CONFIG_PUBLISHER}\${CONFIG_APPNAME}" "StartMenuFolder" "$STARTMENU_FOLDER"
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "DisplayName" "${CONFIG_DISPLAYNAME}"
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "UninstallString" "$\"$INSTDIR\${CONFIG_UNINSTALLER_FILE}$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "QuietUninstallString" "$\"$INSTDIR\${CONFIG_UNINSTALLER_FILE}$\" /S"
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "DisplayIcon" "$\"$INSTDIR\${CONFIG_ICON_FILE}$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "Publisher" "${CONFIG_PUBLISHER}"
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "HelpLink" "$\"${CONFIG_HELPURL}$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "URLUpdateInfo" "$\"${CONFIG_UPDATEURL}$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "URLInfoAbout" "$\"${CONFIG_ABOUTURL}$\""
	WriteRegStr HKLM "${UNINSTALL_REGISTRY_FOLDER}" "DisplayVersion" "${CONFIG_VERSION_DISPLAYNAME}"
	WriteRegDWORD HKLM "${UNINSTALL_REGISTRY_FOLDER}" "VersionMajor" ${CONFIG_VERSION_MAJOR}
	WriteRegDWORD HKLM "${UNINSTALL_REGISTRY_FOLDER}" "VersionMinor" ${CONFIG_VERSION_MINOR}
	WriteRegDWORD HKLM "${UNINSTALL_REGISTRY_FOLDER}" "NoModify" 1
	WriteRegDWORD HKLM "${UNINSTALL_REGISTRY_FOLDER}" "NoRepair" 1

	# determine the size of the install directory
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
 	IntFmt $0 "0x%08X" $0

	WriteRegDWORD HKLM "${UNINSTALL_REGISTRY_FOLDER}" "EstimatedSize" $0

	!insertmacro INSTALLOPTIONS_READ $INSTALL_DESKTOP "${CONFIG_INSTALL_OPTIONS_FILE}" "Field 1" "State"

	${If} $INSTALL_DESKTOP == 1
	    CreateShortCut "$DESKTOP\${CONFIG_DISPLAYNAME}.lnk" "$INSTDIR\${CONFIG_EXECUTABLE_FILE}"
	${EndIf}

	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application

	CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
	CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${CONFIG_DISPLAYNAME}.lnk" "$INSTDIR\${CONFIG_EXECUTABLE_FILE}"

	!insertmacro MUI_STARTMENU_WRITE_END

	ExecWait '"$INSTDIR\redist\vcredist140_x64.exe" /q /norestart'
	ExecWait '"$INSTDIR\redist\d3dredist_x64.exe"'
	RMDir /r "$INSTDIR\redist"
SectionEnd
 
# -----------------------------------------------------------------------------
# Uninstall functions
# -----------------------------------------------------------------------------
 
Function un.onInit
	SetShellVarContext all
FunctionEnd
 
Section "uninstall"
	# remove dektop shortcut
	Delete "$DESKTOP\${CONFIG_DISPLAYNAME}.lnk"

	# get the configured start menu folder
	ReadRegStr $1 HKLM "Software\${CONFIG_PUBLISHER}\${CONFIG_APPNAME}" "StartMenuFolder"

	# remove start menu folder
	Delete "$SMPROGRAMS\$1\${CONFIG_DISPLAYNAME}.lnk"
	RMDir "$SMPROGRAMS\$1\"

	# remove icon file
	Delete "$INSTDIR\${CONFIG_ICON_FILE}"

	# remove files
	{{#buildFile}}Delete "$INSTDIR\{{& path}}"
	{{/buildFile}}

	# remove directories
	{{#buildDirectory}}RMDir "$INSTDIR\{{& path}}"
	{{/buildDirectory}}
 
	# always delete uninstaller as the last action
	Delete "$INSTDIR\${CONFIG_UNINSTALLER_FILE}"
 
	# try to remove the install directory - this will only happen if it is empty
	RMDir $INSTDIR
 
	# remove uninstaller information from the registry
	DeleteRegKey HKLM "${UNINSTALL_REGISTRY_FOLDER}"
SectionEnd