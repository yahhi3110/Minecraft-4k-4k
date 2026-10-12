cmake_minimum_required(VERSION 3.15)
project(mc4k CXX)

add_executable(mc4k WIN32 mc4k.cpp)
target_link_libraries(mc4k PRIVATE opengl32 gdi32 user32)

if(MSVC)
  # main() をWindowsサブシステムのエントリにする
  target_link_options(mc4k PRIVATE /ENTRY:mainCRTStartup)
endif()
