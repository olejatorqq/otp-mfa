// screenshot_tool.cpp

#include "screenshot_tool.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

#include "screenshot_tool.h"

#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QPainter>
#include <QDebug>

ScreenshotTool::ScreenshotTool(QWidget *parent)
    : QWidget(parent)
{
    // Уберём декорации, сделаем окно на весь экран и поверх других
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::CrossCursor);

    // Делаем окно прозрачным и будем закрашивать в paintEvent
    setAttribute(Qt::WA_TranslucentBackground);

    // Получаем снимок экрана (или основного монитора)
    QScreen *screen = QGuiApplication::primaryScreen();
    background = screen->grabWindow(0);

    // Растягиваемся на весь экран
    setGeometry(QApplication::desktop()->geometry());
}

ScreenshotTool::~ScreenshotTool() {}

// Когда нажимаем левую кнопку мыши
void ScreenshotTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        startPoint = event->pos();
        endPoint = event->pos();
        selectionRect = QRect(startPoint, endPoint);
        update(); // перерисовать
    } else {
        // Если нажали правую кнопку - завершаем без результата
        close();
    }
}

void ScreenshotTool::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        endPoint = event->pos();
        selectionRect = QRect(startPoint, endPoint).normalized();
        update();
    }
}

void ScreenshotTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (selecting && event->button() == Qt::LeftButton) {
        selecting = false;
        endPoint = event->pos();
        selectionRect = QRect(startPoint, endPoint).normalized();

        // Захватываем выбранную область
        QPixmap selected = grabSelectedArea();

        if (!selected.isNull()) {
            emit areaSelected(selected);
        }
        // Закрываемся
        close();
    }
}

void ScreenshotTool::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Рисуем скриншот на фоне
    painter.drawPixmap(0, 0, background);

    // Затем полупрозрачный слой (серый/чёрный)
    QColor overlayColor(0, 0, 0, 100); // 100 - прозрачность
    painter.fillRect(rect(), overlayColor);

    // Теперь рисуем "прозрачное окошко" внутри выделения
    // Способ 1: вырезать из painter'а region
    // Способ 2: Просто заново нарисовать background внутри selectionRect

    // Допустим, делаем способ 2:
    if (selecting) {
        QPixmap cropped = background.copy(selectionRect);
        painter.drawPixmap(selectionRect.topLeft(), cropped);
    }

    // Рисуем границу прямоугольника
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(selectionRect);
}

QPixmap ScreenshotTool::grabSelectedArea() const
{
    if (selectionRect.isEmpty()) {
        // Пустой прямоугольник
        return QPixmap();
    }
    // Вырезаем нужную область из background
    QPixmap cut = background.copy(selectionRect);
    return cut;
}
