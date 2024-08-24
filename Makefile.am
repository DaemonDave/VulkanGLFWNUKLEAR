##
## Description  : NUKLEAR-TEMPLATES - Makefile.am
## Author       : DRE 2024
## Purpose      : main Makefile
## Revision     : $Id: Makefile.am,v 1.12 2008/12/02 20:49:08 cvsuser Exp $
##
# added old Makefile as add-on that allows backwards compatability to original Makefile recipes.
#include "Makefile.cmake"


# DRE 2022 : the subdirectories alert for automake 
# NOTE: Every line ends in a Carriage Return
SUBDIRS = 

# test program for the library
check_PROGRAMS							= demo  test-demo test-new



#testing the internal state functions
demo_SOURCES					=   demo.cpp\
overlay.c\
overview.c



demo_CFLAGS			=  -I/home/dave/src/new/nuklear-master/nuklear-master -I/home/dave/src/new/nuklear-master/nuklear-master/glfw-3.2.1/include -I/home/dave/src/vulkansdk/1.3.268.0/x86_64/include -I/home/dave/src/new/nuklear-master/nuklear-master
demo_CXXFLAGS		=  -I/home/dave/src/new/nuklear-master/nuklear-master -I/home/dave/src/new/nuklear-master/nuklear-master/glfw-3.2.1/include -I/home/dave/src/vulkansdk/1.3.268.0/x86_64/include -I/home/dave/src/new/nuklear-master/nuklear-master

demo_LDFLAGS		=  -L/usr/lib/x86_64-linux-gnu

demo_LDADD			=  glfw-3.2.1/src/libglfw3.a  -lrt -lm -lX11-xcb -lc -lXrandr -lXinerama -lXcursor -lvulkan -lX11 


demo_DEPENDENCIES = demoshaders/overlay.frag.h\
					demoshaders/overlay.frag.spv\
					demoshaders/overlay.vert.h\
					demoshaders/overlay.vert.spv\
					demoshaders/triangle.frag.h\
					demoshaders/triangle.frag.spv\
					demoshaders/triangle.vert.h\
					demoshaders/triangle.vert.spv\
					nuklearshaders/nuklear.frag.h\
					nuklearshaders/nuklear.frag.spv\
					nuklearshaders/nuklear.vert.h\
					nuklearshaders/nuklear.vert.spv
					
	

#testing the internal state functions
test_demo_SOURCES					=   demo.cpp\
overlay.c



test_demo_CFLAGS		=  -I/home/dave/src/new/nuklear-master/nuklear-master -I/home/dave/src/new/nuklear-master/nuklear-master/glfw-3.2.1/include -I/home/dave/src/vulkansdk/1.3.268.0/x86_64/include -I/home/dave/src/new/nuklear-master/nuklear-master

test_demo_CXXFLAGS		=  -I/home/dave/src/new/nuklear-master/nuklear-master -I/home/dave/src/new/nuklear-master/nuklear-master/glfw-3.2.1/include -I/home/dave/src/vulkansdk/1.3.268.0/x86_64/include -I/home/dave/src/new/nuklear-master/nuklear-master

test_demo_LDADD			=  glfw-3.2.1/src/libglfw3.a  -lrt -lm -lX11-xcb -lc -lXrandr -lXinerama -lXcursor -lvulkan -lX11 

test_demo_LDFLAGS		=   -L/usr/lib/x86_64-linux-gnu

test_demo_DEPENDENCIES = demoshaders/overlay.frag.h\
					demoshaders/overlay.frag.spv\
					demoshaders/overlay.vert.h\
					demoshaders/overlay.vert.spv\
					demoshaders/triangle.frag.h\
					demoshaders/triangle.frag.spv\
					demoshaders/triangle.vert.h\
					demoshaders/triangle.vert.spv\
					nuklearshaders/nuklear.frag.h\
					nuklearshaders/nuklear.frag.spv\
					nuklearshaders/nuklear.vert.h\
					nuklearshaders/nuklear.vert.spv
	

#testing the internal state functions
test_new_SOURCES	=   OperatingSystem.cpp\
						VulkanFunctions.cpp\
						VulkanCommon.cpp\
						Tools.cpp\
						Tutorial07.cpp\
						main.cpp

