#include "mandelbruhview2.h"


#include <QtCore/QRunnable>

MandelbruhRenderer::MandelbruhRenderer():
    m_program(nullptr)
{

}

void MandelbruhRenderer::setT(qreal t)
{
    m_t = t;
}

void MandelbruhRenderer::setViewportSize(const QSize &size)
{
    m_viewportSize = size;
}

void MandelbruhRenderer::setWindow(QQuickWindow *window)
{
    m_window = window;
}

void MandelbruhRenderer::init()
{
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = vertices.xy;"
                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "uniform lowp float t;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                                    "    i = floor(i * 20.) / 20.;"
                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                                    "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();

    }
}

void MandelbruhRenderer::paint()
{
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();

    m_program->bind();

    m_program->enableAttributeArray(0);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    // This example relies on (deprecated) client-side pointers for the vertex
    // input. Therefore, we have to make sure no vertex buffer is bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setUniformValue("t", (float) m_t);

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->release();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();

    m_window->endExternalCommands();
}

MandelbruhView2::MandelbruhView2() :
    m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &MandelbruhView2::handleWindowChanged);
}

qreal MandelbruhView2::t() const
{
    return m_t;
}

void MandelbruhView2::setT(qreal t)
{
        qDebug() << t;
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void MandelbruhView2::sync()
{
    if (!m_renderer) {
        m_renderer = new MandelbruhRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &MandelbruhRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &MandelbruhRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());
}

void MandelbruhView2::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void MandelbruhView2::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &MandelbruhView2::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &MandelbruhView2::cleanup, Qt::DirectConnection);
        win->setColor(Qt::black);
    }
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(MandelbruhRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    MandelbruhRenderer *m_renderer;
};

void MandelbruhView2::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}
