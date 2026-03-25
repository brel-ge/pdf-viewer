#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include <memory>
#include <poppler/qt6/poppler-qt6.h>

class PDFPageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int pageWidth READ pageWidth WRITE setPageWidth NOTIFY pageWidthChanged)
    Q_PROPERTY(int pageHeight MEMBER m_pageHeigth NOTIFY pageHeigthChanged)

public:
    enum Roles { PageImageRole = Qt::UserRole + 1, PageNumberRole };

    explicit PDFPageModel(QObject *parent = nullptr);
    ~PDFPageModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString source() const;
    void setSource(const QString &source);
    int pageCount() const;
    int pageHeigth() const;

    int pageWidth() const;
    void setPageWidth(int width);

signals:
    void sourceChanged() const;
    void pageCountChanged() const;
    void pageWidthChanged() const;
    void pageHeigthChanged() const;

private:
    void loadDocument();
    QImage renderPage(int pageNum) const;
    QImage renderPageImp(int pageNum) const; // Must be called with m_mutex held
    void prefetchPages(int centerPage) const;
    void evictDistantPages(int centerPage) const;

    QString m_source;
    std::unique_ptr<Poppler::Document> m_document;

    mutable QMutex m_mutex;
    mutable QHash<int, QImage> m_pageCache;
    mutable QThreadPool m_prefetchPool;

    int m_pageWidth = 0;
    mutable int m_pageHeigth = 0;

    static const int MAX_CACHE_SIZE = 10;
    static const int PREFETCH_AHEAD = 2;
    static const int PREFETCH_BEHIND = 1;
};
