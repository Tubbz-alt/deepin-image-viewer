#include "exporter.h"
#include "controller/databasemanager.h"
#include "utils/imageutils.h"

#include <QFileDialog>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

Exporter *Exporter::m_exporter = NULL;
Exporter *Exporter::instance()
{
    if (!m_exporter) {
        m_exporter = new Exporter();
    }

    return m_exporter;
}


Exporter::Exporter(QObject *parent)
    : QObject(parent)
{
}

//TODO: if some format is valid to read, but can't support to export, should add some process ?
//Such as: gif, svg, pbm, pgm

void Exporter::exportImage(const QStringList imagePaths) {
    if (imagePaths.isEmpty()) {
        return;
    } else if (imagePaths.length() == 1) {
        initValidFormatMap();
        QFileDialog exportDialog;

        //Todo: need to filter the format of images.
        QString imagePath = imagePaths.at(0);
        QString imageName = QString("%1.%2").arg(QFileInfo(imagePath).baseName())
                .arg(QFileInfo(imagePath).completeSuffix());
        QString picLocation = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0);

        QString imageSavePath = QString("%1/%2").arg(picLocation).arg(imageName);

        qDebug() << "Exporter:" << QFileInfo(imagePath).baseName() << QFileInfo(imagePath).completeSuffix();
        qDebug() << "imageSavePath:" << imageSavePath;
        QString selectFilter = tr("JPEG(*.bmp *.gif *.jpg; *.jpeg; *.png *.pbm;*.pgm *.ppm *.xbm *.xpm *.svg *.dds *.icns"
                                  "*.jp2 *.mng *.tga *.tiff *.wbmp *.webp;)");
        QString dialogFilePath = exportDialog.getSaveFileName(nullptr, tr("Save File"),
        imageSavePath, getOrderFormat(QFileInfo(imagePath).completeSuffix()),
        &selectFilter, QFileDialog::DontUseNativeDialog);


        qDebug() << "dialogFilePath:" << dialogFilePath;
        QPixmap tmpImage(imagePaths.at(0));
        if (!tmpImage.isNull() && !dialogFilePath.isEmpty())
            tmpImage.save(dialogFilePath);
    } else {
        popupDialogSaveImage(imagePaths);
    }
}

void Exporter::exportAlbum(const QString &albumname) {
    QList<DatabaseManager::ImageInfo> albumImageInfos =
            DatabaseManager::instance()->getImageInfosByAlbum(albumname);
    QStringList tmpImagePaths = QStringList();
    for (int i = 0; i < albumImageInfos.length(); i++) {
        qDebug() << "album:" << albumImageInfos[i].path;
        tmpImagePaths.append(albumImageInfos[i].path);
    }

    popupDialogSaveImage(tmpImagePaths);
}

void Exporter::popupDialogSaveImage(const QStringList imagePaths) {
    QFileDialog exportDialog;
    QString exportdir = exportDialog.getExistingDirectory(nullptr, tr("Save File"),
           QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0),
                                                          QFileDialog::ShowDirsOnly);
    qDebug() << "popupDialogSaveImage:" << exportdir;
    for (int j(0); j < imagePaths.length(); j++) {

        if(utils::image::imageIsSupport(imagePaths[j])) {
            QPixmap tmpImage(imagePaths[j]);
            QString savePath =  QString("%1/%2.%3").arg(exportdir).arg(QFileInfo(imagePaths[j])
        .baseName()).arg(QFileInfo(imagePaths[j]).completeSuffix());
            if (!tmpImage.isNull() && !savePath.isEmpty())
            tmpImage.save(savePath);
        } else {
            continue;
        }
    }
}

void Exporter::initValidFormatMap() {
    m_picFormatMap.insert("jpeg", "JPEG (*.jpeg)");
    m_picFormatMap.insert("jpg", "JPG (*.jpg)");
    m_picFormatMap.insert("bmp", "BMP (*.bmp)");
    m_picFormatMap.insert("png", "PNG (*.png)");

    m_picFormatMap.insert("ppm", "PGM (*.ppm)");
    m_picFormatMap.insert("xbm", "XBM (*.xbm)");
    m_picFormatMap.insert("xpm", "XPM (*.xpm)");

}

QString Exporter::getOrderFormat(QString defaultFormat) {
    QString allFormat = "";
    QMap<QString, QString>::const_iterator i = m_picFormatMap.constBegin();
    while (i != m_picFormatMap.constEnd()) {
        if (i.key() == defaultFormat)
            allFormat = QString("%1;;%2").arg(m_picFormatMap.value(defaultFormat)).arg(allFormat);
        else
            if (i == m_picFormatMap.constEnd() - 1)
                allFormat = QString("%1%2").arg(allFormat).arg(i.value());
            else
               allFormat = QString("%1%2;;").arg(allFormat).arg(i.value());

        qDebug() << i.key() << ": " << i.value();
        ++i;
    }

    return allFormat;
}
