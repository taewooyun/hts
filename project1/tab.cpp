#include "tab.h"
#include <QStylePainter>
#include <QStyleOptionTab>

// -----------------------------
// TabBar
// -----------------------------
TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setExpanding(true);   // 탭들이 남는 공간을 자동으로 채움
}

QSize TabBar::tabSizeHint(int index) const
{
    QSize s = QTabBar::tabSizeHint(index);

    int totalTabs = count();               // ex: 3개
    int parentH = parentWidget() ? parentWidget()->height() : 700;

    int tabHeight = parentH / totalTabs;   // ex: 700 / 3 = 233

    s.setWidth(140);       // 가로폭 원하는 값
    s.setHeight(tabHeight - 10);  // 여유 마진

    return s;
}

void TabBar::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    QStyleOptionTab opt;

    for (int i = 0; i < count(); i++)
    {
        initStyleOption(&opt, i);

        // 탭 모양 그리기
        painter.drawControl(QStyle::CE_TabBarTabShape, opt);

        painter.save();

        // 텍스트 영역
        QRect rect = tabRect(i);

        // 폰트 스타일
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        painter.setFont(font);

        // 텍스트 중앙 정렬
        painter.drawItemText(
            rect,
            Qt::AlignCenter,
            opt.palette,
            true,
            tabText(i)
            );

        painter.restore();
    }
}




// -----------------------------
// TabWidget
// -----------------------------
TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    TabBar *bar = new TabBar;
    setTabBar(bar);
    setTabPosition(QTabWidget::West);

    setStyleSheet(R"(
QTabWidget::pane {
    border: none;
}

QTabBar::tab {
    background: #ffffff;
    color: #333;
    padding: 15px;
    margin: 6px;
    border-radius: 12px;
    border: 1px solid #cfcfcf;
    font-size: 15px;
    font-weight: 600;
}

QTabBar::tab:selected {
    background: #a8b8ff;
    color: #000;
}

QTabBar::tab:hover {
    background: #f1f3ff;
}
)");
}
