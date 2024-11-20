

add_subdirectory(theme)

add_library(pdf-viewer-ui STATIC)
target_link_libraries(pdf-viewer-ui PRIVATE
  Qt6::Quick
  poppler-qt6
  ch-brel-themeplugin
)


qt_add_qml_module(pdf-viewer-ui
  URI ch.brel
  VERSION 1.0
  QML_FILES
  atoms/PrimaryButton.qml
  molecules/BackHomeMenu.qml
  pdf-viewer-ui/main.qml
  SOURCES
  app.cc app.h
  pdfviewer.cc pdfviewer.h
  RESOURCE_PREFIX /
)

qt_add_resources(pdf-viewer-ui assets
  PREFIX ch/brel/pdf-viewer-ui
  FILES
  assets/icons/action/close.png
  assets/icons/action/chevron_left.png
  assets/icons/action/chevron_right.png
  assets/icons/camera/home.png
)
