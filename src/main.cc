
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
  engine.rootContext()->setContextProperty("initialPdfPath", pdfPath);
  engine.load(main_qml);

  return app.exec();
}
