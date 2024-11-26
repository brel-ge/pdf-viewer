#include "pdfpagemodel.h"
#include <QPainter>
#include <QUrl>

PDFPageModel::PDFPageModel(QObject *parent) : QAbstractListModel(parent)
{
    m_pageCache.setMaxCost(MAX_CACHE_SIZE);
}

int PDFPageModel::pageWidth() const
{
    return m_pageWidth;
}

void PDFPageModel::setPageWidth(int width)
{
    if (m_pageWidth == width) {
        return;
    }

    qDebug() << "Setting page width to" << width;
    m_pageWidth = width;

    m_pageCache.clear();
    emit pageWidthChanged();
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0), { PageImageRole });
}

int PDFPageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_document ? m_document->numPages() : 0;
}

QVariant PDFPageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_document) {
        return {};
    }

    const int pageNum = index.row();

    switch (role) {
    case PageImageRole:
        return renderPage(pageNum);
    case PageNumberRole:
        return pageNum + 1;
    default:
        return {};
    }
}

QHash<int, QByteArray> PDFPageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PageImageRole] = "pageImage";
    roles[PageNumberRole] = "pageNumber";
    return roles;
}

QString PDFPageModel::source() const
{
    return m_source;
}

void PDFPageModel::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;
        loadDocument();
        emit sourceChanged();
    }
}

int PDFPageModel::pageCount() const
{
    return m_document ? m_document->numPages() : 0;
}

int PDFPageModel::pageHeigth() const
{
    return m_document ? m_document->page(0)->pageSize().height() : 0;
}

void PDFPageModel::loadDocument()
{
    beginResetModel();

    QString filePath = QUrl(m_source).toLocalFile();
    if (filePath.isEmpty()) {
        filePath = m_source;
    }

    m_document = Poppler::Document::load(filePath);
    m_pageCache.clear();

    if (m_document) {
        m_document->setRenderHint(Poppler::Document::TextAntialiasing);
        m_document->setRenderHint(Poppler::Document::Antialiasing);
    }

    endResetModel();
    emit pageCountChanged();
}

QImage PDFPageModel::renderPage(int pageNum) const
{
    // Check cache first
    qDebug() << "Rendering page" << pageNum;
    QImage *cachedImage = m_pageCache.object(pageNum);
    if (cachedImage != nullptr) {
        return *cachedImage;
    }

    if (!m_document || pageNum < 0 || pageNum >= m_document->numPages()) {
        return {};
    }

    std::unique_ptr<Poppler::Page> page(m_document->page(pageNum));
    if (!page) {
        return {};
    }

    // // Get original page size
    QSizeF originalSize = page->pageSizeF();

    // Calculate DPI to match target width
    qreal scaling = m_pageWidth / originalSize.width();
    qreal dpi = 72.0 * scaling;
    QImage image = page->renderToImage(dpi, dpi);

    // Cache the rendered image
    const int pixelSize = 8;
    m_pageCache.insert(pageNum, new QImage(image),
                       image.width() * image.height() * image.depth() / pixelSize);
    if (m_pageHeigth != image.height()) {
        m_pageHeigth = image.height();
        pageHeigthChanged();
    }

    return image;
}
