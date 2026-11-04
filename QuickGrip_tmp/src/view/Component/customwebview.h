#pragma once

#include <QObject>
#include <QWidget>
#include <QUrl>

class QWebEngineView;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsProxyWidget;

class CustomWebView  : public QWidget
{
	Q_OBJECT

public:
    explicit CustomWebView(const QUrl& url, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;



private:
    QWebEngineView* webView;
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QGraphicsRectItem* mask;
    QGraphicsProxyWidget* proxy;

};
