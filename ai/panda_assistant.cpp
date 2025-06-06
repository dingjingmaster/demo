#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <cmath>

// 自定义Perspective函数，替代gluPerspective
void myPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    GLdouble f = 1.0 / std::tan(fovy * M_PI / 360.0);
    GLdouble m[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar+zNear)/(zNear-zFar), -1,
        0, 0, (2*zFar*zNear)/(zNear-zFar), 0
    };
    glMultMatrixd(m);
}

class PandaWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    PandaWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        setFixedSize(600, 600);
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this]() {
            frame++;
            update();
        });
        timer->start(16); // 约60FPS
    }

    enum Action { Idle, Run, Jump, Eat, Face };
    void setAction(Action a) { action = a; frame = 0; }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        myPerspective(45.0, double(w)/h, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0, -0.5, -6);

        float t = frame / 30.0f;
        float jumpY = 0, runX = 0, eatBamboo = 1.0;
        bool faceMode = false;

        // 动作参数
        if (action == Run) {
            runX = std::sin(t*4) * 0.2;
        } else if (action == Jump) {
            jumpY = std::abs(std::sin(t*3)) * 1.2;
        } else if (action == Eat) {
            eatBamboo = std::max(0.2f, 1.0f - t/3.0f);
        } else if (action == Face) {
            faceMode = true;
        }

        glTranslatef(runX, jumpY, 0);

        // 画身体
        drawSphere(0, 0, 0, 0.7, QColor(255,255,255)); // 身体
        drawSphere(0, 0.8, 0, 0.5, QColor(255,255,255)); // 头

        // 画四肢
        for (int i=0; i<2; ++i) { // 左右手
            float angle = (action==Run) ? std::sin(t*8 + i*3.14f)*30 : 0;
            glPushMatrix();
            glTranslatef(i?0.45f:-0.45f, 0.2f, 0);
            glRotatef(angle, 1,0,0);
            drawSphere(0, -0.4f, 0, 0.18f, QColor(0,0,0));
            glPopMatrix();
        }
        for (int i=0; i<2; ++i) { // 左右脚
            float angle = (action==Run) ? -std::sin(t*8 + i*3.14f)*30 : 0;
            glPushMatrix();
            glTranslatef(i?0.25f:-0.25f, -0.6f, 0.2f);
            glRotatef(angle, 1,0,0);
            drawSphere(0, -0.2f, 0, 0.16f, QColor(0,0,0));
            glPopMatrix();
        }

        // 画耳朵
        drawSphere(-0.25f, 1.15f, 0, 0.13f, QColor(0,0,0));
        drawSphere(0.25f, 1.15f, 0, 0.13f, QColor(0,0,0));

        // 画眼圈
        drawSphere(-0.15f, 0.95f, 0.42f, 0.09f, QColor(0,0,0));
        drawSphere(0.15f, 0.95f, 0.42f, 0.09f, QColor(0,0,0));

        // 画眼睛
        if (faceMode) {
            drawSphere(-0.15f, 0.97f, 0.5f, 0.04f, QColor(255,0,0)); // 扮鬼脸-红眼
            drawSphere(0.15f, 0.97f, 0.5f, 0.04f, QColor(255,0,0));
        } else {
            drawSphere(-0.15f, 0.97f, 0.5f, 0.04f, QColor(0,0,0));
            drawSphere(0.15f, 0.97f, 0.5f, 0.04f, QColor(0,0,0));
        }

        // 鼻子
        drawSphere(0, 0.9f, 0.5f, 0.05f, QColor(0,0,0));

        // 嘴巴
        glColor3f(0,0,0);
        glLineWidth(3);
        glBegin(GL_LINE_STRIP);
        if (faceMode) {
            glVertex3f(-0.04f, 0.85f, 0.52f);
            glVertex3f(0, 0.83f, 0.53f);
            glVertex3f(0.04f, 0.85f, 0.52f);
        } else {
            glVertex3f(-0.04f, 0.85f, 0.52f);
            glVertex3f(0, 0.84f, 0.53f);
            glVertex3f(0.04f, 0.85f, 0.52f);
        }
        glEnd();

        // 竹子
        if (action == Eat) {
            glColor3f(0.2f, 0.7f, 0.2f);
            glPushMatrix();
            glTranslatef(0, 0.7f, 0.7f);
            glRotatef(-30, 1,0,0);
            glScalef(0.08f, eatBamboo, 0.08f);
            drawCylinder();
            glPopMatrix();
        }
    }

    void keyPressEvent(QKeyEvent *e) override {
        if (e->key() == Qt::Key_1) setAction(Idle);
        if (e->key() == Qt::Key_2) setAction(Run);
        if (e->key() == Qt::Key_3) setAction(Jump);
        if (e->key() == Qt::Key_4) setAction(Eat);
        if (e->key() == Qt::Key_5) setAction(Face);
    }

private:
    QTimer *timer;
    int frame = 0;
    Action action = Idle;

    // 简单球体绘制
    void drawSphere(float x, float y, float z, float r, QColor color) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(color.redF(), color.greenF(), color.blueF());
        int lats = 16, longs = 16;
        for(int i = 0; i <= lats; i++) {
            double lat0 = M_PI * (-0.5 + double(i - 1) / lats);
            double z0  = sin(lat0);
            double zr0 =  cos(lat0);

            double lat1 = M_PI * (-0.5 + double(i) / lats);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

            glBegin(GL_QUAD_STRIP);
            for(int j = 0; j <= longs; j++) {
                double lng = 2 * M_PI * double(j - 1) / longs;
                double x0 = cos(lng);
                double y0 = sin(lng);

                glVertex3f(r * x0 * zr0, r * y0 * zr0, r * z0);
                glVertex3f(r * x0 * zr1, r * y0 * zr1, r * z1);
            }
            glEnd();
        }
        glPopMatrix();
    }

    // 简单圆柱体绘制（竹子）
    void drawCylinder() {
        int slices = 20;
        float h = 1.0f;
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(slices);
            float x = cosf(theta);
            float y = sinf(theta);
            glVertex3f(x, y, 0);
            glVertex3f(x, y, h);
        }
        glEnd();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    PandaWidget *panda = new PandaWidget;
    layout->addWidget(panda);

    // 按钮切换动作
    QStringList actions = {"静止", "跑步", "跳跃", "吃竹子", "扮鬼脸"};
    for (int i = 0; i < actions.size(); ++i) {
        QPushButton *btn = new QPushButton(actions[i]);
        layout->addWidget(btn);
        QObject::connect(btn, &QPushButton::clicked, [panda, i]() {
            panda->setAction(PandaWidget::Action(i));
        });
    }

    window.show();
    return app.exec();
}

#include "panda_assistant.moc" 