#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QLabel>
#include <QObject>

class FileView : public QObject {
public:
    FileView();

private:
    QLabel* imageLabel;
};

#endif // FILEVIEW_H
