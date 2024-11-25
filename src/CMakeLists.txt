
add_subdirectory(ch)


qt_add_executable(pdf-viewer)
target_sources(pdf-viewer PRIVATE
  main.cc
)


target_include_directories(pdf-viewer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/application
)


target_link_libraries(pdf-viewer PRIVATE
  Qt6::Core
  Qt6::Quick
  pdf-viewer-uiplugin
)

install(TARGETS pdf-viewer DESTINATION bin)
