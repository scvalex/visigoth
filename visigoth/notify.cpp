#include "notify.h"

#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>

Notify *Notify::instance(0);

Notify::Notify(QObject *parent) :
    QObject(parent),
    statusBar(0)
{
}

Notify::~Notify() {
}

void Notify::init(QStatusBar *bar) {
    ensureInstance();
    instance->statusBar = bar;
}

void Notify::ensureInstance() {
    if (instance == 0) {
        instance = new Notify();
    }
}

void Notify::normal(const QString &text) {
    ensureInstance();
    if (instance->statusBar != 0) {
        instance->statusBar->showMessage(text);
    }
}

void Notify::important(const QString &text) {
    ensureInstance();
    if (instance->statusBar != 0) {
        instance->statusBar->showMessage(text);
    }
}

void Notify::superSpecial(const QString &text) {
    instance->ensureInstance();
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
