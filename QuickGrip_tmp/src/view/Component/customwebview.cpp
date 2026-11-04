#include "customwebview.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QVBoxLayout>
#include <QGraphicsProxyWidget>

CustomWebView::CustomWebView(const QUrl& url, QWidget* parent)
    : QWidget(parent), webView(new QWebEngineView(this)), graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)), mask(nullptr), proxy(nullptr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    webView->setUrl(url);
    graphicsView->setScene(scene);
    graphicsView->setFrameStyle(QFrame::NoFrame);

    // Add webView to the scene
    proxy = scene->addWidget(webView);

    // Create a mask to hide the top 30 pixels
    mask = new QGraphicsRectItem(0, 0, proxy->size().width(), 30);
    mask->setBrush(QBrush(Qt::white));
    mask->setPen(Qt::NoPen);
    scene->addItem(mask);

    layout->addWidget(graphicsView);
}

void CustomWebView::resizeEvent(QResizeEvent* event) 
{
    QWidget::resizeEvent(event);
    webView->resize(event->size());
    graphicsView->resize(event->size());
    proxy->resize(event->size());
    mask->setRect(0, 0, event->size().width(), 30); // Adjust the mask size
}