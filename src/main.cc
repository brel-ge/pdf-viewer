
#include "ch/brel/pdfpagemodel.h"
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include <QUrl>

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);
  QGuiApplication::setApplicationName("PDF Viewer");
  QGuiApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("Simple PDF Viewer");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("pdf", "PDF file to open");

  parser.process(app);

  QString pdfPath;
  if (!parser.positionalArguments().isEmpty()) {
    pdfPath = parser.positionalArguments().first();
  }

  QUrl main_qml(QStringLiteral("qrc:/ch/brel/pdf-viewer-ui/main.qml"));

  QQmlApplicationEngine engine;
  PDFPageModel pdfModel;
  if (!pdfPath.isEmpty()) {
    pdfModel.setSource(pdfPath);
  }

  // Register PDF image provider
  engine.addImageProvider("pdf", new PDFImageProvider(&pdfModel));

  // Expose PDF model to QML
  engine.rootContext()->setContextProperty("pdfModel", &pdfModel);
  engine.load(main_qml);

  return app.exec();
}
