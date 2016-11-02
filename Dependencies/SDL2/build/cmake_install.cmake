# Install script for directory: D:/Lucia/Dependencies/SDL2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SDL2")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Lucia/Dependencies/SDL2/build/libSDL2.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Lucia/Dependencies/SDL2/build/libSDL2.dll.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Lucia/Dependencies/SDL2/build/libSDL2.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libSDL2.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libSDL2.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/mingw64/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libSDL2.dll")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Lucia/Dependencies/SDL2/build/libSDL2main.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "D:/Lucia/Dependencies/SDL2/include/begin_code.h"
    "D:/Lucia/Dependencies/SDL2/include/close_code.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_assert.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_atomic.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_audio.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_bits.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_blendmode.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_clipboard.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_android.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_iphoneos.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_macosx.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_minimal.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_pandora.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_psp.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_windows.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_winrt.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_config_wiz.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_copying.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_cpuinfo.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_egl.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_endian.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_error.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_events.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_filesystem.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_gamecontroller.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_gesture.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_haptic.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_hints.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_joystick.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_keyboard.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_keycode.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_loadso.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_log.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_main.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_messagebox.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_mouse.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_mutex.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_name.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengl.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles2.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles2_gl2.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles2_gl2ext.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles2_gl2platform.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengles2_khrplatform.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_opengl_glext.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_pixels.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_platform.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_power.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_quit.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_rect.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_render.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_revision.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_rwops.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_scancode.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_shape.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_stdinc.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_surface.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_system.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_syswm.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_assert.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_common.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_compare.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_crc32.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_font.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_fuzzer.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_harness.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_images.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_log.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_md5.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_test_random.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_thread.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_timer.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_touch.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_types.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_version.h"
    "D:/Lucia/Dependencies/SDL2/include/SDL_video.h"
    "D:/Lucia/Dependencies/SDL2/build/include/SDL_config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Lucia/Dependencies/SDL2/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
