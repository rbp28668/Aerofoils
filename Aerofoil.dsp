# Microsoft Developer Studio Project File - Name="Aerofoil" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Aerofoil - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Aerofoil.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Aerofoil.mak" CFG="Aerofoil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Aerofoil - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Aerofoil - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Aerofoil/Windows", XABAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Aerofoil - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Aerofoil - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Aerofoil - Win32 Release"
# Name "Aerofoil - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Aerofoil.cpp
# End Source File
# Begin Source File

SOURCE=.\Aerofoil.rc
# End Source File
# Begin Source File

SOURCE=.\AerofoilDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AerofoilView.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundGridDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CNCConnectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CNCConnectionOutputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\CoordMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CutterSimulationOutputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipseFlagsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipsePairDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipseUIProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PaperSizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotFlagsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotOrderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotPointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotPointUIProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectStructureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SparDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.CPP
# End Source File
# Begin Source File

SOURCE=.\WindowsOutputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsUIProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\WingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WingFlagsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WingUIProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Aerofoil.h
# End Source File
# Begin Source File

SOURCE=.\AerofoilDoc.h
# End Source File
# Begin Source File

SOURCE=.\AerofoilView.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundGrid.hpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundGridDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CNCConnectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\CNCConnectionOutputDevice.h
# End Source File
# Begin Source File

SOURCE=.\CoordMap.h
# End Source File
# Begin Source File

SOURCE=.\CutterSimulationOutputDevice.h
# End Source File
# Begin Source File

SOURCE=.\EllipseFlagsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EllipsePairDlg.h
# End Source File
# Begin Source File

SOURCE=.\EllipseUIProxy.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PaperSizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Kernel\ParametricCurve2D.h
# End Source File
# Begin Source File

SOURCE=.\PlotFlagsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotOrderDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotPointDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotPointUIProxy.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\SectionPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectStructureDlg.h
# End Source File
# Begin Source File

SOURCE=.\SparDlg.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.HPP
# End Source File
# Begin Source File

SOURCE=.\WindowsOutputDevice.h
# End Source File
# Begin Source File

SOURCE=.\WindowsUIProxy.h
# End Source File
# Begin Source File

SOURCE=.\WingDlg.h
# End Source File
# Begin Source File

SOURCE=.\WingFlagsDlg.h
# End Source File
# Begin Source File

SOURCE=.\WingUIProxy.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Aerofoil.ico
# End Source File
# Begin Source File

SOURCE=.\res\Aerofoil.rc2
# End Source File
# Begin Source File

SOURCE=.\res\AerofoilDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Aerofoil : {72ADFD7E-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DISK_SPACE:103
# 	1:19:CG_IDS_PHYSICAL_MEM:102
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:104
# 	2:14:PhysicalMemory:CG_IDS_PHYSICAL_MEM
# 	2:9:DiskSpace:CG_IDS_DISK_SPACE
# 	2:16:SpaceUnavailable:CG_IDS_DISK_SPACE_UNAVAIL
# 	2:10:SysInfoKey:1234
# End Section
# Section Aerofoil : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:105
# 	2:21:SplashScreenInsertKey:4.0
# End Section
