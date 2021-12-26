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

int main(int argc, const char*argv[])
{
    const char* pAppName = "vkSampleFramework";

    // Create the application
    std::unique_ptr<FrameworkApplicationBase> gpApplication( Application_ConstructApplication() );

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
                                           [&gpApplication](tcb::span<const VkSurfaceFormatKHR> x) { return gpApplication->PreInitializeSelectSurfaceFormat(x); },
                                           [&gpApplication](Vulkan::AppConfiguration& x) { return gpApplication->PreInitializeSetVulkanConfiguration(x); }))
    {
        LOGE("Unable to initialize Vulkan!!");
        return -1;
    }

    if (!gpApplication->Initialize((uintptr_t)pWindow))
    {
        LOGE("Unable to initialize Application!!");
        return -1;
    }


    glfwMakeContextCurrent(pWindow);
    while(!glfwWindowShouldClose(pWindow)){
        //glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
    glfwTerminate();

    // Release the application
    if (gpApplication)
    {
        gpApplication->Destroy();
        // gpApplication->DestroyVulkanWindow();

        gpApplication.reset();
    }

    return 0;
}
