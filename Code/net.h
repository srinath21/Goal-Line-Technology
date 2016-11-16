#ifndef _NET_H
#define _NET_H
#include<stdio.h>
#include<GL/glut.h>
#include<iostream>
#include<string.h>
using namespace std;
//class for creating components of goalpost
class goalpost
{
private:
	int xmax,xmin; //specifies the maximum & minimum coordinates along which the goalpost must line
	int ymax,ymin;
	int zmax,zmin;
public:
	goalpost(int,int,int,int,int,int);
	void line();
	void nets();
};
goalpost::goalpost(int xm,int xmi,int ym,int ymi,int zm,int zmi)
{
	xmax=xm;
	xmin=xmi;
	ymax=ym;
	ymin=ymi;
	zmax=zm;
	zmin=zmi;
}

//Draw White Line used for goal-line technology

void goalpost::line()
{
	glPushMatrix();
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glVertex3f(-4,11.9,0.0001);
	glVertex3f(-4,12.1,0.0001);
	glVertex3f(4,12.1,0.0001);
	glVertex3f(4,11.9,0.0001);
	glEnd();
	glFlush();
	glPopMatrix();
}
//This is used to generate nets for the goal post
void goalpost::nets()
{
  float i,j;
  //Nets to be generated on the left side of the goal post
	glPushMatrix();
	for(i=ymin;i<=ymax;i+=0.15)
	{
		for(j=zmin;j<=zmax;j+=0.15)
		{
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_LINE_LOOP);
			glVertex3f(-4,i,j);
			glVertex3f(-4,i,j+0.15);
			glVertex3f(-4,i+0.15,j+0.15);
			glVertex3f(-4,i+0.15,j);
			glEnd();
			glFlush();
		}
		glFlush();
	}
	glPopMatrix();
  //Nets to be generated at back
	glPushMatrix();
	for(i=xmin;i<xmax;i+=0.15)
	{
		for(j=zmin;j<=zmax;j+=0.15)
		{
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_LINE_LOOP);
			glVertex3f(i,14,j);
			glVertex3f(i,14,j+0.15);
			glVertex3f(i+0.15,14,j+0.15);
			glVertex3f(i+0.15,14,j);
			glEnd();
			glFlush();
		}
		glFlush();
	}
	glPopMatrix();
  //Nets to be generated at the right side of the goal post.
	glPushMatrix();
	for(i=ymin;i<=ymax;i+=0.15)
	{
		for(j=zmin;j<=zmax;j+=0.15)
		{
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_LINE_LOOP);
			glVertex3f(4,i,j);
			glVertex3f(4,i,j+0.15);
			glVertex3f(4,i+0.15,j+0.15);
			glVertex3f(4,i+0.15,j);
			glEnd();
			glFlush();
		}
		glFlush();
	}
	glPopMatrix();
	//Nets at the top of the goal post
	glPushMatrix();
	for(i=xmin;i<xmax;i+=0.15)
	{
		for(j=ymin;j<ymax;j+=0.15)
		{
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_LINE_LOOP);
			glVertex3f(i,j,3);
			glVertex3f(i,j+0.15,3);
			glVertex3f(i+0.15,j+0.15,3);
			glVertex3f(i+0.15,j,3);
			glEnd();
			glFlush();
		}
		glFlush();
	}
	glPopMatrix();
}
void displaybox(float x1, float y1, float z1, float x2, float y2, float z2)
{
		glPushMatrix();
		glColor3f(0,0,0);
		glBegin(GL_QUADS);
		glVertex3f(x1,y1,z1);
		glVertex3f(x1,y1,z2);
		glVertex3f(x2,y2,z2);
		glVertex3f(x2,y2,z1);
		glEnd();
		glFlush();
		glPopMatrix();
}
void drawStrokeText(float x,float y,float z,char *string)
{
 char *c;
 glRasterPos3f(x,y,z);

 for(c=string;*c!='\0';c++)
 {	glColor3f(0.0,0.0,1);
	 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
 }
}

#endif
