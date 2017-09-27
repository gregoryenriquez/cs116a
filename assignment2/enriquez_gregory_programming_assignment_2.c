/* Gregory Enriquez */

#define CONTROL_MODE 2 // 1 - WASD controls, 2 - Keyboard arrow controls

/*
  Compiled on Mac OSX
  gcc -Wno-deprecated-declarations  -o enriquez_gregory_programming_assignment_2  enriquez_gregory_programming_assignment_2.o  -framework GLUT -framework OpenGL -framework Cocoa -lm

  Control Mode 1 - WASD movement
  --------
  W - Move forward (Z-axis)
  A - Move backward (Z-axis)
  S - Move / strafe left (X-axis)
  D - Move / strafe right (X-axis)
  Q - Roll teapot left
  E - Roll teapot right
  Up - Move upward (Y-axis)
  Down - Move downward (Y-axis)
  Left - Rotate camera left 15 degrees
  Right - Rotate camera right 15 degrees

  Control Mode 2 - Arrow movement
  --------------
  Up - Move forward (Z-axis)
  Down - Move backward (Z-axis)
  Left - Move / strafe left (X-axis)
  Right - Move / strafe right (X-axis)
  Q - Roll teapot left
  E - Roll teapot right
  A - Move upward (Y-axis)
  Z - Move downward (Y-axis)
  S - Rotate camera left 15 degrees
  D - Rotate camera right 15 degrees
*/

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#elif __linux__
  #include <GL/glut.h>
#endif
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define X_RESOLUTION 640 
#define Y_RESOLUTION 480
#define DEBUG_PRINT 0

const GLsizei MAX_LENGTH = 100;
const GLsizei MAX_HEIGHT = 10;
const GLsizei MAX_DEPTH = 100;

float camera_angle_degrees = 0;
float center_x = 50, center_y = 2, center_z = 1;
float tea_angle = 0;
float tea_radians = 0;
float terrain[MAX_LENGTH][MAX_DEPTH];
float roll_degrees = 0;
float roll_right = 1;
float sphere_radius;
float camera_angle_radians = 0;
GLsizei barrel = 0;

void printCoords();

void init (void)
{

  glShadeModel (GL_SMOOTH);
  glClearColor (1.0f, 1.0f, 1.0f, 0.0f);        
  glClearDepth (1.0f);
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  glEnable (GL_COLOR_MATERIAL);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  GLfloat lightPos[4] = {-1.0, 1.0, 0.5, 0.0};
  glLightfv (GL_LIGHT0, GL_POSITION, (GLfloat *) &lightPos);
  glEnable (GL_LIGHT1);
  GLfloat lightAmbient1[4] = {0.0, 0.0,  0.0, 0.0};
  GLfloat lightPos1[4]     = {1.0, 0.0, -0.2, 0.0};
  GLfloat lightDiffuse1[4] = {0.5, 0.5,  0.3, 0.0};
  glLightfv (GL_LIGHT1,GL_POSITION, (GLfloat *) &lightPos1);
  glLightfv (GL_LIGHT1,GL_AMBIENT, (GLfloat *) &lightAmbient1);
  glLightfv (GL_LIGHT1,GL_DIFFUSE, (GLfloat *) &lightDiffuse1);
  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  time_t t;
  srand((unsigned) time(&t));

  /* Generate random heights for terrain vertices */
  for (GLsizei i = 0; i < MAX_LENGTH; i++) {
    for (GLsizei j = 0; j < MAX_DEPTH; j++) {
      terrain[i][j] = 0.1f * (rand() % MAX_HEIGHT);
    }
  }
}


void display (void)
{
  sphere_radius = 1.0;

  if (tea_angle >= 360.0f) {
    tea_angle = 0;
  } else {
    tea_angle = tea_angle + 1.0f;
  }
  tea_radians = tea_angle * M_PI / 180.0f;

  if (camera_angle_degrees >= 360.0f)
  {
    camera_angle_degrees = 0;
  }
  camera_angle_radians = camera_angle_degrees * M_PI / 180.0f;


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();

  gluLookAt ( sin(camera_angle_radians) * 6.0f + center_x, 
              center_y + 3.0f, 
              -cos(camera_angle_radians) * 6.0f + center_z, 
              center_x, center_y, center_z, 0.0f, 1.0f, 0.0f
              ); // move camera

  glPushMatrix ();
  glTranslatef (0, 0, 0);
  glColor3f (1.0f, 0.0f, 0.0f);

  glBegin(GL_LINES);
    for (GLsizei j = 0; j < MAX_DEPTH; j++) {
        for (GLsizei i = 0; i < MAX_LENGTH; i++) {
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i+1, terrain[i+1][j], j);
            glColor3f (1.0f, 0.0f, 1.0f);
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i+1, terrain[i+1][j+1], j+1);
            glColor3f (0.0f, 0.0f, 1.0f);
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i, terrain[i][j+1], j+1);
            glColor3f (1.0f, 0.0f, 0.0f);

            if (i + 1 == MAX_LENGTH && j + 1 < MAX_DEPTH) {
               glVertex3f(i+1, terrain[i+1][j], j); 
               glVertex3f(i+1, terrain[i+1][j+1], j+1); 
            }
        }
    }

  glEnd();
  glPopMatrix ();

  glPushMatrix ();
  glTranslatef (center_x, center_y, center_z);

  /* Do a barrel roll! */
  if (barrel) {
    if (roll_degrees >= 360.0f || roll_degrees <= -360.0f) {
      barrel = 0;
      roll_degrees = 0;
    } else {
      if (roll_right) {
        roll_degrees = roll_degrees + 5.0f;
        glRotatef(roll_degrees, 0, 0, 1);  
      } else {
        roll_degrees = roll_degrees - 5.0f;
        glRotatef(roll_degrees, 0, 0, 1);  
      }
    }
  } else {
    glRotatef(tea_angle, 0, 1, 0);
  }

  glTranslatef (0, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f);
  glutSolidTeapot(sphere_radius);
  glPopMatrix ();


  GLfloat lightPos1[4] = {0, 0, 0, 0.0}; 
  glLightfv (GL_LIGHT1,GL_POSITION, (GLfloat *) &lightPos1);

  glutSwapBuffers();
  glutPostRedisplay();
}

