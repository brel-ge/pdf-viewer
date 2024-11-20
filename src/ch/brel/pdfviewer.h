
#pragma once

#include <QQuickPaintedItem>
#include <memory>
#include <poppler/qt6/poppler-qt6.h>

class PDFViewer : public QQuickPaintedItem {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
  Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY
                 currentPageChanged)
  Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

public:
  PDFViewer(QQuickItem *parent = nullptr);
  void paint(QPainter *painter) override;

  QString source() const;
  void setSource(const QString &source);

  int currentPage() const;
  void setCurrentPage(int page);

  int pageCount() const;

signals:
  void sourceChanged();
  void currentPageChanged();
  void pageCountChanged();

private:
  void loadDocument();

  QString m_source;
  int m_currentPage;
  std::unique_ptr<Poppler::Document> m_document;
};
