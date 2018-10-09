#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gl/glut.h"
#include "visuals.h" 

model md;
static float starglare = 2.0; //star glare size
static float starop = 0.0;  //star glare op
static float sunglare = 30.0; //sun glare size
static float sunop = 0.0; //sun glare op
static int isgrowing = -1;
static int isgrowingSun = 0;
static int i = 0;
static int stars = 100;
static bool firsttime = 1;
static int x[100]; //star x-pos
static int y[100]; //star y-pos
static float rotx = 0.0;
static float roty = 0.0;
static float camerax = 0.0;
static float cameray = 0.0;
static bool animate = true;

using namespace std;

void Stars() //100 asteria
{
	if (firsttime == 1) //happens only once
	{
		srand(time(NULL));
		for (i = 0; i < stars; i++)
		{
			x[i] = rand() % 1000 - 500;//from -500 to 500
			y[i] = rand() % 1000 - 500;//from -500 to 500
		}
		firsttime = 0;
	}
	for (i = 0; i < stars; i++)
	{
		//star
		glPushMatrix();
		GLUquadric *quad;
		quad = gluNewQuadric();
		glTranslatef(x[i], y[i], -200);
		glColor4f(1.0, 1.0, 1.0, 1);
		gluSphere(quad, 2, 20, 20);
		glPopMatrix();
		//star glare
		glPushMatrix();
		glTranslatef(x[i], y[i], -200);
		glColor4f(1.0, 1.0, 1.0, starop);
		gluSphere(quad, starglare, 20, 20);
		glPopMatrix();
	}
}

void Render()
{    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();

  glTranslatef(0, 0, -200);
  glRotatef(camerax, 1, 0, 0);
  glRotatef(cameray, 0, 1, 0);

  Stars();
  //Sun
  glPushMatrix();
  GLUquadric *quad;
  quad = gluNewQuadric();
  glTranslatef(0.0, 0.0, 0);
  glColor4f(0.9, 0.9, 0.0, 1.0);
  gluSphere(quad, 30, 50, 50);
  glPopMatrix();

  //Around Sun
  glPushMatrix();
  glTranslatef(0.0, 0.0, 0);
  glColor4f(0.9, 0.9, 0.0, sunop);
  gluSphere(quad, sunglare, 50, 50);
  glPopMatrix();

  //Red planet
  glPushMatrix();
  glRotatef(roty, 0, 1, 0);
  glTranslatef(95.0, 0.0, -50);
  glRotatef(roty, 0, 1, 0);
  glScalef(0.04, 0.04, 0.04);
  glColor3f(2.0, 0.0, 0.0);	
  DisplayModel(md);
  glPopMatrix();
  
  //Blue planet
  glPushMatrix();
  //Red Planet's Movement
  glRotatef(roty, 0, 1, 0);
  glTranslatef(95.0, 0.0, -50);
  glRotatef(roty, 0, 1, 0);
  //Blue Planet's Movement
  glRotatef(roty, 0, 1, 0);
  glTranslatef(7.0, 0.0, -50);
  glScalef(0.02, 0.02, 0.02);
  glColor3f(0.0, 0.0, 1.0);
  DisplayModel(md);
  glPopMatrix();

  glutSwapBuffers();
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{
	if (animate)
	{
		rotx += 0.4;
		roty += 0.4;
		if (starglare >= 7.0)
		{
			isgrowing = -1;    //an einai sthn maximum timh ths diametrou tote arxizei na meiwnetai

		}
		else if (starglare <= 2.0)
		{
			isgrowing = 1;		//an einai sthn miniimum timh ths diametrou tote arxizei na auksanetai
		}
		starglare = starglare + isgrowing*(0.03);
		starop = starop + isgrowing*(0.002);
		if (sunglare >= 38.0)
		{
			isgrowingSun = -1;		//an einai sthn maximum timh ths diametrou tote arxizei na meiwnetai
		}
		else if (sunglare <= 30.0)
		{
			isgrowingSun = 1;		//an einai sthn miniimum timh ths diametrou tote arxizei na auksanetai
		}
		sunglare = sunglare + isgrowingSun*(0.04);
		sunop = sunop + isgrowingSun*(0.004);
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'p' : animate = !animate;
		break;
	case 'w': camerax += 0.6;
		break;
	case 'a': cameray -= 0.6;
		break;
	case 's': camerax -= 0.6;
		break;
	case 'd': cameray += 0.6;
		break;
	default : break;
	}

	glutPostRedisplay();

}


void Setup()
{ 
	ReadFile(&md);

	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 30.0, 50.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
 

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void ReadFile(model *md)
{

	ifstream obj_file("planet.obj");

	if (obj_file.fail())
		exit(1);

	md->vertices = 0;
	md->faces = 0;
	md->normals = 0;

	string line;

	while (getline(obj_file, line)) {

		if (line.length() > 0) {

			if ((line.at(0) == 'v')) {
				if (line.at(1) == ' ') {
					istringstream stm(line);
					stm.ignore(1, ' ');
					stm >> md->obj_points[md->vertices].x;
					stm >> md->obj_points[md->vertices].y;
					stm >> md->obj_points[md->vertices].z;
					md->vertices++;
				}
				else if (line.at(1) == 'n') {
					istringstream stm(line);
					stm.ignore(2, ' ');	
					stm >> md->obj_normals[md->normals].x;
					stm >> md->obj_normals[md->normals].y;
					stm >> md->obj_normals[md->normals].z;
					md->normals++;
				}
			}
			else if (line.at(0) == 'f') {
				int pos;
				pos = line.find("//");
				line.replace(pos, 2, "  ");
				pos = line.find("//", pos + 1);
				line.replace(pos, 2, "  ");
				pos = line.find("//", pos + 1);
				line.replace(pos, 2, "  ");
				istringstream stm(line);
				stm.ignore(1, ' ');
				stm >> md->obj_faces[md->faces].vtx[0];
				stm >> md->obj_faces[md->faces].norm[0];
				stm >> md->obj_faces[md->faces].vtx[1];
				stm >> md->obj_faces[md->faces].norm[1];
				stm >> md->obj_faces[md->faces].vtx[2];
				stm >> md->obj_faces[md->faces].norm[2];
				md->faces++;
			}
		}
	}
	obj_file.close();
}


void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();

}