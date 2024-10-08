#ifndef OVERLAY_H
#define OVERLAY_H 1

#define GLFW_INCLUDE_VULKAN
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

// needed C structs
// added variable arg list
#include <stdarg.h>
// math missing
#include <math.h>
// for integer
#include <limits.h>
//for time structs
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { UP, DOWN };

typedef struct overlay_settings
{
    float bg_color[4];
    uint8_t orientation;
    int zoom;
} overlay_settings_t;

// hmmm maybe introduce a struct for this?
void init_overlay(GLFWwindow *_win, VkDevice logical_device,
                  VkPhysicalDevice physical_device,
                  uint32_t graphics_queue_family_index, VkQueue graphics_queue,
                  VkImageView *image_views, uint32_t image_views_len,
                  uint32_t width, uint32_t height, VkFormat color_format);

void resize_overlay(uint32_t width, uint32_t height);

// buffer index is the framebuffer index that is to be rendered to
// use the render finished semaphore of the main program so that
// the overlay has the chance to wait for the main programm to finish
// and then render the overlay on top. Will return a Semaphore that
// that the main program can wait vor
VkSemaphore submit_overlay(struct overlay_settings *settings,
                           VkQueue graphics_queue, uint32_t buffer_index,
                           VkSemaphore wait_semaphore);

// cleanup
void shutdown_overlay();

#ifdef __cplusplus
}
#endif
#endif
