/*  Louis BOUDDHOU
 *  Final Project
 *
 *  Used class exemple 8, 9, 13 and 15 to help
 * 
 *  Key bindings:
 *  m          Modes (orthogonal, perspective, first person navigation)
 *  n          Interrupt light movement
 *  l          Toggle lighting
 *  +/-        Changes field of view for perspective
 *  </>        Move light when stopped
 *  x          Toggle axes on and off
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


#include "CSCIx229.h"

int mode=0;       //  Texture mode
int axes=0;       //  Display axes
int proj_mode=0;       //  Projection mode
int move=1;       //  Move lighting
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int new_angle = 0;
int light=1;      //  Lighting
double moveX = 0.0;
double moveZ = 0.0;
double y_pos = 0.0;

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int walls;
unsigned int tiles;
unsigned int dice[6];



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
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   //  Front
   glBindTexture(GL_TEXTURE_2D,dice[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBindTexture(GL_TEXTURE_2D,dice[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBindTexture(GL_TEXTURE_2D,dice[3]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();

   //  Left
   glBindTexture(GL_TEXTURE_2D,dice[4]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBindTexture(GL_TEXTURE_2D,dice[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBindTexture(GL_TEXTURE_2D,dice[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
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

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,walls);

   //  Cube
   //  Create Base
      // Beige Front
      glColor3f(245/255.0, 241/255.0, 222/255.0);
      glBegin(GL_QUADS);
      glNormal3f( 0, 0, 1);
      glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
      glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
      glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
      glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
      glEnd();
      // Beige Back
      glColor3f(245/255.0, 241/255.0, 222/255.0);
      glBegin(GL_QUADS);
      glNormal3f( 0, 0,-1);
      glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
      glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
      glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
      glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
      glEnd();
      // FFE5CC Right
      glColor3f(255/255.0, 229/255.0, 204/255.0);
      glBegin(GL_QUADS);
      glNormal3f(+1, 0, 0);
      glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
      glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
      glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
      glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
      glEnd();
      // FFE5CC Left
      glColor3f(255/255.0, 229/255.0, 204/255.0);
      glBegin(GL_QUADS);
      glNormal3f(-1, 0, 0);
      glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
      glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
      glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
      glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
      glEnd();
      // Magenta Bottom
      glColor3f(1.0, 0.0, 1.0);
      glBegin(GL_QUADS);
      glNormal3f( 0,-1, 0);
      glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
      glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
      glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
      glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
      glEnd();

   //glEnd();

   glBindTexture(GL_TEXTURE_2D,tiles);
   // Building Rooftop
   glBegin(GL_TRIANGLES);
      // grey front roof
      glColor3f(1,1,1);
      glNormal3f(0,1,2.3);
      glTexCoord2f(0,0); glVertex3f(1,1,1);
      glTexCoord2f(1.5,0); glVertex3f(-1,1,1);
      glTexCoord2f(1,1); glVertex3f(0,2.3,0);

      // dimgrey right roof
      glColor3f(1,1,1);
      glNormal3f(2.3,1,0);
      glTexCoord2f(0,0); glVertex3f(1,1,-1);
      glTexCoord2f(1.5,0); glVertex3f(1,1,1);
      glTexCoord2f(1,1); glVertex3f(0,2.3,0);

      // grey back roof
      glColor3f(1,1,1);
      glNormal3f(0,1,-2.3);
      glTexCoord2f(0,0); glVertex3f(1,1,-1);
      glTexCoord2f(1.5,0); glVertex3f(-1,1,-1);
      glTexCoord2f(1,1); glVertex3f(0,2.3,0);

      // dimgrey left roof
      glColor3f(1,1,1);

      glNormal3f(-2.3,1,0);
      glTexCoord2f(0,0); glVertex3f(-1,1,1);
      glTexCoord2f(1.5,0); glVertex3f(-1,1,-1);
      glTexCoord2f(1,1); glVertex3f(0,2.3,0);      

   // Building door and windows
   // glBegin(GL_QUADS);
   //    // 944C009 Front door
   //    glColor3f(153/255.0, 76/255.0, 0.0);
   //    glNormal3f(0,1,2.3);
   //    glVertex3f(0.8,-1,1.01);
   //    glVertex3f(0.8,0.5,1.01);
   //    glVertex3f(0.2,0.5,1.01);
   //    glVertex3f(0.2,-1,1.01);

   //    // 944C009 Right window
   //    glColor3f(0.0, 191/255.0, 1.0);
   //    glNormal3f(+1, 0, 0);
   //    glVertex3f(1.01,0.5,0.8);
   //    glVertex3f(1.01,0.5,-0.8);
   //    glVertex3f(1.01,-0.1,-0.8);
   //    glVertex3f(1.01,-0.1,0.8);
   glEnd();
   
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}  


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void sphere(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White sphere
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

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
   if (proj_mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else if(proj_mode == 2)
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

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as sphere (still no lighting here)
        glColor3f(1,1,1);
        sphere(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Decide what to draw

   glPushMatrix();
   glScaled(2,2,2);
         glPushMatrix();
         glScaled(0.7,0.7,0.7);
         // row 1
         cube(0,0,0 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0 , 0.1,0.1,0.1 , 0);
         cube(0.42,0,0 , 0.1,0.1,0.1 , 0);
         // row 2
         cube(0,0.21,0 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.21,0 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0 , 0.1,0.1,0.1 , 0);
         // row 3
         cube(0,0.42,0 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.42,0 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0 , 0.1,0.1,0.1 , 0);

         // row 1
         cube(0,0,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.42,0,0.21 , 0.1,0.1,0.1 , 0);
         // row 2
         cube(0,0.21,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.21,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0.21 , 0.1,0.1,0.1 , 0);
         // row 3
         cube(0,0.42,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.42,0.21 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0.21 , 0.1,0.1,0.1 , 0);

         // row 1
         cube(0,0,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.21,0,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.42,0,0.42 , 0.1,0.1,0.1 , 0);
         // row 2
         cube(0,0.21,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.21,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.21,0.42 , 0.1,0.1,0.1 , 0);
         // row 3
         cube(0,0.42,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.21,0.42,0.42 , 0.1,0.1,0.1 , 0);
         cube(0.42,0.42,0.42 , 0.1,0.1,0.1 , 0);
         glPopMatrix();

         // Draw a house
         house(1.5,0.3,0.0 , 0.3,0.3,0.3 , 0);

         // The the sun
         sphere(1.0, 2.0, 1.0, 0.3);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
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
   if (proj_mode == 2) Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection= First Person View",th,ph,dim,fov);
   else Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");

   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   if(move){
      //  Elapsed time in seconds
      double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
      zh = fmod(90*t,360.0);
      //  Tell GLUT it is necessary to redisplay the scene
      glutPostRedisplay();
   }
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (proj_mode == 0 || proj_mode == 1)
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
   Project(proj_mode?fov:0,asp,dim);
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
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      // mode = 1-mode;
      proj_mode = (proj_mode+1)%3;
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
   Project(proj_mode?fov:0,asp,dim);
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
   Project(proj_mode?fov:0,asp,dim);
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
   glutCreateWindow("Textures and Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   walls = LoadTexBMP("wall_bricks.bmp");
   tiles = LoadTexBMP("tile_roof.bmp");
   dice[0] = LoadTexBMP("dice1.bmp");
   dice[1] = LoadTexBMP("dice2.bmp");
   dice[2] = LoadTexBMP("dice3.bmp");
   dice[3] = LoadTexBMP("dice4.bmp");
   dice[4] = LoadTexBMP("dice5.bmp");
   dice[5] = LoadTexBMP("dice6.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
