#ifndef TAB_H
#define TAB_H

#include <QTabWidget>
#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTab>

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = nullptr);
    QSize tabSizeHint(int index) const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
};

#endif // TAB_H
