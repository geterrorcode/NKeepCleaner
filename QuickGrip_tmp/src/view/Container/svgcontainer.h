#pragma once

#include <QWidget>

//#include <QtGui>
//#include <QtSvg>
//
//
//#include <QtOpenGL>
//#include <QtOpenGLWidgets>

//#include <QSvgRenderer>

//#include <QWheelEvent>
//#include <QMouseEvent>
//#include <QGraphicsRectItem>
//#include <QGraphicsSvgItem>
//#include <QPaintEvent>

#include <qmath.h>

#ifdef USE_OPENGLWIDGETS
#include <QtOpenGLWidgets/qopenglwidget.h>
#endif

#define USE_SAMPLE_BUFFERS 1

static const int   TIMER_RESOLUTION = 2000;
static const float SECS_PER_TIMER = TIMER_RESOLUTION / 1000.;
static const int SCALE_INTERVAL = 1000;
static const int ROTATE_INTERVAL = 2000;

class SvgContainer  : public QWidget
{
    
	Q_OBJECT

public:
	SvgContainer(QWidget *parent = Q_NULLPTR);
	~SvgContainer();

//public:
//    
//    //SvgContainer(QWidget* parent = Q_NULLPTR)
//    SvgContainer()
//#if USE_SAMPLE_BUFFERS
//        : QOpenGLWidget(QGLFormat(QGL::SampleBuffers)),
//#else
//        : QOpenGLWidget(),
//#endif
//        m_frames(0),
//        m_rotate(0),
//        m_scale(0.9),
//        m_fps(0),
//        m_fontMetrics(0)
//    {
//        m_renderer = new QSvgRenderer;
//
//        m_scaleLine = new QTimeLine(SCALE_INTERVAL, this);
//        m_scaleLine->setUpdateInterval(16);
//        m_scaleLine->setEasingCurve(QEasingCurve::InCirc);
//        connect(m_scaleLine, SIGNAL(finished()),
//            SLOT(scaleChangeCurve()));
//        connect(m_scaleLine, SIGNAL(valueChanged(qreal)),
//            SLOT(scaleChangeScale(qreal)));
//
//        m_rotateLine = new QTimeLine(ROTATE_INTERVAL, this);
//        m_rotateLine->setEasingCurve(QEasingCurve::Linear);
//        m_rotateLine->setUpdateInterval(10);
//        connect(m_rotateLine, SIGNAL(finished()),
//            SLOT(rotateChangeCurve()));
//        connect(m_rotateLine, SIGNAL(valueChanged(qreal)),
//            SLOT(rotateChangeScale(qreal)));
//
//        QPalette pal = palette();
//        pal.setBrush(QPalette::Window, QColor(25, 76, 154));
//        setPalette(pal);
//    }
//    ~Renderer()
//    {
//        delete m_renderer;
//    }
//
//    void load(const QString& fileName)
//    {
//        m_renderer->load(fileName);
//        startTimer(TIMER_RESOLUTION);
//        m_rotateLine->start();
//        m_scaleLine->start();
//    }
//
//protected:
//    virtual void paintEvent(QPaintEvent*)
//    {
//        QPainter p(this);
//
//        p.setRenderHint(QPainter::HighQualityAntialiasing);
//
//        p.save();
//        p.translate(width() / 2, height() / 2);
//        p.rotate(m_rotate);
//        p.scale(m_scale, m_scale);
//        p.translate(-width() / 2, -height() / 2);
//
//        m_renderer->render(&p);
//        p.restore();
//
//        renderFps(&p);
//
//        ++m_frames;
//
//        QTimer::singleShot(0, this, SLOT(update()));
//    }
//    void timerEvent(QTimerEvent*)
//    {
//        m_fps = m_frames / SECS_PER_TIMER;
//        m_frames = 0;
//    }
//    QSize sizeHint() const
//    {
//        return m_renderer->defaultSize() + QSize(40, 40);
//    }
//
//private:
//    void renderFps(QPainter* p)
//    {
//        if (!m_fontMetrics) {
//            m_fontMetrics = new QFontMetrics(p->font());
//        }
//        QString str;
//        if (m_fps) {
//            str = tr("fps %1").arg(m_fps, 0, 'g', 5);
//        }
//        else {
//            str = tr("fps -");
//        }
//        QRect rect = m_fontMetrics->boundingRect(str);
//        p->setPen(Qt::yellow);
//        p->drawText(width() - 19 - rect.width(),
//            height() - 10, str);
//    }
//private slots:
//    void scaleChangeCurve()
//    {
//        m_scaleLine->toggleDirection();
//        m_scaleLine->start();
//    }
//    void scaleChangeScale(qreal sc)
//    {
//        m_scale = 0.9 + sc;
//    }
//    void rotateChangeCurve()
//    {
//        m_rotateLine->start();
//    }
//    void rotateChangeScale(qreal sc)
//    {
//        m_rotate = -sc * 360.;
//    }
//private:
//    QSvgRenderer* m_renderer;
//    int m_frames;
//    qreal m_rotate;
//    qreal m_scale;
//    QTimeLine* m_scaleLine;
//    QTimeLine* m_rotateLine;
//    qreal m_fps;
//    QFontMetrics* m_fontMetrics;

};
