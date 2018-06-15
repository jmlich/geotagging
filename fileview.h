#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QObject>
#include <QLabel>

class FileView : public QObject
{
public:
    FileView();
private:
    QLabel *imageLabel;
};

#endif // FILEVIEW_H
