#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QObject>
#include <QWidget>

class OverlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OverlayWidget(QWidget *parent);

signals:
    void clicked();

public slots:
    void exitFromView();

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        qDebug() << "Widget clicked!";
        this->hide();

        emit clicked();

        // Let the base class handle the event as well
        QWidget::mousePressEvent(event);
    }

private:
};

#endif // OVERLAYWIDGET_H
