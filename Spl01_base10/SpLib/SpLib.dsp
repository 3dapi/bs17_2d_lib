# Microsoft Developer Studio Project File - Name="SpLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SpLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SpLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SpLib.mak" CFG="SpLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SpLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SpLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SpLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /O2 /MD /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c /I "$(DXSDK_DIR)include"
# ADD CPP /nologo /W3 /GX /Z7 /O2 /MD /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c /I "$(DXSDK_DIR)include"
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\SpLib.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\include	copy SpLib.h ..\include
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SpLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /MDd /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c /I "$(DXSDK_DIR)include"
# ADD CPP /nologo /W3 /GX /Z7 /Od /MDd /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c /I "$(DXSDK_DIR)include"
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\SpLib_.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=mkdir ..\include	copy SpLib.h ..\include
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SpLib - Win32 Release"
# Name "SpLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\LcFont.cpp
# End Source File
# Begin Source File

SOURCE=.\LcInput.cpp
# End Source File
# Begin Source File

SOURCE=.\LcModel.cpp
# End Source File
# Begin Source File

SOURCE=.\LcSound.cpp
# End Source File
# Begin Source File

SOURCE=.\LcTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\SpLib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\ILcFont.h
# End Source File
# Begin Source File

SOURCE=.\ILcInput.h
# End Source File
# Begin Source File

SOURCE=.\ILcModel.h
# End Source File
# Begin Source File

SOURCE=.\ILcSound.h
# End Source File
# Begin Source File

SOURCE=.\ILcTexture.h
# End Source File
# Begin Source File

SOURCE=.\LcFont.h
# End Source File
# Begin Source File

SOURCE=.\LcInput.h
# End Source File
# Begin Source File

SOURCE=.\LcModel.h
# End Source File
# Begin Source File

SOURCE=.\LcSound.h
# End Source File
# Begin Source File

SOURCE=.\LcTexture.h
# End Source File
# Begin Source File

SOURCE=.\SpLib.h
# End Source File
# End Group
# End Target
# End Project
