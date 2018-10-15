# Microsoft Developer Studio Project File - Name="Kernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Kernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Kernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Kernel.mak" CFG="Kernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Kernel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Kernel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Aerofoil/Windows", XABAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Kernel - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Kernel - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Kernel - Win32 Release"
# Name "Kernel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Aerofoil.cpp
# End Source File
# Begin Source File

SOURCE=.\Cutpath.cpp
# End Source File
# Begin Source File

SOURCE=.\Drivers\DXFOutputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Drivers\DXFOutputDevice.h
# End Source File
# Begin Source File

SOURCE=.\Ellipse.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipseFlags.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipsePair.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipsePlotter.cpp
# End Source File
# Begin Source File

SOURCE=.\KernelError.cpp
# End Source File
# Begin Source File

SOURCE=.\Drivers\LASERJET.CPP
# End Source File
# Begin Source File

SOURCE=.\Drivers\LASERJET.H
# End Source File
# Begin Source File

SOURCE=.\ObjectSerializer.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotCommonImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotFlags.cpp
# End Source File
# Begin Source File

SOURCE=.\Plotfoil.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\PointPlotter.cpp
# End Source File
# Begin Source File

SOURCE=.\PointStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\PointT.cpp
# End Source File
# Begin Source File

SOURCE=.\Drivers\Postscript.cpp
# End Source File
# Begin Source File

SOURCE=.\RectT.cpp
# End Source File
# Begin Source File

SOURCE=.\Spar.cpp
# End Source File
# Begin Source File

SOURCE=.\Spline.cpp
# End Source File
# Begin Source File

SOURCE=.\Structure.cpp
# End Source File
# Begin Source File

SOURCE=.\Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\UIProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Wing.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Aerofoil.h
# End Source File
# Begin Source File

SOURCE=.\Cutpath.h
# End Source File
# Begin Source File

SOURCE=.\Ellipse.h
# End Source File
# Begin Source File

SOURCE=.\EllipseFlags.h
# End Source File
# Begin Source File

SOURCE=.\EllipsePair.h
# End Source File
# Begin Source File

SOURCE=.\EllipsePlotter.h
# End Source File
# Begin Source File

SOURCE=.\KernelError.h
# End Source File
# Begin Source File

SOURCE=.\ObjectSerializer.h
# End Source File
# Begin Source File

SOURCE=.\OutputDevice.h
# End Source File
# Begin Source File

SOURCE=.\ParametricCurve2D.h
# End Source File
# Begin Source File

SOURCE=.\Plot.h
# End Source File
# Begin Source File

SOURCE=.\PlotCommonImpl.h
# End Source File
# Begin Source File

SOURCE=.\PlotFlags.h
# End Source File
# Begin Source File

SOURCE=.\Plotfoil.h
# End Source File
# Begin Source File

SOURCE=.\PlotStructure.h
# End Source File
# Begin Source File

SOURCE=.\PointPlotter.h
# End Source File
# Begin Source File

SOURCE=.\PointStructure.h
# End Source File
# Begin Source File

SOURCE=.\PointT.h
# End Source File
# Begin Source File

SOURCE=.\Drivers\Postscript.h
# End Source File
# Begin Source File

SOURCE=.\RectT.h
# End Source File
# Begin Source File

SOURCE=.\Spar.h
# End Source File
# Begin Source File

SOURCE=.\Spline.h
# End Source File
# Begin Source File

SOURCE=.\Structure.h
# End Source File
# Begin Source File

SOURCE=.\Transform.h
# End Source File
# Begin Source File

SOURCE=.\UIProxy.h
# End Source File
# Begin Source File

SOURCE=.\Wing.h
# End Source File
# End Group
# End Target
# End Project
