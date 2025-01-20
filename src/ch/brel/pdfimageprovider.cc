
#include "pdfimageprovider.h"
#include "pdfpagemodel.h"

PDFImageProvider::PDFImageProvider(PDFPageModel *model)
    : QQuickImageProvider(QQuickImageProvider::Image), m_model(model)
{
}

QImage PDFImageProvider::requestImage(const QString &imgId, QSize *size, const QSize &requestedSize)
{
    bool isInt = false;
    const int pageNum = imgId.toInt(&isInt);
    if (!isInt) {
        return {};
    }

    auto img =
            m_model->data(m_model->index(pageNum - 1), PDFPageModel::PageImageRole).value<QImage>();

    if (!requestedSize.isNull() && requestedSize.isValid()) {
        img = img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    if (size != nullptr) {
        *size = img.size();
    }
    return img;
}
