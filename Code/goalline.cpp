#include <stdlib.h> // standard definitions
#include <math.h> // math definitions
#include <stdio.h> // standard I/O
#include <GL/glut.h> //glut library
#include "net.h"
// escape key (for exit)
#define ESC 27
#define SPACE 32  //space key for shooting
float x = 0.0, y = -4.4,z=0.0; // initiallz 3 units south of origin
float deltaMove = 0.0; // initiallz camera doesn't move
GLUquadricObj *IDquadric; //for the cylinders
void cleanupQuadric(void);
// Camera direction
float lx = 0.0, lz=0.0,ly=0;; // camera points initiallz along y-axis
int updateindex=0;
int cameraindex=1;  //determines where to place the camera
int motionindex=0;  //determines whether the ball is in motion or not
int goalindex=0;  //determines whether the ball has crossed the line or not
int startindex=0;  //used to start the simulation
int collisionindex=0;  //to check whether the ball has collided with the nets or not
int count=0;    //to make sure that the ball moves only once
class ball            //controls the position of the ball and its movement
{
private:
  float velocity;
  float pos[3];
public:
  float det;  //used for goal detection
  ball(float x,float y,float z);  //used to set the ball position
	void drawball();   //drawing the ball
  void moveball();   //controls the motion of the ball
  void setvelocity(float vel);    //sets the initial velocity of the ball
  void reset1(float,float,float);   //resets the whole scene
};
ball::ball(float x,float y,float z)
{
  pos[0]=x;
  pos[1]=y;
  pos[2]=z;
  velocity = 0;
  det=pos[1]-0.25;
}
void ball::drawball()
{
	glColor3f(1.0, 1.0, 1.0); // set drawing color to white
	glPushMatrix();    //draw the ball as a sphere
		glTranslatef(pos[0], pos[1], pos[2]+0.25);  //keep it above the ground
    glutSolidSphere(0.25, 10, 10);
		glColor3f(0.12,0.23,0.5);
		glutWireSphere(0.25,10,10);
	glPopMatrix();
	glColor3f(0.0, 0.5, 0.0);
	glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]+0.25);	// translate to just above ground
		glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
		glutSolidSphere(0.25, 10, 10); // shadow same size as body
	glPopMatrix();
}
void ball::moveball()
{
	if(velocity>0&&pos[1]<13.8&&!collisionindex&&startindex)
  {
  	pos[1]+=velocity;    //increment the position of the ball depending on the velocity
  	det=pos[1]-0.25;
  }
  else if(velocity>0)    //for the rebound after hitting the nets
  {
    collisionindex=1;
    velocity-=0.05;
    pos[1]-=velocity;
  }
  else                //stop the motion of the ball
  {
    motionindex=0;
  }
  if(det>=12.1)       //ball has crossed the line completely
    goalindex=1;
  else                //ball has not crossed the line
    goalindex=0;
  velocity-=0.005;    //constant rate of deceleration

}
void ball::setvelocity(float vel) //set velocity initially
{
  velocity=vel;
}
void ball::reset1(float x,float y,float z)    //reset the position of the ball
{
  pos[0]=x;
  pos[1]=y;
  pos[2]=z;
}
ball b(0,0,0);    //create the ball object
//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y,z) are the current camera position. The values (lx, lz, ly)
// point in the direction the camera is looking.
// Camera position
// Reshape callback
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)  //reshape function
{
  glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	float ratio =  ((float) w) / ((float) h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
}
void timer(int value)     //timer function to help in motion of the ball
{
  if(motionindex==1)
  {
    b.moveball();
	  glutPostRedisplay();
    glutTimerFunc(10,timer,0);
  }

}
void drawObjects()
{
  //Draw the ball
	b.drawball();
	// Goal Posts
	//Goal Post on the right hand side
	glPushMatrix();
		glColor3f(0.9,0.9,1);
		glTranslatef(4,12,0);
		gluCylinder(IDquadric,0.1,0.1,3,3,3);
	glPopMatrix();
	//Goal Post on the left hand side
	glPushMatrix();
		glColor3f(0.9,0.9,1);
		glTranslatef(-4,12,0);
		gluCylinder(IDquadric,0.1,0.1,3,3,3);
	glPopMatrix();
	//Goal Post on the top
	glPushMatrix();
		glColor3f(0.9,0.9,1);
		glTranslatef(-4.1,12,3);
		glRotatef(90,0,12,0);
		gluCylinder(IDquadric,0.1,0.1,8.2,3,3);
	glPopMatrix();
}
void reset()    //reset the whole scene
{
  updateindex=0;
  cameraindex=1;
  b.setvelocity(0);
  b.reset1(0,0,0);
  motionindex=0;
  goalindex=0;
  startindex=0;
  collisionindex=0;
  count=0;
  x=0;y=-4.4;z=0;
  lx=0;ly=0.0;lz=0;
}
void timer1()   //function to call timer()
{
  if(motionindex==1&&startindex==1)
  {
    timer(0);
  }
}
//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementallz moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void side()   //camera movements for side view
{
  if (deltaMove) { // update camera position
    x +=lx;
}
}
void top()    //camera movements for top view
{
  if (deltaMove) { // update camera position
    z+=lz;	}
}
void normal()   //camera movements for original view
{
  if (deltaMove) { // update camera position
    y += ly;
  }
}
void update(void)   //calling one of the camera update functions
{
	switch(updateindex)
  {
    case 1:top();break;
    case 2:side(); break;
    default:normal();
  }
	glutPostRedisplay(); // redisplay everything
}
goalpost goal1(4,-4,14,12,3,0);   //draw goalpost components
//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{
	float i, j;
	// Clear color and depth buffers
	glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	IDquadric=gluNewQuadric();      // Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(IDquadric, GLU_SMOOTH);  // Create Smooth Normals ( NEW )
	gluQuadricTexture(IDquadric, GL_TRUE);    // Create Texture Coords ( NEW )
	atexit(cleanupQuadric);
	// Reset transformations
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, lz, 0), with the z-axis pointing up
  switch(cameraindex)   //change the camera views
  {
    case 1://original view
          gluLookAt(
            x,      y,     1,
            x + lx, 12+ly, 1,
            0.0,    0.0,    1	);
          glPushMatrix();
            displaybox(7.5,12,5,13.5,12,7);   //display the scoreboard
          glPopMatrix();
          glPushMatrix();
            glColor3f(0,1,0);
            drawStrokeText(7,10,5.25,"Result:");  //display result in the scoreboard
          glPopMatrix();
          if(goalindex==1&&motionindex==0)    //display goal if ball has crossed the line
          {
            glPushMatrix();
              glColor3f(0,1,0);
              drawStrokeText(8.75,10,5.25,"Goal!!!");
            glPopMatrix();
          }
          if(goalindex==0&&motionindex==0&&startindex==1)   //display no goal if ball hasn't crossed the line
          {
            glPushMatrix();
              glColor3f(0,1,0);
              drawStrokeText(8.75,10,5.25,"No Goal!");
            glPopMatrix();
          }
          //display player-required information on the screen
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(5,10,2,"t/T-Top View Review");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(5,10,1.55,"s/S-Side View Review");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-30,40,7,"1- Very Low Speed");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-30,40,5.5,"2 - Low Speed");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-30,40,4,"3- Moderate Speed");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-30,40,2.5,"4- High Speed");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-30,40,1,"5- Very High Speed");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(5,10,1.1,"r/R- Reset");
          glPopMatrix();
          glPushMatrix();
            glColor3f(0.8,1,0.8);
            drawStrokeText(-0.8,-2,0.1,"Select Speed and Press Space to shoot");
          glPopMatrix();
          glFlush();
          break;
    case 2:	//top View
    	     gluLookAt(
             0,      12,     z+2,
             0.0299955+0.00824877, 12, z+lz,
             0.0,    0.0,    1	);
          break;
    case 3: //side View
            gluLookAt(
              x+3.8,      12,     0.25,
              x + lx, 12, 0.25,
              0.0,    0.0,    1	);
          glPushMatrix();
            displaybox(-6,8,1,-6,10,2);
          glPopMatrix();
          if(goalindex==1&&motionindex==0)
          {
            glPushMatrix();
              glColor3f(0,1,0);
              drawStrokeText(-5.99,8.35,1.4,"Goal!!!");
            glPopMatrix();
          }
          if(goalindex==0&&motionindex==0&&startindex==1)
          {
            glPushMatrix();
              glColor3f(0,1,0);
              drawStrokeText(-5.99,8.2,1.4,"No Goal!");
            glPopMatrix();
          }
          glFlush();
          break;
  }

	// Draw ground - 200x200 square colored green
	glColor3f(0.0, 0.7, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-100.0, -100.0, 0.0);
		glVertex3f(-100.0,  100.0, 0.0);
		glVertex3f( 100.0,  100.0, 0.0);
		glVertex3f( 100.0, -100.0, 0.0);
	glEnd();
	// Draw football
	glPushMatrix();
  drawObjects();
	glPopMatrix();
	  //Draw Goal Post
	glPushMatrix();
		glLineWidth(10);
		goal1.line();
		glLineWidth(0.1);
		goal1.nets();
	glPopMatrix();

	glutSwapBuffers(); // Make it all visible
	glutPostRedisplay();
}
void cleanupQuadric(void)          // Properly Kill The Window
{
  gluDeleteQuadric(IDquadric);
}
//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy) //processing keyboard inputs for motion of the ball
{
	switch(key)
	{
		case 'q':
		case ESC:exit(0);
    case '5': count+=1; if(count>1) break; b.setvelocity(0.5); break;
    case '4': count+=1; if(count>1) break; b.setvelocity(0.349);break;
    case '3': count+=1; if(count>1) break; b.setvelocity(0.343); break;
    case '2': count+=1; if(count>1) break; b.setvelocity(0.3415); break;
    case '1': count+=1; if(count>1) break; b.setvelocity(0.2); break;
		case SPACE: startindex=1; motionindex=1; timer(0);  //start the motion of the ball
                break;
    case 't':
    case 'T':updateindex=1;cameraindex=2;lz=0;z=0;x=0;y=-4.5;break;   //change camera position to top view
    case 's':
    case 'S':updateindex=2;cameraindex=3;lx=0;z=0;x=0;y=-4.5;break;   //change camera position to side view
    case 'r':reset();break;   //reset the scene
  }

}
void pressSpecialKey(int key, int xx, int yy)   //processing keyboard inputs for motion of the camera
{
	switch (key) {
		case GLUT_KEY_UP ://move camera forward
                        deltaMove =+1 ;
                        if(cameraindex==1) ly=0.05; //if original view move along y-axis
                        else if(cameraindex==2) lz=-0.05; //if top view move along z-axis
                        else lx=-0.05;    //if side view move along x-axis
                        break;
		case GLUT_KEY_DOWN : //move camera backwards
                          deltaMove = +1;
                          if(cameraindex==1) ly=-0.05;    //if original view move along y-axis
                          else if(cameraindex==2) lz=+0.05;   //if top view move along z-axis
                          else lx=+0.05;  //if side view move along x-axis
                          break;
	}
}

void releaseSpecialKey(int key, int x, int y) //function to stop motion of the camera when key is released
{
	switch (key) {
		case GLUT_KEY_UP : deltaMove = 0.0; break;
		case GLUT_KEY_DOWN : deltaMove = 0.0; break;
	}
}
//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	printf("\n\
-----------------------------------------------------------------------\n\
  - Hold up-arrow/down-arrow to move camera forward/backward\n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");
	// general initializations
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 400);
	glutCreateWindow("Simulation of Goal-Line Technology");
	// register callbacks
	glutReshapeFunc(changeSize); // window reshape callback
	glutDisplayFunc(renderScene); // (re)display callback
	glutIdleFunc(update); // incremental update
	glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
	glutKeyboardFunc(processNormalKeys); // process standard key clicks
	glutSpecialFunc(pressSpecialKey); // process special key pressed
	glutSpecialUpFunc(releaseSpecialKey); // process special key release
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	// enter GLUT event processing cycle
	glutMainLoop();
	return 0; // this is just to keep the compiler happy
}
