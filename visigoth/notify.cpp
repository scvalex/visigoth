#include "notify.h"

#include <QDebug>
#include <QMessageBox>

Notify *Notify::instance(0);

Notify::Notify(QObject *parent) :
    QObject(parent)
{
}

Notify::~Notify() {
}

void Notify::ensureInstance() {
    if (instance == 0) {
        instance = new Notify();
    }
}

void Notify::normal(const QString &text) {
    ensureInstance();
    qDebug() << text;
}

void Notify::important(const QString &text) {
    ensureInstance();
    qDebug() << text;
}

void Notify::superSpecial(const QString &text) {
    ensureInstance();
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
