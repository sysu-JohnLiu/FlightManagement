#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QObject>

#include <QEvent>
#include <QMouseEvent>
#include <QLineEdit>

class MyEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit MyEventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            Q_UNUSED(mouseEvent); // 如果需要，可以在这里处理鼠标事件
            emit clicked();
            return true; // 阻止事件继续传播（如果需要）
        }
        return QObject::eventFilter(obj, event);
    }

signals:
    void clicked();
};

#endif // MYEVENTFILTER_H
