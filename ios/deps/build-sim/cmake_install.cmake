# Install script for directory: /Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

set(CMAKE_BINARY_DIR "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim")

if(NOT PLATFORM_NAME)
  if(NOT "$ENV{PLATFORM_NAME}" STREQUAL "")
    set(PLATFORM_NAME "$ENV{PLATFORM_NAME}")
  endif()
  if(NOT PLATFORM_NAME)
    set(PLATFORM_NAME iphonesimulator)
  endif()
endif()

if(NOT EFFECTIVE_PLATFORM_NAME)
  if(NOT "$ENV{EFFECTIVE_PLATFORM_NAME}" STREQUAL "")
    set(EFFECTIVE_PLATFORM_NAME "$ENV{EFFECTIVE_PLATFORM_NAME}")
  endif()
  if(NOT EFFECTIVE_PLATFORM_NAME)
    set(EFFECTIVE_PLATFORM_NAME -iphonesimulator)
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/Debug${EFFECTIVE_PLATFORM_NAME}/libSDL2maind.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2maind.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2maind.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2maind.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/Release${EFFECTIVE_PLATFORM_NAME}/libSDL2main.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libSDL2main.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libSDL2main.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2main.a")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/Debug${EFFECTIVE_PLATFORM_NAME}/libSDL2d.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2d.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2d.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2d.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/Release${EFFECTIVE_PLATFORM_NAME}/libSDL2.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libSDL2.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libSDL2.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2.a")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake"
         "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake"
         "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES
    "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/SDL2Config.cmake"
    "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/SDL2ConfigVersion.cmake"
    "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/cmake/sdlfind.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_atomic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_audio.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_bits.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_blendmode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_clipboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_copying.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_cpuinfo.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_egl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_endian.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_error.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_events.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_filesystem.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gamecontroller.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gesture.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_guid.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_haptic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hidapi.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hints.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_joystick.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keyboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keycode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_loadso.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_locale.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_main.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_messagebox.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_metal.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mouse.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mutex.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_name.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl_glext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2ext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_khrplatform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_pixels.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_power.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_quit.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rect.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_render.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rwops.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_scancode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_sensor.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_shape.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_stdinc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_surface.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_system.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_syswm.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_common.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_compare.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_crc32.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_font.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_fuzzer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_harness.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_images.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_md5.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_memory.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_random.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_thread.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_timer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_touch.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_types.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_version.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_video.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_vulkan.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/begin_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/close_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_revision.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include-config-debug/SDL2/SDL_config.h"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_atomic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_audio.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_bits.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_blendmode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_clipboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_copying.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_cpuinfo.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_egl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_endian.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_error.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_events.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_filesystem.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gamecontroller.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gesture.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_guid.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_haptic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hidapi.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hints.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_joystick.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keyboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keycode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_loadso.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_locale.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_main.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_messagebox.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_metal.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mouse.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mutex.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_name.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl_glext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2ext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_khrplatform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_pixels.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_power.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_quit.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rect.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_render.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rwops.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_scancode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_sensor.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_shape.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_stdinc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_surface.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_system.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_syswm.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_common.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_compare.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_crc32.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_font.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_fuzzer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_harness.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_images.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_md5.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_memory.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_random.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_thread.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_timer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_touch.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_types.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_version.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_video.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_vulkan.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/begin_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/close_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_revision.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include-config-release/SDL2/SDL_config.h"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_atomic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_audio.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_bits.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_blendmode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_clipboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_copying.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_cpuinfo.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_egl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_endian.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_error.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_events.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_filesystem.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gamecontroller.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gesture.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_guid.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_haptic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hidapi.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hints.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_joystick.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keyboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keycode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_loadso.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_locale.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_main.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_messagebox.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_metal.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mouse.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mutex.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_name.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl_glext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2ext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_khrplatform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_pixels.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_power.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_quit.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rect.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_render.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rwops.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_scancode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_sensor.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_shape.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_stdinc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_surface.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_system.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_syswm.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_common.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_compare.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_crc32.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_font.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_fuzzer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_harness.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_images.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_md5.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_memory.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_random.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_thread.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_timer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_touch.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_types.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_version.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_video.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_vulkan.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/begin_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/close_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_revision.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include-config-minsizerel/SDL2/SDL_config.h"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_atomic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_audio.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_bits.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_blendmode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_clipboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_copying.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_cpuinfo.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_egl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_endian.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_error.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_events.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_filesystem.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gamecontroller.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_gesture.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_guid.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_haptic.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hidapi.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_hints.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_joystick.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keyboard.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_keycode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_loadso.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_locale.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_main.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_messagebox.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_metal.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_misc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mouse.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_mutex.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_name.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengl_glext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2ext.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_gl2platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_opengles2_khrplatform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_pixels.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_platform.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_power.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_quit.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rect.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_render.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_rwops.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_scancode.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_sensor.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_shape.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_stdinc.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_surface.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_system.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_syswm.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_assert.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_common.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_compare.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_crc32.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_font.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_fuzzer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_harness.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_images.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_log.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_md5.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_memory.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_test_random.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_thread.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_timer.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_touch.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_types.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_version.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_video.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/SDL_vulkan.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/begin_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/include/close_code.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include/SDL2/SDL_revision.h"
      "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/include-config-relwithdebinfo/SDL2/SDL_config.h"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/SDL2" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/LICENSE.txt")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/sdl2.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/sdl2-config")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aclocal" TYPE FILE FILES "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/SDL2-2.32.8/sdl2.m4")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/matsumoto_takaya/Documents/GitHub/DOOM/ios/deps/build-sim/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
