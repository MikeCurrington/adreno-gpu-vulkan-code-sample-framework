// Copyright (c) 2021, Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

/// @file osxMain.cpp
/// @brief OSX 'main' entry point and event handler.
/// 
/// Implements OSX specific wrapping of frameworkApplicationBase.
//  There should be the minimum (possible) amount of code in here.

#include "system/os_common.h"
#include "vulkan/vulkan.hpp"

#define GLFW_INCLUDE_NONE (1)
#include <GLFW/glfw3.h>

#include "main/frameworkApplicationBase.hpp"
#include "main/applicationEntrypoint.hpp"
#include "gui/gui.hpp"

// Globals
static std::unique_ptr<FrameworkApplicationBase> gpApplication;

// Forward declarations
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main(int argc, const char*argv[])
{
    const char* pAppName = "vkSampleFramework";

    // Create the application
    gpApplication.reset( Application_ConstructApplication() );

    // Load the config file
    // Need this here in order to get the window sizes
    gpApplication->LoadConfigFile();

    if(!glfwInit()){
        fprintf(stderr, "glfwInit failed\n");
        return -1;
    }

    if (!glfwVulkanSupported())
    {
        fprintf(stderr, "glfwVulkanSupported failed\n");
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* pWindow = glfwCreateWindow(gSurfaceWidth, gSurfaceHeight, pAppName, NULL, NULL);

    if(!pWindow)
    {
        fprintf(stderr, "glfwCreateWindow failed\n");
        glfwTerminate();
        return -1;
    }

    // Initialize some Vulkan stuff
    int iDesiredMSAA = 4;   // 0 = off, -1 = best available, [2|4|8|16] = specified
    if (!gpApplication->GetVulkan()->Init( (uintptr_t)pWindow,
                                           (uintptr_t)0,
                                           iDesiredMSAA,
                                           [](tcb::span<const VkSurfaceFormatKHR> x) { return gpApplication->PreInitializeSelectSurfaceFormat(x); },
                                           [](Vulkan::AppConfiguration& x) { return gpApplication->PreInitializeSetVulkanConfiguration(x); }))
    {
        LOGE("Unable to initialize Vulkan!!");
        return -1;
    }

    if (!gpApplication->Initialize((uintptr_t)pWindow))
    {
        LOGE("Unable to initialize Application!!");
        return -1;
    }

    glfwSetKeyCallback(pWindow, KeyCallback);

    glfwMakeContextCurrent(pWindow);
    while(!glfwWindowShouldClose(pWindow)){
        if (gpApplication)
        {
            gpApplication->Render();
        }
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    // Release the application
    gpApplication->Destroy();
    gpApplication.reset();

    // Shudown glfw
    glfwDestroyWindow(pWindow);
    pWindow = nullptr;
    glfwTerminate();

    return 0;
}

// GLFW keyboard input callback
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch(action) {
        case GLFW_PRESS:
            gpApplication->KeyDownEvent(key);
            break;
        case GLFW_REPEAT:
            gpApplication->KeyRepeatEvent(key);
            break;
        case GLFW_RELEASE:
        default:
            gpApplication->KeyUpEvent(key);
            break;
    }
}

