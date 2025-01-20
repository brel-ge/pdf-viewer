
#pragma once

#include <QQuickImageProvider>
#include <QImage>
#include <QSize>

class PDFPageModel;

class PDFImageProvider : public QQuickImageProvider
{
public:
    explicit PDFImageProvider(PDFPageModel *model);
    QImage requestImage(const QString &imgId, QSize *size, const QSize &requestedSize) override;

private:
    PDFPageModel *m_model;
};
