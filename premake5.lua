workspace "Directx11Renderer"
    configurations { "Debug", "Release" }
    startproject "Directx11Renderer"

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }


include "Libraries/glfw.lua"
include "Directx11Renderer/Directx11Renderer.lua"