void reshape (int w, int h)  
{
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION); 
  glLoadIdentity ();  
  if (h == 0)  
  { 
    gluPerspective (80, (float) w, 1.0, 5000.0);
  }
  else
  {
    gluPerspective (80, (float) w / (float) h, 1.0, 5000.0); // near and far clipping planes
  }
  glMatrixMode (GL_MODELVIEW);  
  glLoadIdentity (); 
}

void keyboard (unsigned char key, int x, int y)
{

  #if CONTROL_MODE==1
  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'w':
      if (center_z + 1 < MAX_DEPTH) {
        center_z++;
      }
      break;
    case 'a':
      if (center_x + 1 < MAX_LENGTH) {
        center_x++;
      }
      break;
    case 's':
      if (center_z - 1 > 0) {
        center_z--;
      }
      break;
    case 'd':
      if (center_x - 1 > 0) {
        center_x--;
      }
      break;
    case 'e':
      if (!barrel) {
        barrel = barrel + 1;
        roll_right = 1;
      }
      break;
    case 'q':
      if (!barrel) {
        barrel = barrel - 1;
        roll_right = 0;        
      }
      break;
    default:
    break;
  }
  #endif

  #if CONTROL_MODE==2
  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'a':
      if (center_y < 100) {
        center_y++;
      }
      break;
    case 'z':
      if (center_y - 1 > 0) {
        center_y--;
      }
      break;
    case 's':
      camera_angle_degrees += 15;
      break;
    case 'd':
      camera_angle_degrees -= 15;
      break;
    case 'e':
      if (!barrel) {
        barrel = barrel + 1;
        roll_right = 1;
      }
      break;
    case 'q':
      if (!barrel) {
        barrel = barrel - 1;
        roll_right = 0;        
      }
      break;
    default:
    break;
  }
  #endif

  #if DEBUG_PRINT
    printCoords();
  #endif 

}

void arrow_keys (int key, int x, int y)
{

  #if CONTROL_MODE==1
  switch (key)
  {
    case GLUT_KEY_UP:
      if (center_y < 100) {
        center_y++;
      }
      break;
    case GLUT_KEY_DOWN:
      if (center_y - 1 > 0) {
        center_y--;
      }
      break;
    case GLUT_KEY_LEFT:
      camera_angle_degrees += 15;
      break;
    case GLUT_KEY_RIGHT:
      camera_angle_degrees -= 15;
      break;
    default:
      break;
  }
  #endif

  #if CONTROL_MODE==2
  switch (key)
  {
    case GLUT_KEY_UP:
      if (center_z + 1 < MAX_DEPTH) {
        center_z++;
      }
      break;
    case GLUT_KEY_DOWN:
      if (center_z - 1 > 0) {
        center_z--;
      }

      break;
    case GLUT_KEY_LEFT:
      if (center_x + 1 < MAX_LENGTH) {
        center_x++;
      }
      break;
    case GLUT_KEY_RIGHT:
      if (center_x - 1 > 0) {
        center_x--;
      }
      break;
    default:
      break;
  }
  #endif

    #if DEBUG_PRINT
    printCoords();
    #endif 
}

void printCoords() {
  printf("D/degrees: %.2f tea-angle: %.2f Center - X: %.2f Y: %.2f Z: %.2f\n", camera_angle_degrees, tea_angle, 
    center_x, center_y, center_z);   
}


int main (int argc, char *argv[]) 
{
  int window;

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize (1280, 720 ); 
  glutInitWindowPosition (0, 0);
  window = glutCreateWindow ("Flying Teapot Camera Demo");
  init ();
  glutDisplayFunc (display);  
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutSpecialFunc (arrow_keys);
  glutMainLoop ();
}

