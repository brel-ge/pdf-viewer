

add_library(ch-brel-theme STATIC)


qt_add_qml_module(ch-brel-theme
  URI ch.brel.theme
  VERSION 1.0
  SOURCES
  theme.cc
  theme.h

  RESOURCE_PREFIX /
)

target_include_directories(ch-brel-theme PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(ch-brel-theme PRIVATE
  Qt6::Core
  Qt6::Gui
)
