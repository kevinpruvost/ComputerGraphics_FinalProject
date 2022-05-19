cmake_minimum_required (VERSION 2.6)
project (Stb_image)

if(ANDROID)
  # Android requires GL ES 2.0 package automatically
  set(LIBRARIES GLESv2)
else()
  find_package (OpenGL REQUIRED)
  include_directories (${OPENGL_INCLUDE_DIR})
  set(LIBRARIES ${OPENGL_LIBRARIES})
endif()

file (GLOB STB_SOURCES *.cpp)
file (GLOB STB_HEADERS *.h)

add_library (Stb_image ${STB_SOURCES} ${STB_HEADERS})
target_link_libraries (Stb_image ${LIBRARIES})
target_include_directories(Stb_image PUBLIC ./)

install (TARGETS Stb_image
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install (FILES ${STB_HEADERS} DESTINATION include/stb_image)

export(PACKAGE Stb_image)