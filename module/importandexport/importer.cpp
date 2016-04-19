#include "importer.h"
#include "utils/imgutil.h"
#include "controller/databasemanager.h"
#include <libexif/exif-data.h>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

Importer::Importer(QObject *parent)
    : QObject(parent),m_progress(1),m_imagesCount(0)
{
    connect(&m_futureWatcher, SIGNAL(finished()),
            this, SLOT(onFutureWatcherFinish()));
    connect(&m_futureWatcher, SIGNAL(resultReadyAt(int)),
            this, SLOT(onFutureResultReady(int)));
}

QString insertImage(const QString &path)
{
    QFileInfo fileInfo(path);
    DatabaseManager::ImageInfo imgInfo;
    imgInfo.name = fileInfo.fileName();
    imgInfo.path = fileInfo.absoluteFilePath();
    imgInfo.time = utils::getCreateDateTime(path);
    imgInfo.albums = QStringList();
    imgInfo.labels = QStringList();
    imgInfo.thumbnail = utils::getThumbnail(path);

    DatabaseManager::instance()->insertImageInfo(imgInfo);

    return path;
}

void Importer::loadCacheImages()
{
    QStringList pathList = m_cacheImportList;
    QFuture<QString> future = QtConcurrent::mapped(pathList, insertImage);
    m_futureWatcher.setFuture(future);
}

Importer *Importer::m_importer = NULL;
Importer *Importer::instance()
{
    if (!m_importer) {
        m_importer = new Importer();
    }

    return m_importer;
}

double Importer::getProgress() const
{
    return m_progress;
}

void Importer::importFromPath(const QString &path)
{
    QDir dir( path );
    if( !dir.exists() ) {
        return;
    }

    m_futureWatcher.setPaused(true);
    QStringList filters;
    filters << QString("*.jpeg")
            <<QString("*.jpg")
           <<QString("*.png")
          <<QString("*.tiff")
         <<QString("*.gif")
        <<QString("*.bmp");

    QDirIterator dirIterator(path,
                             filters,
                             QDir::Files | QDir::NoSymLinks,
                             QDirIterator::Subdirectories);
    while(dirIterator.hasNext())
    {
        dirIterator.next();
        QFileInfo fileInfo = dirIterator.fileInfo();
        QString filePath = fileInfo.absoluteFilePath();

        if (! DatabaseManager::instance()->imageExist(fileInfo.fileName())) {
            m_cacheImportList.append(filePath);
        }

        m_imagesCount ++;
    }

    m_futureWatcher.setPaused(false);
    if (!m_futureWatcher.isRunning()) {
        loadCacheImages();
    }

    DatabaseManager::instance()->clearRecentImported();
}

void Importer::importSingleFile(const QString &filePath)
{
    m_cacheImportList << filePath;
    m_imagesCount ++;
    if (!m_futureWatcher.isRunning()) {
        loadCacheImages();
    }

    DatabaseManager::instance()->clearRecentImported();
}

void Importer::onFutureWatcherFinish()
{
    // Imported finish
    if (m_cacheImportList.isEmpty()) {
        qDebug() << "Imported finish, no more cache!";
        m_imagesCount = 0;
        m_progress = 1;
        emit importProgressChanged(m_progress);
    }
    else {
        loadCacheImages();
    }
}

void Importer::onFutureResultReady(int index)
{
    m_cacheImportList.removeAll(m_futureWatcher.resultAt(index));
    m_progress = 1 - (1.0 * m_cacheImportList.count() / m_imagesCount);
    emit importProgressChanged(m_progress);
}
