/** @file importfileprocessor.cpp
  * Soubor s tridou ImportFileProcessor dedici ze tridy QObject
  * , slouží pro import fotografií
  */
#include "importfileprocessor.h"

ImportFileProcessor::ImportFileProcessor(QObject *parent) :
        QObject(parent)
{
    idRouteCount = 0;
    idImageCount = 0;
}
int ImportFileProcessor::countFiles(QString fileName)
{
    int count = 0;
    if(QFileInfo(fileName).isDir())
    {
        QDir dir(fileName);
        QStringList fileList = dir.entryList();
        foreach(QString file, fileList)
        {
            if(file != "." && file != "..")
                count += countFiles(dir.absoluteFilePath(file));
        }
    }
    else
        count = 1;
    return count;

}

void ImportFileProcessor::processDropUrls(QList<QUrl> *urlList)
{
    unrecognizedList = new QStringList;
    int rec = 0;
    //importSubdirs = -2; //nenarazila jsem zatim na adresar
    importSubdirs = 1; //importovat podasresare
    countProcessed = 0;
    int count = 0;
    foreach(QUrl url, *urlList)
    {
        count += countFiles(url.toLocalFile());
    }
    emit(setProgressMaximum(count));

    foreach(QUrl url, *urlList)
    {
        processDropFile(url.toLocalFile(), rec);
    }

    emit(finished(unrecognizedList));
}

void ImportFileProcessor::processDropFile(QString fileName, int recursion)
{
    if(QFileInfo(fileName).isDir())
    {
        if(recursion > 0 && importSubdirs < 0)   //v adresari je dalsi adresar, neni nastaveno prochazeni podadresaru
        {
            /* QMessageBox *askSubdir = new QMessageBox(QMessageBox::Question, tr("File import"),
                                                     tr("Import also files from subdirectories?"),
                                                     QMessageBox::Yes | QMessageBox::No);
            QPalette pal = askSubdir->palette();
            pal.setColor(QPalette::Window, "#D0D0E7");
            askSubdir->setPalette(pal);
            int ret = askSubdir->exec();

            if(ret == QMessageBox::Yes)
                importSubdirs = 1;
            else
                importSubdirs = 0;*/
        }
        if( importSubdirs == 1 || recursion <= 0)
        {
            QDir dir(fileName);
            QStringList fileList = dir.entryList();
            foreach(QString file, fileList)
            {
                if(file != "." && file != "..")
                    processDropFile(dir.absoluteFilePath(file), recursion+1);
            }
        }
        return;
    }

    QString suffix = QFileInfo(fileName).suffix();
    if(suffix == "gpx"){    //gpx soubor
        loadGpsFile(fileName);
    }
    else if(QImageReader::imageFormat(fileName) != "")
    {
        loadImageFile(fileName);
    }
    else
    {
       unrecognizedList->append(fileName);
       countProcessed++;
       emit(setProgressValue(countProcessed));
    }
}
void ImportFileProcessor::loadGpsFile(QString gpsFileName)
{

    GpsRoute *gpsRoute = new GpsRoute(idRouteCount);
    if (gpsRoute->loadFile(gpsFileName))
    {
        delete gpsRoute;
        unrecognizedList->append(gpsFileName);
        return;
    }
    idRouteCount++;
    emit(importRouteFinished(gpsRoute));
    countProcessed++;
    emit(setProgressValue(countProcessed));
}

void ImportFileProcessor::loadImageFile(QString imageFileName)
{
    ImageData *imageData = new ImageData;

    if(imageData->loadData(imageFileName))
    {
        delete imageData;
        unrecognizedList->append(imageFileName);
        return;
    }
    imageData->id = idImageCount;
    idImageCount++;
    emit(importImageFinished(imageData));
    countProcessed++;
    emit(setProgressValue(countProcessed));
}

void ImportFileProcessor::importImages(QStringList imageList)
{
    countProcessed = 0;
    int count = 0;
    foreach(QString imageFile, imageList)
    {
        count += countFiles(imageFile);
    }
    emit(setProgressMaximum(count));

    unrecognizedList = new QStringList;
    foreach(QString imageFile, imageList)
        loadImageFile(imageFile);
    emit(finished(unrecognizedList));
}

void ImportFileProcessor::importGpsFiles(QStringList gpsList)
{
    countProcessed = 0;
    int count = 0;
    foreach(QString gpsFile, gpsList)
    {
        count += countFiles(gpsFile);
    }
    emit(setProgressMaximum(count));

    unrecognizedList = new QStringList;
    foreach(QString gpsFile, gpsList)
        loadGpsFile(gpsFile);

    emit(finished(unrecognizedList));
}
