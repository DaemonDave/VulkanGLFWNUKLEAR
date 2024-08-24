#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([vulkan-gui], [0.1], [https://github.com/DaemonDave])
AC_CONFIG_SRCDIR([Tools.h])
AC_CONFIG_HEADERS([config.h])


# DRE 2022 - I added missed Automake inclusion because Michael Kerrisk's project because he didn't have the .am file.\
# The command option dist-bzip2 makes this into a distribution and uses bzip2 to compress the project Logger-1.0.tar.bz2 \
# tarballs for sending outside as separately self-configuring initialized projects.
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects])



# DRE 2022 -Automake initialization MUST GO HERE
# HERE ARE SOME ADD-ONS autoscan couldn't find:
# DRE 2022 - I added missed CANONCICAL HOST that describes the operting system far better than most kludged ones. 
AC_CANONICAL_HOST
# DRE 2022 - I added missed LT_INIT which initializes autoconf and automake to create libraries - all these tools are \
# singular so you don't need them until you do.
LT_INIT
# DRE 2022 - I added missed AC_CONFIG_MACRO_DIRS that describes remake m4 files (m4 is the engine within Autoconf and Automake).
AC_CONFIG_MACRO_DIRS([m4])

# DRE 2022 - added CPP C++ compiler
# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_FUNC_MALLOC
AC_FUNC_STRNLEN
AC_C_INLINE
AC_FUNC_MMAP
AC_TYPE_INT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_CHECK_TYPES([ptrdiff_t])

# Checks for header files.
AC_PATH_X


# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_HEADER_MAJOR

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_GETGROUPS
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MKTIME

# Standard checks for Unix functions

AC_CHECK_FUNCS([floor getcwd gethostname gettimeofday memchr memmove memset munmap pow select setenv socket sqrt strchr strdup strerror strrchr strstr strtol strtoul])
AC_CHECK_FUNCS([alarm atexit clock_gettime dup2 endpwent fdatasync ftruncate gethostbyname gethostname getpagesize getpass getspnam gettimeofday inet_ntoa memset mkdir mkfifo munmap pathconf putenv realpath rmdir select setenv sethostname setlocale socket stime strchr strdup strerror strrchr strstr strtol strtoul uname utime utmpxname])
AC_CHECK_FUNCS([isascii memchr memmove memset strcasecmp strchr strerror strrchr strspn strtol strtoul strtoull])

# DRE 2022 - Check for doxygen for autodocumentation
AC_CHECK_PROG([DOXYGEN],[doxygen],1,0)
# Check for Vulkan executables
AC_CHECK_PROG([XXD],[xxd],1,0)
AC_CHECK_PROG([GLSL_VALIDATOR],[glslangValidator],1,0)


# Checks for libraries.
# FIXME: Replace `main' with a function in `-lseccomp':
# DRE 2022 - changed to syslog library checks in automatic library extension libc for a kernel based program.
AC_CHECK_LIB([c], [openlog])
AC_CHECK_LIB([c], [syslog])
AC_CHECK_LIB([c], [vsyslog])
AC_CHECK_LIB([c], [closelog])
# DRE 2022: I found these needed libraries in Michael's Makefile.inc:
# LINUX_LIBRARIES = -lrt -ldl -lacl -lcrypt -lcap
AC_CHECK_LIB([rt], [clock_settime])
AC_CHECK_LIB([dl], [dlmopen])
AC_CHECK_LIB([acl], [acl_init])
AC_CHECK_LIB([crypt], [crypt_r])
AC_CHECK_LIB([cap], [cap_size])
AC_CHECK_LIB([yaml], [yaml_realloc])
AC_CHECK_LIB([z], [zlibVersion])
AC_CHECK_LIB([stb], [yaml_realloc])
AC_CHECK_LIB([curl], [curl_share_init])
AC_CHECK_LIB([SDL2], [SDL_UpdateYUVTexture])
AC_CHECK_LIB([glfw], [glfwSetWindowAttrib])
AC_CHECK_LIB([vulkan], [vkGetPhysicalDeviceSurfaceSupportKHR])
AC_CHECK_LIB([tinyobjloader], [_ZTIN7tinyobj14MaterialReaderE])

# ADDED BUT COMMENTED OUT UNLESS NEEDED

