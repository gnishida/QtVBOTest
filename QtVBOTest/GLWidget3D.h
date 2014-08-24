#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Camera.h"
#include <QVector3D>
#include <vector>

using namespace std;

class GLWidget3D : public QGLWidget
{
public:
	void drawScene();
	void loadOBJ(const char* filename);

protected:
	void initializeGL();

	void resizeGL(int width, int height);
	void paintGL();    

	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	static enum{VERTEX,NORMAL,COLOR,TOTAL_VBO_ID};

	Camera camera;

	GLuint vbo[3];
	vector<float> vertices;
	vector<float> normals;
	vector<float> colors;
};

