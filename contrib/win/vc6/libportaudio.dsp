# Microsoft Developer Studio Project File - Name="libportaudio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libportaudio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libportaudio.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libportaudio.mak" CFG="libportaudio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libportaudio - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libportaudio - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libportaudio"
# PROP Scc_LocalPath "..\..\..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libportaudio - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\libportaudio"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\portaudio\include" /I "..\..\..\..\portaudio\src\common" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "PA_NO_DS" /D "PA_NO_ASIO" /D ALLOW_SMP_DANGERS=1 /D _CRT_SECURE_NO_WARNINGS=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libportaudio - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\libportaudio"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\portaudio\include" /I "..\..\..\..\portaudio\src\common" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "PA_NO_DS" /D "PA_NO_ASIO" /D ALLOW_SMP_DANGERS=1 /D _CRT_SECURE_NO_WARNINGS=1 /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libportaudio - Win32 Release"
# Name "libportaudio - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_allocation.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_converters.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_cpuload.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_dither.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_front.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_process.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_skeleton.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_stream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_trace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\os\win\pa_win_hostapis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\os\win\pa_win_util.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\os\win\pa_win_waveformat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\hostapi\wmme\pa_win_wmme.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\os\win\pa_x86_plain_converters.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_allocation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\pa_asio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_converters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_cpuload.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_dither.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_endianness.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_hostapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\pa_linux_alsa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\pa_mac_core.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_process.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_trace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\common\pa_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\pa_win_waveformat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\pa_win_wmme.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\src\os\win\pa_x86_plain_converters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portaudio\include\portaudio.h
# End Source File
# End Group
# End Target
# End Project
