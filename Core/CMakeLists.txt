cmake_minimum_required(VERSION 3.6)

project(CubismCore)
add_library(Live2DCubismCore STATIC IMPORTED GLOBAL)

set(CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(ANDROID)
  set_target_properties(Live2DCubismCore
    PROPERTIES
      IMPORTED_LOCATION
        ${CORE_PATH}/lib/android/${ANDROID_ABI}/libLive2DCubismCore.a
  )
elseif(LINUX)
  set_target_properties(Live2DCubismCore
    PROPERTIES
      IMPORTED_LOCATION
        ${CORE_PATH}/lib/linux/x86_64/libLive2DCubismCore.a
  )
elseif(WINDOWS)
  # Detect compiler.
  if(MSVC_VERSION MATCHES 1900)
    message(FATAL_ERROR "Cubism SDK 5-r.5 requires Visual Studio 2017 or newer.")
  elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
    # Visual Studio 2017
    set(MSVC_NUM 141)
  elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
    # Visual Studio 2019
    set(MSVC_NUM 142)
  elseif(MSVC_VERSION GREATER_EQUAL 1930)
    # Visual Studio 2022
    set(MSVC_NUM 143)
  elseif(MSVC)
    message(FATAL_ERROR "Unsupported Visual C++ compiler used.")
  else()
    message(FATAL_ERROR "Unsupported compiler used.")
  endif()

  # Detect core crt.
  if(CORE_CRL_MD)
    set(CRT MD)
  else()
    set(CRT MT)
  endif()

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION_DEBUG
          ${CORE_PATH}/lib/windows/x86_64/${MSVC_NUM}/Live2DCubismCore_${CRT}d.lib
        IMPORTED_LOCATION_RELEASE
          ${CORE_PATH}/lib/windows/x86_64/${MSVC_NUM}/Live2DCubismCore_MD.lib
    )
  else()
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION_DEBUG
          ${CORE_PATH}/lib/windows/x86/${MSVC_NUM}/Live2DCubismCore_${CRT}d.lib
        IMPORTED_LOCATION_RELEASE
          ${CORE_PATH}/lib/windows/x86/${MSVC_NUM}/Live2DCubismCore_MD.lib
    )
  endif()
