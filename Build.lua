-- premake5.lua
workspace "ZwiftPlay"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "ZwiftPlay"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group ""

include "ZwiftPlay/Build-App.lua"
