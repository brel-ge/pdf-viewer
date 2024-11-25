#pragma once

#include <QAbstractListModel>
#include <QCache>
#include <QObject>
#include <QQuickImageProvider>
#include <memory>
#include <poppler/qt6/poppler-qt6.h>

class PDFPageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

public:
    enum Roles { PageImageRole = Qt::UserRole + 1, PageNumberRole };

    explicit PDFPageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString source() const;
    void setSource(const QString &source);
    int pageCount() const;

signals:
    void sourceChanged();
    void pageCountChanged();

private:
    void loadDocument();
    QImage renderPage(int pageNum) const;

    QString m_source;
    std::unique_ptr<Poppler::Document> m_document;
    mutable QCache<int, QImage> m_pageCache;
    static const int MAX_CACHE_SIZE = 10; // Adjust as needed
};

class PDFImageProvider : public QQuickImageProvider
{
public:
    explicit PDFImageProvider(PDFPageModel *model)
        : QQuickImageProvider(QQuickImageProvider::Image), m_model(model)
    {
    }

    QImage requestImage(const QString &imgId, QSize *size, const QSize &requestedSize) override
    {
        bool isInt = false;
        const int pageNum = imgId.toInt(&isInt);
        if (!isInt) {
            return {};
        }

        auto img = m_model->data(m_model->index(pageNum - 1), PDFPageModel::PageImageRole)
                           .value<QImage>();

        if (!requestedSize.isNull() && requestedSize.isValid()) {
            img = img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        if (size != nullptr) {
            *size = img.size();
        }
        return img;
    }

private:
    PDFPageModel *m_model;
};
