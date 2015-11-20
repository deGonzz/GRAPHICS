/*  Louis BOUDDHOU
 *  Assignment 4: Projections
 *
 *  Used class exemple 8 and 9 to help
 * 
 *  Key bindings:
 *  m          Modes (orthogonal, perspective, first person navigation)
 *  n          Toggle light movement
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes on and off
 *  ESC        Exit
 *  0          Reset view angle
 *
 * In orthogonal and perspective modes:
 *   arrows     Change view angle 
 *   
 * In first person navigation
 *   arrows     Change view angle on static plane
 *   w/s        Change height
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int new_angle = 0;
double moveX = 0.0;
double moveZ = 0.0;
double y_pos = 0.0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}


/*
   Draw a pyramid
*/
static void house(double x,double y,double z,
   double dx,double dy,double dz,double r)   
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Create Base
      // Beige Front
      glColor3f(245/255.0, 241/255.0, 222/255.0);
      glVertex3f(-1,-1, 1);
      glVertex3f(+1,-1, 1);
      glVertex3f(+1,+1, 1);
      glVertex3f(-1,+1, 1);
      // Beige Back
      glColor3f(245/255.0, 241/255.0, 222/255.0);
      glVertex3f(+1,-1,-1);
      glVertex3f(-1,-1,-1);
      glVertex3f(-1,+1,-1);
      glVertex3f(+1,+1,-1);
      // FFE5CC Right
      glColor3f(255/255.0, 229/255.0, 204/255.0);
      glVertex3f(+1,-1,+1);
      glVertex3f(+1,-1,-1);
      glVertex3f(+1,+1,-1);
      glVertex3f(+1,+1,+1);
      // FFE5CC Left
      glColor3f(255/255.0, 229/255.0, 204/255.0);
      glVertex3f(-1,-1,-1);
      glVertex3f(-1,-1,+1);
      glVertex3f(-1,+1,+1);
      glVertex3f(-1,+1,-1);
      // Magenta Bottom
      glColor3f(1.0, 0.0, 1.0);
      glVertex3f(-1,-1,-1);
      glVertex3f(+1,-1,-1);
      glVertex3f(+1,-1,+1);
      glVertex3f(-1,-1,+1);

   glEnd();

   // Building Rooftop
   glBegin(GL_TRIANGLES);
      // grey front roof
      glColor3f(128/255.0,128/255.0,128/255.0);
      glVertex3f(1,1,1);
      glVertex3f(-1,1,1);
      glVertex3f(0,2.3,0);

      // dimgrey right roof
      glColor3f(105/255.0, 105/255.0, 105/255.0);
      glVertex3f(1,1,-1);
      glVertex3f(1,1,1);
      glVertex3f(0,2.3,0);

      // grey back roof
      glColor3f(128/255.0, 128/255.0, 128/255.0);
      glVertex3f(1,1,-1);
      glVertex3f(-1,1,-1);
      glVertex3f(0,2.3,0);

      // dimgrey back roof
      glColor3f(105/255.0, 105/255.0, 105/255.0);
      glVertex3f(-1,1,1);
      glVertex3f(-1,1,-1);
      glVertex3f(0,2.3,0);      
   glEnd();

   // Building door and windows
   glBegin(GL_QUADS);
      // 944C009 Front door
      glColor3f(153/255.0, 76/255.0, 0.0);
      glVertex3f(0.8,-1,1.01);
      glVertex3f(0.8,0.5,1.01);
      glVertex3f(0.2,0.5,1.01);
      glVertex3f(0.2,-1,1.01);

      // 944C009 Right window
      glColor3f(0.0, 191/255.0, 1.0);
      glVertex3f(1.01,0.5,0.8);
      glVertex3f(1.01,0.5,-0.8);
      glVertex3f(1.01,-0.1,-0.8);
      glVertex3f(1.01,-0.1,0.8);
   glEnd();
   glPopMatrix();
}  


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f( Cos(th), Cos(th),0);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

static void sphere(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else if(mode == 2)
   {
      double Ex = 0 + moveX;
      double Ez = +2*dim + moveZ;
      double Cx = Ex-Sin(new_angle);
      double Cz = Ez-Cos(new_angle);
      gluLookAt(Ex,y_pos,Ez , Cx,y_pos,Cz , 0,1,0);
   }
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Decide what to draw

   glPushMatrix();
   glScaled(2,2,2);
         glPushMatrix();
         glScaled(0.7,0.7,0.7);
         // row 1
         cube(0,0,0 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0 , 0.1,0.1,0.1 , 90);
         cube(0.42,0,0 , 0.1,0.1,0.1 , 180);
         // row 2
         cube(0,0.21,0 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.21,0 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0 , 0.1,0.1,0.1 , 90);
         // row 3
         cube(0,0.42,0 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.42,0 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0 , 0.1,0.1,0.1 , 90);

         // row 1
         cube(0,0,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0.21 , 0.1,0.1,0.1 , 90);
         cube(0.42,0,0.21 , 0.1,0.1,0.1 , 180);
         // row 2
         cube(0,0.21,0.21 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.21,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0.21 , 0.1,0.1,0.1 , 90);
         // row 3
         cube(0,0.42,0.21 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.42,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0.21 , 0.1,0.1,0.1 , 90);

         // row 1
         cube(0,0,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0.42 , 0.1,0.1,0.1 , 90);
         cube(0.42,0,0.42 , 0.1,0.1,0.1 , 180);
         // row 2
         cube(0,0.21,0.42 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.21,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0.42 , 0.1,0.1,0.1 , 90);
         // row 3
         cube(0,0.42,0.42 , 0.1,0.1,0.1 , 180);
         cube(0.21,0.42,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0.42 , 0.1,0.1,0.1 , 90);
         glPopMatrix();

         // Draw a house
         house(1.5,0.3,0.0 , 0.3,0.3,0.3 , 0);

         // The the sun
         sphere(1.0, 2.0, 1.0, 0.3);
   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   glPopMatrix();
   //  Display parameters
   glWindowPos2i(5,5);
   if (mode == 2) Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection= First Person View",th,ph,dim,fov);
   else Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");

   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (mode == 0 || mode == 1)
   {
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP)
         dim += 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
         dim -= 0.1;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   }
   else 
   {
      if (key == GLUT_KEY_RIGHT)
         new_angle -= 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         new_angle += 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP) 
      {
         moveX -= 0.5*Sin(new_angle);
         moveZ -= 0.5*Cos(new_angle);
      }     
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
      {
         moveX += 0.5*Sin(new_angle);
         moveZ += 0.5*Cos(new_angle);
      }
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP)
         dim += 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
         dim -= 0.1;
      //  Keep angles to +/-360 degrees
      new_angle %= 360;
   }

   
   
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angles
   else if (ch == 'w' || ch == 'W')
      y_pos += 0.1;

   else if (ch == 's' || ch == 'S')
      y_pos -= 0.1;

   else if (ch == '0')
      new_angle = moveX = moveZ = y_pos = th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      // mode = 1-mode;
      mode = (mode+1)%3;
   //  Toggle light movement
   else if (ch == 'n' || ch == 'N')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && fov>1)
      fov--;
   else if (ch == '+' && fov<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);


   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
