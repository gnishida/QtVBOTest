#include <iostream>
#include <GL/glew.h>
#include "GLWidget3D.h"
#include "MainWindow.h"
#include <GL/GLU.h>
#include "OBJLoader.h"

void GLWidget3D::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton) {
		camera.mouseDown(e->x(), e->y());
	} else if (e->buttons() & Qt::RightButton) {
		camera.mouseDown(e->x(), e->y());
	}
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *e)
{
	camera.mouseUp();

	updateGL();
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton) {
		camera.rotate(e->x(), e->y());
	} else if (e->buttons() & Qt::RightButton) {
		camera.zoom(e->x(), e->y());
	}

	updateGL();
}

void GLWidget3D::initializeGL()
{
	int err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

	glClearColor(0.443, 0.439, 0.458, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	static GLfloat lightPosition[4] = {0.0f, 0.0f, 100.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glGenBuffers(1, &vbo[0]);
	loadOBJ("models/triangle.obj");
}

void GLWidget3D::resizeGL(int width, int height)
{
	height = height?height:1;

	camera.setWindowSize(width, height);

	glViewport( 0, 0, (GLint)width, (GLint)height );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width/(GLfloat)height, 0.1f, 10000);
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget3D::paintGL()
{
	glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -camera.z);
	glMultMatrixd(camera.rt);

	drawScene();		
}

void GLWidget3D::drawScene()
{
	/*

    glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < vertices.size() / 3; i += 3) {
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; ++j) {
			glNormal3f(normals[(i + j)*3], normals[(i + j)*3 + 1], normals[(i + j)*3 + 2]);
			glVertex3f(vertices[(i + j)*3], vertices[(i + j)*3 + 1], vertices[(i + j)*3 + 2]);
		}
		glEnd();
	}*/



	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]); 
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL]); 
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]); 
	glColorPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
}

void GLWidget3D::loadOBJ(const char* filename)
{
	glDeleteBuffers(TOTAL_VBO_ID, &vbo[0]);

	OBJLoader::load(filename, vertices, normals);
	colors.resize(vertices.size());
	for (int i = 0; i < colors.size(); i += 3) {
		colors[i] = 1.0f;
		colors[i+1] = 0.0f;
		colors[i+2] = 0.0f;
	}

	glGenBuffers(TOTAL_VBO_ID, &vbo[0]);
        
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]); 
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), colors.data(), GL_STATIC_DRAW);
}