# MOVED INTERNAL
#AC_CHECK_HEADERS([tiny_obj_loader_c.h])
# Checks for libraries.
# FIXME: Replace 'main' with a function in '-lGL':
#AC_CHECK_LIB([GL], [main])
# FIXME: Replace 'main' with a function in '-lGLESv2':
#AC_CHECK_LIB([GLESv2], [main])
# FIXME: Replace 'main' with a function in '-lGLEW':
#AC_CHECK_LIB([GLEW], [main])
# FIXME: Replace 'main' with a function in '-lGLEW32':
#AC_CHECK_LIB([GLEW32], [main])
# FIXME: Replace 'main' with a function in '-lGLU':
#AC_CHECK_LIB([GLU], [main])
# FIXME: Replace 'main' with a function in '-lGLU32':
#AC_CHECK_LIB([GLU32], [main])
# FIXME: Replace 'main' with a function in '-lSDL2':
#AC_CHECK_LIB([SDL2], [main])
# FIXME: Replace 'main' with a function in '-lSDL2main':
#AC_CHECK_LIB([SDL2main], [main])
# FIXME: Replace 'main' with a function in '-lX11':
#AC_CHECK_LIB([X11], [main])
# FIXME: Replace 'main' with a function in '-lXext':
#AC_CHECK_LIB([Xext], [main])
# FIXME: Replace 'main' with a function in '-lXrandr':
#AC_CHECK_LIB([Xrandr], [main])
# FIXME: Replace 'main' with a function in '-lallegro':
#AC_CHECK_LIB([allegro], [main])
# FIXME: Replace 'main' with a function in '-lallegro_font':
#AC_CHECK_LIB([allegro_font], [main])
# FIXME: Replace 'main' with a function in '-lallegro_image':
#AC_CHECK_LIB([allegro_image], [main])
# FIXME: Replace 'main' with a function in '-lallegro_main':
#AC_CHECK_LIB([allegro_main], [main])
# FIXME: Replace 'main' with a function in '-lallegro_primitives':
#AC_CHECK_LIB([allegro_primitives], [main])
# FIXME: Replace 'main' with a function in '-lallegro_ttf':
#AC_CHECK_LIB([allegro_ttf], [main])
# FIXME: Replace 'main' with a function in '-lgdi32':
#AC_CHECK_LIB([gdi32], [main])
# FIXME: Replace 'main' with a function in '-lglfw':
#AC_CHECK_LIB([glfw], [main])
# FIXME: Replace 'main' with a function in '-lglfw3':
#AC_CHECK_LIB([glfw3], [main])
# FIXME: Replace 'main' with a function in '-lm':
#AC_CHECK_LIB([m], [main])
# FIXME: Replace 'main' with a function in '-lmingw32':
#AC_CHECK_LIB([mingw32], [main])
# FIXME: Replace 'main' with a function in '-lopengl32':
#AC_CHECK_LIB([opengl32], [main])
# FIXME: Replace 'main' with a function in '-ludev':
#AC_CHECK_LIB([udev], [main])
# FIXME: Replace 'main' with a function in '-lwinmm':
#AC_CHECK_LIB([winmm], [main])


# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h locale.h malloc.h netdb.h netinet/in.h paths.h shadow.h stddef.h stdint.h stdlib.h string.h sys/acl.h sys/file.h sys/ioctl.h sys/mount.h sys/param.h sys/socket.h sys/statfs.h sys/statvfs.h sys/time.h syslog.h termios.h unistd.h utime.h utmpx.h])
AC_CHECK_HEADERS([inttypes.h sys/param.h sys/socket.h sys/time.h termios.h unistd.h])

# Headers for Vulkan Windows API
AC_CHECK_HEADERS([GLFW/glfw3.h])

AC_CHECK_HEADERS([vulkan/vulkan.h])
AC_CHECK_HEADERS([curl/curl.h])
# needed for video effects
AC_CHECK_HEADERS([SDL2/SDL.h])
AC_CHECK_HEADERS([SDL2/SDL_video.h])
AC_CHECK_HEADERS([SDL2/SDL_messagebox.h])
AC_CHECK_HEADERS([SDL2/SDL_joystick.h])
AC_CHECK_HEADERS([SDL2/SDL_vulkan.h])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([malloc.h stdint.h sys/time.h unistd.h stdarg.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday memset pow setlocale sqrt strchr strstr strtol])

#Local defines for compilation
AC_DEFINE([USE_SWAPCHAIN_EXTENSIONS], [], [Use all Vk Swapchains Extensions])
AC_DEFINE([VK_NO_PROTOTYPES], [], [Use no Vk Prototypes])
AC_DEFINE([VK_USE_PLATFORM_XCB_KHR], [], [Use all Vk XCB window system])


# THIS CONFIGURE ADDS ALL SUBFOLDERS TO A AUTOMAKE BUILD IF NEEDED.
AC_CONFIG_FILES([Makefile])


AC_OUTPUT
