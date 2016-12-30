#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <gl/glut.h>
#include "object.h" 
#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")
Object obj=Object();
string cd;
GLfloat dx = 0,dy = 0,dz = 0;
GLfloat ax = 0,ay = 0,az = 0;
GLint mx = 0,my = 0;
GLint MouseDown = 0;
GLfloat aspect = 1;
int start = 1;
int runningsteps = 0;
int stage = 0;
int countt = 0;

void myIdle()
{
	Sleep(10);
	if (stage == 0)
	{
		countt++;
		if (countt > 500)
			stage = 1;
	}
	else if (start == 1)
	{
		if (runningsteps++ == 200)
			start = 0;
	}
	
	glutPostRedisplay();
}

void myReshape(int width,int height)
{
	aspect = (float)width/(height?height:1);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75,aspect,1,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,10,0,0,0,0,1,0);
}

void calculate(int i) // 计算第i个顶点每次变换的位移
{
	obj.helper[i].x += obj.little[i].x;
	obj.helper[i].y += obj.little[i].y;
	obj.helper[i].z += obj.little[i].z;
}

void myKeyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'w':
		dz+=1.0f;
		break;
	case 's':
		dz-=1.0f;
		break;
	case 'a':
		dx-=1.0f;
		break;
	case 'd':
		dx+=1.0f;
		break;
	case 'q':
		dy+=1.0f;
		break;
	case 'e':
		dy-=1.0f;
		break;
	/*case 'x':
		if (start!=2)
			start = 1;
		break;*/
	}
	
}

void myMouse(int button,int state,int x,int y)
{
	if(button==GLUT_DOWN) MouseDown = 1,mx = x,my = y;
	else if(button==GLUT_WHEEL_UP)
	{
		dz+=1.0f;
	}
	else if(button==GLUT_WHEEL_DOWN)
	{
		dz-=1.0f;
	}
	else
	{
		MouseDown = 0;
	}
}

void myMotion(int x,int y)
{
	if(MouseDown)
	{
		ax += (y-my)/5.0f;
		ay += (x-mx)/5.0f;
		mx = x;
		my = y;
	}
}

void setLight()
{
	static const GLfloat light_position[] = {50.0f,50.0f,50.0f,0.0f};
	static const GLfloat light_ambient[] = {0.0f,0.0f,0.0f,0.0f};
	static const GLfloat light_diffuse[] = {1.0f,0.9f,0.9f,0.0f};
	static const GLfloat light_specular[] = {1.0f,1.0f,1.0f,0.0f};
	static const GLfloat light_direction[] = {-1.0f,-1.0f,-1.0f};

	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light_direction);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,10.0f);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,120.0f);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void setMaterial()
{
	GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_emission[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_shininess = 10.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslatef(dx,dy,dz);
	glRotatef(ax,1.0f,0.0f,0.0f);
	glRotatef(ay,0.0f,1.0f,0.0f);
	int length = obj.faces.size();
	if (stage == 0)
	{
		//setLight();
		//setMaterial();
		glutSolidSphere(1.0, 20, 20);
	}
	else if (stage == 1)
	{
		glBegin(GL_TRIANGLES);
		vector<int>::iterator iter = obj.faces.begin();
		if (!obj.texcoords.empty())
		{
			while (iter != obj.faces.end())
			{
				glNormal3f(obj.normals[*(iter + 2) - 1].x, obj.normals[*(iter + 2) - 1].y, obj.normals[*(iter + 2) - 1].z);
				glTexCoord2f(obj.texcoords[*(iter + 1) - 1].first, obj.texcoords[*(iter + 1) - 1].second);
				glVertex3f(obj.helper[*iter - 1].x, obj.helper[*iter - 1].y, obj.helper[*iter - 1].z);
				//glVertex3f(obj.vertexs[*iter - 1].x, obj.vertexs[*iter - 1].y, obj.vertexs[*iter - 1].z);
				iter += 3;
			}
		}
		if (start == 1)
		{
			int l = obj.vertexs.size();
			for (int i = 0; i < l; i++)
				calculate(i);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void init()
{
	ReadObj(cd,obj);

	//glutFullScreen();
	glClearColor(0.93,0.94,0.98,1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	setLight();
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowPosition(320,180);
	glutInitWindowSize(1280,720);
	glutCreateWindow("OpenGl");
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboard);
	glutMouseFunc(&myMouse);
	glutMotionFunc(&myMotion);
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);

	init();
	glutMainLoop();
	return 0;
}