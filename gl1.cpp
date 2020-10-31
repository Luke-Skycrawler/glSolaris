#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include "astroid.h"

float fTranslate;
float fRotate,teapotRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f
float position[]={0.0f,0.0f,0.0f};

bool bPersp = true;
bool bAnim = false;
bool bWire = true;

int wHeight = 0;
int wWidth = 0;

//todo
//hint: some additional parameters may needed here when you operate the teapot
astroid sun(1.0,default_axis,0.5f);
void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	sun.draw();
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	
	if (bPersp){
		//todo when 'p' operation, hint: use FUNCTION gluPerspective
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glTranslated(0,0,-10.f);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		gluPerspective(30,whRatio,1.0,100);
	}
	else
	    glOrtho(-3 ,3, -3, 3,-100,100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8},r=8.0f;
float theta=0.0f,phi=0.0f;
float orientation[] = {0, 0, 10};
float center[] = {0, 0, 0};
float STEP = 0.01f,BOUND=1.75f;
static bool keypressed[256] = {false};
static const float Dtheta=0.01f,north=1.5f;
//todo; hint: you may need another ARRAY when you operate the teapot
void setCamera(){
	eye[0]=r*sin(theta)*cos(phi);
	eye[1]=sin(phi)*r;
	eye[2]=(cos(theta)*cos(phi))*r;
	
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1,0);
	//updateView(wHeight, wWidth);
}
void keyup(unsigned char k, int x, int y){
	keypressed[k]=false;
}
void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': 
	case 'd': 
	case 'w': 
	case 's': 
	case 'z':
	case 'c': keypressed[k] = true; break;
        }
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	//gluLookAt(eye[0], eye[1], eye[2],
	//	center[0], center[1], center[2],
	//	0, 1, 0);				
	setCamera();
	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

////	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
//	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	//if (bAnim) 
	//	fRotate    += 0.5f;
	//else if(teapot_spin)
	//	teapotRotate += 0.5f;
	if(bAnim)sun.animate();
	if(keypressed['a']) theta-=Dtheta;
	if(keypressed['d']) theta+=Dtheta;
	if(keypressed['w'] && phi < north-Dtheta) phi+=Dtheta;
	if(keypressed['s'] && phi > Dtheta-north) phi-=Dtheta;
	if(keypressed['z']) r-=STEP;
	if(keypressed['c']) r+=STEP;
	//todo; hint: when you want to rotate the teapot, you may like to add another line here =)
	glutSwapBuffers();
}
int main (int argc,  char *argv[])
{	
	float moonObit[]={sin(.2f),0,cos(.2f)};
	float jupiteraxis[] ={0,sin(.3f),cos(.3f)};
	float jupiternormal[] ={sin(.1f)*cos(1.f),sin(1.f),cos(.1f)};
	float* earthaxis = moonObit;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1080,1080);
	int windowHandle = glutCreateWindow("Solaris");
	astroid earth(0.5,earthaxis,1.0f),jupiter(0.8f,zaxis,0.01f);
	sun.satelites.push_back(*new obit(4,default_axis,0.1f,earth));
	//sun.satelites.push_back(*new obit(0.5,4,default_axis,0.1f));
	//astroid &earth = sun.satelites[0].mass;
	earth.satelites.push_back(*new obit(0.2,1.5,moonObit,0.5f));
	sun.satelites.push_back(*new obit(12,jupiternormal,0.01f,jupiter));
	const int n=100;
	for(int i=0;i<6;i++){
		float r=1+i*.2;
		jupiter.satelites.push_back(*new obit(0.01,1.0+i*0.2,jupiteraxis,pow(1/r,1.5)));
	}
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(keyup);
	glutMainLoop();
	return 0;
}


