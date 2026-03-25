#include "pdfpagemodel.h"
#include <QMetaObject>
#include <QMutexLocker>
#include <QRunnable>
#include <QUrl>
#include <QImage>

PDFPageModel::PDFPageModel(QObject *parent) : QAbstractListModel(parent)
{
    m_prefetchPool.setMaxThreadCount(1);
}

PDFPageModel::~PDFPageModel()
{
    m_prefetchPool.waitForDone();
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

    {
        QMutexLocker locker(&m_mutex);
        m_pageCache.clear();
    }

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
        return { };
    }

    const int pageNum = index.row();

    switch (role) {
    case PageImageRole:
        return renderPage(pageNum);
    case PageNumberRole:
        return pageNum + 1;
    default:
        return { };
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

    m_prefetchPool.waitForDone();

    QString filePath = QUrl(m_source).toLocalFile();
    if (filePath.isEmpty()) {
        filePath = m_source;
    }

    {
        QMutexLocker locker(&m_mutex);
        m_document = Poppler::Document::load(filePath);
        m_pageCache.clear();

        if (m_document) {
            m_document->setRenderHint(Poppler::Document::TextAntialiasing);
            m_document->setRenderHint(Poppler::Document::Antialiasing);
        }
    }

    endResetModel();
    emit pageCountChanged();
}

QImage PDFPageModel::renderPageImp(int pageNum) const
{
    if (!m_document || pageNum < 0 || pageNum >= m_document->numPages()) {
        return { };
    }

    std::unique_ptr<Poppler::Page> page(m_document->page(pageNum));
    if (!page) {
        return { };
    }

    const QSizeF originalSize = page->pageSizeF();
    const qreal defaultDpi = 72.0;
    const qreal scaling = m_pageWidth / originalSize.width();
    const qreal dpi = defaultDpi * scaling;

    return page->renderToImage(dpi, dpi);
}

QImage PDFPageModel::renderPage(int pageNum) const
{
    QImage result;

    {
        QMutexLocker locker(&m_mutex);

        // Check cache first
        if (m_pageCache.contains(pageNum)) {
            qDebug() << "Cache hit for page" << pageNum;
            return m_pageCache.value(pageNum);
        }

        qDebug() << "Rendering page" << pageNum;

        QImage image = renderPageImp(pageNum);
        if (image.isNull()) {
            return { };
        }

        m_pageCache.insert(pageNum, image);
        result = image;

        // Update page height safely — emit signal on the main thread
        if (m_pageHeigth != image.height()) {
            m_pageHeigth = image.height();
            QMetaObject::invokeMethod(const_cast<PDFPageModel *>(this), "pageHeigthChanged",
                                      Qt::QueuedConnection);
        }
    } // mutex released here

    // Evict distant pages if cache is too large
    evictDistantPages(pageNum);

    // Trigger prefetch for adjacent pages in the background
    prefetchPages(pageNum);

    return result;
}

void PDFPageModel::prefetchPages(int centerPage) const
{
    const int numPages = m_document ? m_document->numPages() : 0;
    if (numPages == 0) {
        return;
    }

    const int startPage = qMax(0, centerPage - PREFETCH_BEHIND);
    const int endPage = qMin(numPages - 1, centerPage + PREFETCH_AHEAD);

    for (int page = startPage; page <= endPage; ++page) {
        if (page == centerPage) {
            continue; // Already rendered
        }

        {
            QMutexLocker locker(&m_mutex);
            if (m_pageCache.contains(page)) {
                continue; // Already cached
            }
        }

        // Capture values needed for the lambda
        const int pageToRender = page;
        const int currentPageWidth = m_pageWidth;

        // Submit a background render task
        m_prefetchPool.start([this, pageToRender, currentPageWidth]() {
            QMutexLocker locker(&m_mutex);

            // Re-check: another thread may have rendered it, or width may have changed
            if (m_pageCache.contains(pageToRender) || m_pageWidth != currentPageWidth) {
                return;
            }

            qDebug() << "Prefetching page" << pageToRender;

            QImage image = renderPageImp(pageToRender);
            if (!image.isNull()) {
                m_pageCache.insert(pageToRender, image);
            }
        });
    }
}

void PDFPageModel::evictDistantPages(int centerPage) const
{
    QMutexLocker locker(&m_mutex);

    if (m_pageCache.size() <= MAX_CACHE_SIZE) {
        return;
    }

    // Remove pages furthest from the center page
    QList<int> cachedPages = m_pageCache.keys();
    std::sort(cachedPages.begin(), cachedPages.end(),
              [centerPage](int a, int b) { return qAbs(a - centerPage) > qAbs(b - centerPage); });

    while (m_pageCache.size() > MAX_CACHE_SIZE && !cachedPages.isEmpty()) {
        int pageToRemove = cachedPages.takeFirst();
        qDebug() << "Evicting page" << pageToRemove << "from cache (distance"
                 << qAbs(pageToRemove - centerPage) << "from current page" << centerPage << ")";
        m_pageCache.remove(pageToRemove);
    }
}
