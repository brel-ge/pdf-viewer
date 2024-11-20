#include "pdfviewer.h"
#include <QPainter>
#include <QUrl>

PDFViewer::PDFViewer(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_currentPage(0) {
  setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void PDFViewer::paint(QPainter *painter) {
  if (!m_document || m_currentPage < 0 ||
      m_currentPage >= m_document->numPages()) {
    return;
  }

  std::unique_ptr<Poppler::Page> page(m_document->page(m_currentPage));
  if (!page) {
    return;
  }

  // Scale the page to fit the width while maintaining aspect ratio
  QSizeF pageSize = page->pageSizeF();
  qreal scale = width() / pageSize.width();

  QImage image = page->renderToImage(72.0 * scale, 72.0 * scale);
  painter->drawImage(0, 0, image);

  // Update the height to match the scaled page
  setHeight(image.height());
}

QString PDFViewer::source() const { return m_source; }

void PDFViewer::setSource(const QString &source) {
  if (m_source != source) {
    m_source = source;
    loadDocument();
    emit sourceChanged();
  }
}

int PDFViewer::currentPage() const { return m_currentPage; }

void PDFViewer::setCurrentPage(int page) {
  if (m_currentPage != page && m_document) {
    m_currentPage = qBound(0, page, m_document->numPages() - 1);
    update();
    emit currentPageChanged();
  }
}

int PDFViewer::pageCount() const {
  return m_document ? m_document->numPages() : 0;
}

void PDFViewer::loadDocument() {
  QString filePath = QUrl(m_source).toLocalFile();
  if (filePath.isEmpty()) {
    filePath = m_source;
  }

  m_document = Poppler::Document::load(filePath);
  if (m_document) {
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
    m_document->setRenderHint(Poppler::Document::Antialiasing);
    m_currentPage = 0;
    emit pageCountChanged();
    update();
  }
}