elseif(APPLE)
  if(IOS)
    # CMake accepts either an SDK name or a full path to a versioned .sdk.
    get_filename_component(CORE_IOS_SYSROOT "${CMAKE_OSX_SYSROOT}" NAME)
    string(TOLOWER "${CORE_IOS_SYSROOT}" CORE_IOS_SYSROOT)
    if(CORE_IOS_SYSROOT MATCHES "^iphonesimulator([0-9.]+)?(\\.sdk)?$")
      set(CORE_IOS_SDK iphonesimulator)
    elseif(CORE_IOS_SYSROOT MATCHES "^iphoneos([0-9.]+)?(\\.sdk)?$")
      set(CORE_IOS_SDK iphoneos)
    else()
      message(FATAL_ERROR "Cubism Core requires an iOS SDK: set CMAKE_OSX_SYSROOT "
        "to iphoneos, iphonesimulator or the corresponding .sdk path; "
        "got '${CMAKE_OSX_SYSROOT}'.")
    endif()

    # Device and simulator archives belong to separate SDKs.
    if(CMAKE_IOS_INSTALL_COMBINED)
      message(FATAL_ERROR "Configure Cubism Core device and simulator builds separately.")
    endif()
    set(CORE_IOS_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}")
    if(NOT CORE_IOS_ARCHITECTURES)
      set(CORE_IOS_ARCHITECTURES "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    if(NOT CORE_IOS_ARCHITECTURES)
      message(FATAL_ERROR "Set CMAKE_OSX_ARCHITECTURES for Cubism Core iOS.")
    endif()
    list(REMOVE_DUPLICATES CORE_IOS_ARCHITECTURES)
    list(LENGTH CORE_IOS_ARCHITECTURES CORE_IOS_ARCH_COUNT)
    foreach(CORE_IOS_CONFIG Debug Release)
      set(CORE_IOS_LIBRARIES)
      foreach(CORE_IOS_ARCH IN LISTS CORE_IOS_ARCHITECTURES)
        if(CORE_IOS_SDK STREQUAL "iphonesimulator")
          if(NOT CORE_IOS_ARCH STREQUAL "arm64" AND NOT CORE_IOS_ARCH STREQUAL "x86_64")
            message(FATAL_ERROR "Cubism Core iOS simulator supports arm64 and x86_64; got '${CORE_IOS_ARCH}'.")
          endif()
          set(CORE_IOS_SUFFIX "iphonesimulator-${CORE_IOS_ARCH}")
        else()
          if(NOT CORE_IOS_ARCH STREQUAL "arm64")
            message(FATAL_ERROR "Cubism Core iOS device supports arm64; got '${CORE_IOS_ARCH}'.")
          endif()
          set(CORE_IOS_SUFFIX iphoneos)
        endif()
        set(CORE_IOS_LIBRARY "${CORE_PATH}/lib/ios/${CORE_IOS_CONFIG}-${CORE_IOS_SUFFIX}/libLive2DCubismCore.a")
        if(NOT EXISTS "${CORE_IOS_LIBRARY}")
          message(FATAL_ERROR "Missing Cubism Core library: ${CORE_IOS_LIBRARY}")
        endif()
        list(APPEND CORE_IOS_LIBRARIES "${CORE_IOS_LIBRARY}")
      endforeach()
      if(CORE_IOS_ARCH_COUNT GREATER 1)
        if(CMAKE_LIPO)
          set(CORE_IOS_LIPO "${CMAKE_LIPO}")
        else()
          find_program(CORE_IOS_LIPO NAMES lipo REQUIRED)
        endif()
        set(CORE_IOS_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/ios/${CORE_IOS_CONFIG}-${CORE_IOS_SDK}/libLive2DCubismCore.a")
        get_filename_component(CORE_IOS_OUTPUT_DIR "${CORE_IOS_LIBRARY}" DIRECTORY)
        add_custom_command(OUTPUT "${CORE_IOS_LIBRARY}"
          COMMAND "${CMAKE_COMMAND}" -E make_directory "${CORE_IOS_OUTPUT_DIR}"
          COMMAND "${CORE_IOS_LIPO}" -create ${CORE_IOS_LIBRARIES} -output "${CORE_IOS_LIBRARY}"
          DEPENDS ${CORE_IOS_LIBRARIES}
          COMMENT "Combining Cubism Core iOS simulator architectures (${CORE_IOS_CONFIG})"
          VERBATIM
        )
        add_custom_target(Live2DCubismCore_ios_${CORE_IOS_CONFIG} DEPENDS "${CORE_IOS_LIBRARY}")
        add_dependencies(Live2DCubismCore Live2DCubismCore_ios_${CORE_IOS_CONFIG})
      endif()
      string(TOUPPER "${CORE_IOS_CONFIG}" CORE_IOS_CONFIG_UPPER)
      set_property(TARGET Live2DCubismCore PROPERTY IMPORTED_LOCATION_${CORE_IOS_CONFIG_UPPER} "${CORE_IOS_LIBRARY}")
    endforeach()
    set_target_properties(Live2DCubismCore PROPERTIES
      IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
      IMPORTED_LOCATION "${CORE_IOS_LIBRARY}"
      MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
      MAP_IMPORTED_CONFIG_MINSIZEREL Release
      MAP_IMPORTED_CONFIG_NOCONFIG Release
    )
  elseif(MACOSX)
    # Respect explicit cross/universal builds; otherwise use CMake's target CPU.
    set(CORE_MACOS_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}")
    if(NOT CORE_MACOS_ARCHITECTURES)
      set(CORE_MACOS_ARCHITECTURES "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    if(NOT CORE_MACOS_ARCHITECTURES)
      message(FATAL_ERROR "Cannot determine the macOS architecture for Cubism Core. "
        "Set CMAKE_OSX_ARCHITECTURES to x86_64, arm64 or x86_64;arm64.")
    endif()
    list(REMOVE_DUPLICATES CORE_MACOS_ARCHITECTURES)
    set(CORE_MACOS_LIBRARIES)
    foreach(CORE_MACOS_ARCH IN LISTS CORE_MACOS_ARCHITECTURES)
      if(NOT CORE_MACOS_ARCH STREQUAL "x86_64" AND NOT CORE_MACOS_ARCH STREQUAL "arm64")
        message(FATAL_ERROR "Bundled Cubism Core for macOS supports only x86_64 and arm64; "
          "requested '${CORE_MACOS_ARCH}'. Set CMAKE_OSX_ARCHITECTURES explicitly.")
      endif()
      set(CORE_MACOS_LIBRARY "${CORE_PATH}/lib/macos/${CORE_MACOS_ARCH}/libLive2DCubismCore.a")
      if(NOT EXISTS "${CORE_MACOS_LIBRARY}")
        message(FATAL_ERROR "Missing Cubism Core macOS ${CORE_MACOS_ARCH} library: ${CORE_MACOS_LIBRARY}")
      endif()
      list(APPEND CORE_MACOS_LIBRARIES "${CORE_MACOS_LIBRARY}")
    endforeach()

    list(LENGTH CORE_MACOS_ARCHITECTURES CORE_MACOS_ARCH_COUNT)
    if(CORE_MACOS_ARCH_COUNT GREATER 1)
      # Keep the supplied thin archives intact; generate the universal library in the build tree.
      if(CMAKE_LIPO)
        set(CORE_MACOS_LIPO "${CMAKE_LIPO}")
      else()
        find_program(CORE_MACOS_LIPO NAMES lipo)
      endif()
      if(NOT CORE_MACOS_LIPO)
        message(FATAL_ERROR "A macOS universal Cubism Core build requires lipo. Set CMAKE_LIPO to its path.")
      endif()
      set(CORE_MACOS_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/macos/universal/libLive2DCubismCore.a")
      add_custom_command(OUTPUT "${CORE_MACOS_LIBRARY}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/macos/universal"
        COMMAND "${CORE_MACOS_LIPO}" -create ${CORE_MACOS_LIBRARIES} -output "${CORE_MACOS_LIBRARY}"
        DEPENDS ${CORE_MACOS_LIBRARIES}
        COMMENT "Combining macOS Cubism Core architectures"
        VERBATIM
      )
      add_custom_target(Live2DCubismCore_universal DEPENDS "${CORE_MACOS_LIBRARY}")
      add_dependencies(Live2DCubismCore Live2DCubismCore_universal)
    endif()
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION "${CORE_MACOS_LIBRARY}"
    )
  endif()
endif()
# Set core include directory.
set_target_properties(Live2DCubismCore
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CORE_PATH}/include
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_NOCONFIG Release
)
