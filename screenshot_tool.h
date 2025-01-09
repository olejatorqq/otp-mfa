#ifndef SCREENSHOT_TOOL_H
#define SCREENSHOT_TOOL_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QRect>

class ScreenshotTool : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenshotTool(QWidget *parent = nullptr);
    ~ScreenshotTool();

signals:
    // Сигнал, который мы пошлём, когда пользователь закончил выделение
    // и мы получили итоговый QPixmap
    void areaSelected(const QPixmap &pixmap);

protected:
    // Перехватываем события мыши и отрисовку
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool selecting = false;  // Начали ли выделять
    QPoint startPoint;       // Точка, где нажали мышь
    QPoint endPoint;         // Точка, где отпустили мышь
    QPixmap background;      // Снимок всего экрана (фон)
    QRect selectionRect;     // Итоговый прямоугольник выделения

    QPixmap grabSelectedArea() const; // Вырезает нужную область
};

#endif // SCREENSHOT_TOOL_H
