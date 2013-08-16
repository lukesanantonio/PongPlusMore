find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(SDL2_PKG_CONFIG QUIET sdl2)
endif()

find_package(Threads REQUIRED)

find_path(SDL2_INCLUDE_DIRS "SDL.h" HINTS ${SDL2_PKG_CONFIG_INCLUDE_DIRS}
          PATH_SUFFIXES SDL2 DOC "SDL include directory")

find_library(SDL2_LIBRARY_TEMP "SDL2" HINTS ${SDL2_PKG_CONFIG_LIBRARY_DIRS}
             DOC "")
set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} CACHE INTERNAL "")

if(SDL2_LIBRARY_TEMP)
  set(SDL2_LIBRARYS_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
  set(SDL2_LIBRARYS ${SDL2_LIBRARYS_TEMP} CACHE STRING "SDL2 Librarys")
else()
  set(SDL2_LIBRARYS "SDL2_LIBRARYS-NOTFOUND" CACHE STRING "SDL2 Librarys")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARYS
                                                   SDL2_INCLUDE_DIRS)
