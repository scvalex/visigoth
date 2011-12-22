#ifndef NOTIFY_H
#define NOTIFY_H

#include <QObject>

class QStatusBar;

class Notify : public QObject
{
    Q_OBJECT
public:
    static void init(QStatusBar *bar);

    /* Use this when the notification is probably uninteresting. */
    static void normal(const QString &text);
    /* Use this when the user should see the notification, but his activity will not need to be interrupted. */
    static void important(const QString &text);
    /* Use this when the user *MUST* read the notifcation.  This causes a popup. */
    static void superSpecial(const QString &text);

protected:
    static void ensureInstance();

private:
    explicit Notify(QObject *parent = 0);
    virtual ~Notify();

    static Notify *instance;

    QStatusBar *statusBar;
};

#endif // NOTIFY_H
