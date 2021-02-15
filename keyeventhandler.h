/** @file keyeventhandler.h
  * Hlavickovy soubor tridy KeyEventHandler dedici ze tridy QObject,
  * slouží pro rozpoznani klavesovych zkratek
  */

#ifndef KEYEVENTHANDLER_H
#define KEYEVENTHANDLER_H

#include <QObject>
#include <QKeyEvent>
class KeyEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit KeyEventHandler(QObject *parent = 0);

signals:
    void deleteSelected();
    void selectImage();
    void escapePressed();
public slots:
    void processKeyEvent(QKeyEvent *event);

};

#endif // KEYEVENTHANDLER_H
