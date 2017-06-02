#ifndef OBSIDIAN2D_RENDERER_VULKAN_INSTANCE_
#define OBSIDIAN2D_RENDERER_VULKAN_INSTANCE_

#include <Obsidian2D/Util/Loggable.h>
#include <vulkan/vulkan.h>

namespace Obsidian2D
{
    namespace Renderer
    {
        namespace Vulkan {
            class Instance : public Loggable
            {
            private:

                VkApplicationInfo getAppInfo()
                {
                    this->log("getAppInfo()");
                    VkApplicationInfo app_info = {};
                    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                    app_info.pNext = NULL;
                    app_info.pApplicationName = "Obsidian2D";
                    app_info.applicationVersion = 1;
                    app_info.pEngineName = "Obsidian2D";
                    app_info.engineVersion = 1;
                    app_info.apiVersion = VK_API_VERSION_1_0;
                    return app_info;
                }

                VkInstanceCreateInfo getInstanceCreateInfo(VkApplicationInfo *app_info)
                {
                    this->log("getInstanceCreateInfo()");
                    VkInstanceCreateInfo inst_info = {};
                    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                    inst_info.pNext = NULL;
                    inst_info.flags = 0;
                    inst_info.pApplicationInfo = app_info;
                    inst_info.enabledExtensionCount = 0;
                    inst_info.ppEnabledExtensionNames = NULL;
                    inst_info.enabledLayerCount = 0;
                    inst_info.ppEnabledLayerNames = NULL;
                    return inst_info;
                }

            protected:
            public:
                VkResult init()
                {
                    VkApplicationInfo app_info = getAppInfo();
                    VkInstanceCreateInfo inst_info = getInstanceCreateInfo(&app_info);

                    VkInstance inst;
                    VkResult res;
                    res = vkCreateInstance(&inst_info, NULL, &inst);
                    return res;
                }
            };
        }
    }
}
#endif //OBSIDIAN2D_RENDERER_VULKAN_INSTANCE_
