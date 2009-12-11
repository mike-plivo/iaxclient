# Microsoft Developer Studio Project File - Name="libiaxclient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libiaxclient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libiaxclient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libiaxclient.mak" CFG="libiaxclient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libiaxclient - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libiaxclient - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libiaxclient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "i386rel"
# PROP BASE Intermediate_Dir "i386rel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\libiaxclient"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /I "..\..\..\lib\libiax2\src" /I "..\..\..\..\portaudio\include" /I "..\..\..\lib\spandsp" /I "..\..\..\lib\gsm\inc" /I "..\..\..\..\speex\include" /I "..\..\..\..\libogg\include" /I "..\..\..\lib\portmixer\px_common" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "PA_NO_DS" /D "PA_NO_ASIO" /D "BUILDING_DLL" /D "LIBIAX" /D SPEEX_EC=1 /D "ALLOW_SMP_DANGERS" /D "_CRT_SECURE_NO_DEPRECATE" /D log10f=log10 /D vsnprintf=_vsnprintf /D strncasecmp=strnicmp /D __inline__=_inline /D inline=__inline /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libiaxclient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "i386dbg"
# PROP BASE Intermediate_Dir "i386dbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\libiaxclient"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\lib\libiax2\src" /I "..\..\..\..\portaudio\include" /I "..\..\..\lib\spandsp" /I "..\..\..\lib\gsm\inc" /I "..\..\..\..\speex\include" /I "..\..\..\..\libogg\include" /I "..\..\..\lib\portmixer\px_common" /D "_DEBUG" /D "WIN32" /D "_LIB" /D "PA_NO_DS" /D "PA_NO_ASIO" /D "BUILDING_DLL" /D "LIBIAX" /D SPEEX_EC=1 /D "ALLOW_SMP_DANGERS" /D "_CRT_SECURE_NO_DEPRECATE" /D log10f=log10 /D vsnprintf=_vsnprintf /D strncasecmp=strnicmp /D __inline__=_inline /D inline=__inline /Fr /FD /GZ /c
# SUBTRACT CPP /YX
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

# Name "libiaxclient - Win32 Release"
# Name "libiaxclient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\lib\audio_encode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\audio_portaudio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_alaw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_gsm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_speex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_ulaw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\iaxclient_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\spandsp\plc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\ringbuffer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\winfuncs.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\lib\audio_alsa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\audio_portaudio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_alaw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_ffmpeg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_gsm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_ilbc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_speex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\codec_ulaw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\iaxclient.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\iaxclient_lib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\ringbuffer.h
# End Source File
# End Group
# End Target
# End Project