test_new_CFLAGS		=   -I/home/dave/src/vulkansdk-linux-x86_64-1.3.261.1/1.3.261.1/x86_64/include  -DUSE_SWAPCHAIN_EXTENSIONS -DVK_NO_PROTOTYPES -DVK_USE_PLATFORM_XCB_KHR


test_new_CPPFLAGS	=   -I/home/dave/src/vulkansdk-linux-x86_64-1.3.261.1/1.3.261.1/x86_64/include -I/home/dave/src/vulkansdk-linux-x86_64-1.3.261.1/1.3.261.1/x86_64/include/vulkan  -O3 -DNDEBUG   -std=c++11 -DUSE_SWAPCHAIN_EXTENSIONS -DVK_NO_PROTOTYPES -DVK_USE_PLATFORM_XCB_KHR




test_new_CXXFLAGS		=  -Iglfw-3.2.1/include 

test_new_LDADD			=  -L/usr/lib64  glfw-3.2.1/src/libglfw3.a  -lrt -lm -lxcb -lc -lXrandr -lXinerama -lXcursor -lvulkan -lX11 

test_new_LDFLAGS		=  -L/usr/lib/x86_64-linux-gnu -L/home/src/vulkansdk-linux-x86_64-1.3.261.1/1.3.261.1/x86_64/bin/



test_new_DEPENDENCIES = demoshaders/overlay.frag.h\
					demoshaders/overlay.frag.spv\
					demoshaders/overlay.vert.h\
					demoshaders/overlay.vert.spv\
					demoshaders/triangle.frag.h\
					demoshaders/triangle.frag.spv\
					demoshaders/triangle.vert.h\
					demoshaders/triangle.vert.spv\
					nuklearshaders/nuklear.frag.h\
					nuklearshaders/nuklear.frag.spv\
					nuklearshaders/nuklear.vert.h\
					nuklearshaders/nuklear.vert.spv


# DRE2022 - a generic aclocal flag set for m4 files
ACLOCAL_AMFLAGS = -I m4 -I/usr/include -I/usr/local/include


demoshaders/triangle.frag.h: demoshaders/triangle.frag.spv
	/usr/bin/xxd -i demoshaders/triangle.frag.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.frag.h

demoshaders/triangle.vert.h: demoshaders/triangle.vert.spv
	/usr/bin/xxd -i demoshaders/triangle.vert.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.vert.h

demoshaders/overlay.frag.h: demoshaders/overlay.frag.spv
	/usr/bin/xxd -i demoshaders/overlay.frag.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.frag.h

demoshaders/overlay.vert.h: demoshaders/overlay.vert.spv
	/usr/bin/xxd -i demoshaders/overlay.vert.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.vert.h

nuklearshaders/nuklear.frag.h: nuklearshaders/nuklear.frag.spv
	/usr/bin/xxd -i nuklearshaders/nuklear.frag.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.frag.h

nuklearshaders/nuklear.vert.h: nuklearshaders/nuklear.vert.spv
	/usr/bin/xxd -i nuklearshaders/nuklear.vert.spv /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.vert.h

demoshaders/triangle.frag.spv: demoshaders/triangle.frag
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.frag -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.frag.spv

demoshaders/triangle.vert.spv: demoshaders/triangle.vert
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.vert -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/triangle.vert.spv

demoshaders/overlay.frag.spv: demoshaders/overlay.frag
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.frag -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.frag.spv

demoshaders/overlay.vert.spv: demoshaders/overlay.vert
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.vert -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/demoshaders/overlay.vert.spv

nuklearshaders/nuklear.frag.spv: nuklearshaders/nuklear.frag
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.frag -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.frag.spv

nuklearshaders/nuklear.vert.spv: nuklearshaders/nuklear.vert
	/home/dave/src/vulkansdk/1.3.268.0/x86_64/bin/glslangValidator -V /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.vert -o /home/dave/src/NUKLEAR-GLFW-VULKAN/working/nuklear-glfw-vulkan-master/nuklearshaders/nuklear.vert.spv

