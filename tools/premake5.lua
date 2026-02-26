--
-- premake5.lua
--
-- Premake solution to build the EDK project.
-- Ivan Sancho as Unreal Authorized Instructor, 2023/24
-- Graphic Programming
--
-- All rights are reserved.
--
--

PROJ_DIR = path.getabsolute("./../")

project_names = {
  "PR03_00_Template",
  "PR03_01_Demo",
  
  "PR03_02_FirstScene",
  "PR03_03_SolarSystem",
  "PR03_04_BasicMaterial",
  "PR03_05_CustomMaterial",
  "PR03_06_Quad",
  --"PR03_07_Cube",
  --"PR03_08_Sphere",
  --"PR03_09_Surface",
  --"PR03_10_Camera",
  --"PR03_11_Terrain",
  --"PR03_12_Transparencies",
  --"PR03_13_DemoRenderTarget",
  --"PR03_14_BasicPostprocess",
}

-- Solution workspace declaration:
solution("EDK_Workspace_" .. _ACTION)
  location(PROJ_DIR .. "/build/")
  language "C++"
  kind "ConsoleApp"
  startproject "PR03_01_Demo"
  platforms {
    "x32",
    "x64",
    "Native",
  }
  defines {
    "_CRT_SECURE_NO_WARNINGS",
  }
  configurations {
    "Debug",
    "Release",
  }

-- Workspace "Debug" configuration:
filter { "configurations:Debug" }
  defines {
    "DEBUG",
  }
  symbols "On"
  warnings "Extra"
  targetsuffix "_d"

-- Workspace "Release" configuration:
filter { "configurations:Release" }
  defines {
    "NDEBUG",
  }
  symbols "Off"
  warnings "Extra"
  optimize "On"
  optimize "Size"
  flags {
    "No64BitChecks",
    "NoPCH",
  }

--[[********************** PROJECT GENERATOR FUNCTION **********************]]--
function GenerateProject(project_name, project_group)
  project(project_name)
    location (PROJ_DIR .. "/build/" .. project_name .. "/" .. _ACTION)
    includedirs {
      path.join(PROJ_DIR, "./include/"),
      path.join(PROJ_DIR, "./include/dev"),
      path.join(PROJ_DIR, "./deps/EDK/include/"),
      path.join(PROJ_DIR, "./deps/Perlin/include/"),
      path.join(PROJ_DIR, "./deps/Perlin/src/"),
      path.join(PROJ_DIR, "./deps/STB/include/"),
    }
    files {
      path.join(PROJ_DIR, "./include/*.h"),
      path.join(PROJ_DIR, "./include/dev/*.h"),
      path.join(PROJ_DIR, "./src/*.cc"),
      path.join(PROJ_DIR, "./src/dev/*.cc"),
      path.join(PROJ_DIR, "./deps/EDK/include/EDK/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/EDK/dev/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/ESAT/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/ESAT_extra/*.h"),
      path.join(PROJ_DIR, "./deps/Perlin/include/Perlin/*.h"),
      path.join(PROJ_DIR, "./deps/Perlin/src/*.cpp"),
      path.join(PROJ_DIR, "./deps/STB/include/*.h"),
    }

    --Debug filters:
    filter { "configurations:Debug", "platforms:x32" }
      architecture "x86"
      targetdir (path.join(PROJ_DIR, "bin/Debug/x32"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Debug"))
      libdirs { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32"), }
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/EDK_d"),
              "OpenGL32",
              "winmm" }
    filter { "configurations:Debug", "platforms:x64" }
      architecture "x64"
      targetdir (path.join(PROJ_DIR, "bin/Debug/x64"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Debug"))
      libdirs { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x64"), }
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x64/EDK_d"),
              "OpenGL32",
              "winmm" }
    --Release filters:
    filter { "configurations:Release", "platforms:x32" }
      architecture "x86"
      targetdir (path.join(PROJ_DIR, "bin/Release/x32/"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Release"))
      libdirs { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32"), }
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/EDK"),
              "OpenGL32",
              "winmm" }
    filter { "configurations:Release", "platforms:x64" }
      architecture "x64"
      targetdir (path.join(PROJ_DIR, "bin/Release/x64/"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Release"))
      libdirs { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x64"), }
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x64/EDK"),
              "OpenGL32",
              "winmm" }
end --Function "GenerateProject" ends here.

--Calling the Lua function:
for i, prj in ipairs(project_names) do
  GenerateProject(prj)
  --Specific file per project:
  project(prj)
  files {
    path.join(PROJ_DIR, "./tests/" .. prj .. ".cc")
  }
end

--[[
--For more specific configuration in a project, do:
project "PR03_00_Template"
	files {
	  path.join(PROJ_DIR, "./tests" .. PR03_00_Template.cc"),
  }
]]--
