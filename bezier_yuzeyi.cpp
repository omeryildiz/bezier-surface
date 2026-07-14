#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <array>
#include <cmath>
#include <cstddef>

namespace {

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vec3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

Vec3 normalize(const Vec3& v) {
    const float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length <= 0.0f) {
        return {0.0f, 0.0f, 1.0f};
    }
    return {v.x / length, v.y / length, v.z / length};
}

constexpr int kDegree = 4;
constexpr int kControlPointCount = kDegree * kDegree;
constexpr int kDetail = 20;
constexpr float kSelectionRadius = 0.55f;
constexpr float kWorldRange = 10.0f;

using Basis = std::array<float, kDegree>;
using BasisGrid = std::array<Basis, kDetail + 1>;
using MeshRow = std::array<Vec3, kDetail + 1>;
using MeshGrid = std::array<MeshRow, kDetail + 1>;

std::array<Vec3, kControlPointCount> controlPoints = {{
    {2.0f, 0.0f, 1.0f},   {3.0f, 2.0f, -1.0f},  {6.0f, 2.0f, 1.0f},   {9.0f, 0.0f, -1.0f},
    {0.0f, 3.0f, 1.0f},   {3.0f, 4.0f, -1.0f},  {6.0f, 4.0f, 1.0f},   {10.0f, 3.0f, -1.0f},
    {0.0f, 5.0f, 1.0f},   {3.0f, 6.0f, -1.0f},  {6.0f, 6.0f, 1.0f},   {10.0f, 5.0f, -1.0f},
    {1.0f, 9.0f, 1.0f},   {3.0f, 8.0f, -1.0f},  {6.0f, 8.0f, 1.0f},   {7.0f, 9.0f, -1.0f},
}};

BasisGrid basisGrid {};
BasisGrid derivativeGrid {};
MeshGrid surfaceVertices {};
MeshGrid surfaceNormals {};

bool wireframe = false;
bool surfaceDirty = true;
int selectedPoint = -1;
float xRot = 0.0f;
float zRot = 0.0f;
int windowWidth = 800;
int windowHeight = 800;
float orthoLeft = -kWorldRange;
float orthoRight = kWorldRange;
float orthoBottom = -kWorldRange;
float orthoTop = kWorldRange;

void updateProjectionBounds(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    if (width <= height) {
        orthoLeft = -kWorldRange;
        orthoRight = kWorldRange;
        orthoBottom = -kWorldRange * static_cast<float>(height) / static_cast<float>(width);
        orthoTop = -orthoBottom;
    } else {
        orthoBottom = -kWorldRange;
        orthoTop = kWorldRange;
        orthoLeft = -kWorldRange * static_cast<float>(width) / static_cast<float>(height);
        orthoRight = -orthoLeft;
    }
}

void initializeBasisTables() {
    for (int i = 0; i <= kDetail; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(kDetail);
        const float omt = 1.0f - t;

        basisGrid[i] = {
            omt * omt * omt,
            3.0f * t * omt * omt,
            3.0f * t * t * omt,
            t * t * t,
        };

        derivativeGrid[i] = {
            -3.0f * omt * omt,
            3.0f * (1.0f - 4.0f * t + 3.0f * t * t),
            3.0f * (2.0f * t - 3.0f * t * t),
            3.0f * t * t,
        };
    }
}

Vec3 evaluateSurface(int uIndex, int vIndex, const BasisGrid& uBasis, const BasisGrid& vBasis) {
    Vec3 result {0.0f, 0.0f, 0.0f};
    for (int row = 0; row < kDegree; ++row) {
        for (int col = 0; col < kDegree; ++col) {
            const float weight = uBasis[uIndex][row] * vBasis[vIndex][col];
            result += controlPoints[row * kDegree + col] * weight;
        }
    }
    return result;
}

void rebuildSurfaceIfNeeded() {
    if (!surfaceDirty) {
        return;
    }

    for (int u = 0; u <= kDetail; ++u) {
        for (int v = 0; v <= kDetail; ++v) {
            surfaceVertices[u][v] = evaluateSurface(u, v, basisGrid, basisGrid);
            const Vec3 tangentU = evaluateSurface(u, v, derivativeGrid, basisGrid);
            const Vec3 tangentV = evaluateSurface(u, v, basisGrid, derivativeGrid);
            surfaceNormals[u][v] = normalize(cross(tangentU, tangentV));
        }
    }

    surfaceDirty = false;
}

void requestRedraw() {
    glutPostRedisplay();
}

void drawAxes() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glEnd();
}

void drawSurface() {
    for (int row = 0; row < kDetail; ++row) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int col = 0; col <= kDetail; ++col) {
            const Vec3& normalA = surfaceNormals[row][col];
            const Vec3& pointA = surfaceVertices[row][col];
            glNormal3f(normalA.x, normalA.y, normalA.z);
            glVertex3f(pointA.x, pointA.y, pointA.z);

            const Vec3& normalB = surfaceNormals[row + 1][col];
            const Vec3& pointB = surfaceVertices[row + 1][col];
            glNormal3f(normalB.x, normalB.y, normalB.z);
            glVertex3f(pointB.x, pointB.y, pointB.z);
        }
        glEnd();
    }
}

void drawControlPoints() {
    glColor3f(0.0f, 0.35f, 1.0f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    for (const Vec3& point : controlPoints) {
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

Vec3 screenToWorld(int x, int y) {
    const float normalizedX = static_cast<float>(x) / static_cast<float>(windowWidth);
    const float normalizedY = static_cast<float>(y) / static_cast<float>(windowHeight);

    return {
        orthoLeft + normalizedX * (orthoRight - orthoLeft),
        orthoTop - normalizedY * (orthoTop - orthoBottom),
        0.0f,
    };
}

int findControlPoint(const Vec3& worldPos) {
    for (std::size_t i = 0; i < controlPoints.size(); ++i) {
        const Vec3& point = controlPoints[i];
        if (std::fabs(point.x - worldPos.x) <= kSelectionRadius &&
            std::fabs(point.y - worldPos.y) <= kSelectionRadius) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void menu(int selection) {
    if (selection == 1) {
        wireframe = !wireframe;
        requestRedraw();
    }
}

void display() {
    rebuildSurfaceIfNeeded();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    drawAxes();
    drawSurface();
    drawControlPoints();

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = (height == 0) ? 1 : height;
    updateProjectionBounds(windowWidth, windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, -kWorldRange, kWorldRange);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:
            xRot -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            xRot += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            zRot -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            zRot += 5.0f;
            break;
        default:
            return;
    }
    requestRedraw();
}

void mouseButton(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON) {
        return;
    }

    if (state == GLUT_DOWN) {
        selectedPoint = findControlPoint(screenToWorld(x, y));
    } else {
        selectedPoint = -1;
    }
}

void mouseMotion(int x, int y) {
    if (selectedPoint < 0) {
        return;
    }

    const Vec3 worldPos = screenToWorld(x, y);
    controlPoints[selectedPoint].x = worldPos.x;
    controlPoints[selectedPoint].y = worldPos.y;
    surfaceDirty = true;
    requestRedraw();
}

void initializeGlState() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glColor3f(1.0f, 1.0f, 1.0f);
}

}  // namespace

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("Bezier Yuzeyi");

    initializeBasisTables();
    initializeGlState();

    glutCreateMenu(menu);
    glutAddMenuEntry("Cizgili/Cizgisiz", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
