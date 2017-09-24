/* Gregory Enriquez */

/*
  Compiled on Mac OSX 

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


#define X_RESOLUTION 640 
#define Y_RESOLUTION 480
#define LEFT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
#define RIGHT_MOUSE_BUTTON 3
#define DEBUG_PRINT 1

float camera_angle_degrees = -11;
float camera_position_x = 14, camera_position_y = 7.0f, camera_position_z = -2.0f;
float center_x = 4, center_y = 2, center_z = 5;
float tea_angle = 0;
float tea_radians = 0;
// GLsizei cam_pos_x = 0, cam_pos_y = -8, cam_pos_z = -7;
GLsizei light_pos_x = 0, light_pos_y = 0, light_pos_z = 0;
GLsizei cir_offset = 0;
GLsizei speed = 5;
const GLsizei MAX_LENGTH = 100;
const GLsizei MAX_HEIGHT = 2;
const GLsizei MAX_DEPTH = 100;
GLsizei terrain[MAX_LENGTH][MAX_DEPTH];

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

  // glPushMatrix ();
  // glTranslatef (0, 0, 0);
  // glColor3f (1.0f, 0.0f, 1.0f);
  // glBegin(GL_LINES);
  //   GLsizei x = 0, y = 0, z = 0;
  GLsizei z_max = MAX_DEPTH;
  GLsizei x_max = MAX_LENGTH;
  GLsizei y_max = MAX_HEIGHT;
  for (GLsizei i = 0; i < x_max; i++) {
    for (GLsizei j = 0; j < z_max; j++) {
      terrain[i][j] = rand() % 2;
    }
  }

    // (float) (rand() / 3.0f)
    // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    // glVertex3f(-0.5f, -0.25f, 0.0f); // vertex 2
    // glVertex3f(0.5f, -0.25f, 0.0f); // vertex 3
    // glVertex3f(1.0f, -1.0f, 0.0f);
  glEnd();
  // glutSolidTeapot(sphere_radius);
  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();
  gluLookAt (0, -8, -7, 0, 0, 0, 0.0f, 1.0f, 0.0f); // move camera
}


void display (void)
{
  float sphere_radius;
  float red_sphere_position_x, red_sphere_position_y, red_sphere_position_z;
  float green_sphere_position_x, green_sphere_position_y, green_sphere_position_z;
  float blue_sphere_position_x, blue_sphere_position_y, blue_sphere_position_z;

  float camera_angle_radians = 0;

  sphere_radius = 1.0;
  red_sphere_position_y = -6.0f;
  green_sphere_position_y = -6.0f;
  blue_sphere_position_y = -6.0f;
  red_sphere_position_x = 6.0f;
  red_sphere_position_z = -3.25f;
  green_sphere_position_x = 5.0f;
  green_sphere_position_z = -5.0f;
  blue_sphere_position_x = 7.0f;
  blue_sphere_position_z = -5.0f;
  // center_x = (red_sphere_position_x + green_sphere_position_x + blue_sphere_position_x) / 3.0f;
  // center_y = -6.0f;
  // center_z = (red_sphere_position_z + green_sphere_position_z + blue_sphere_position_z) / 3.0f;
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
  // else
  // {
  //   camera_angle_degrees = camera_angle_degrees + 1.0f;
  // }
  camera_angle_radians = camera_angle_degrees * M_PI / 180.0f;
  // camera_position_x = sin(camera_angle_radians) * 6.0f + 6.0f;
  
  // camera_position_x = cam_pos_x;
  // camera_position_y = cam_pos_y;
  // camera_position_z = cam_pos_z;
  // camera_position_y = 0.0f;    
  // // camera_position_y = -6.0f;


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  GLsizei x = sin(camera_angle_radians) * 50.0f + camera_position_x;
  GLsizei y = camera_position_y;
  GLsizei z = camera_position_z;
  gluLookAt ( x, 
              y, 
              z, 
              center_x, center_y, center_z, 0.0f, 1.0f, 0.0f

              ); // move camera
  // gluLookAt ( sin(camera_angle_radians) * 6.0f + camera_position_x, 
  //             camera_position_y, 
  //             cos(camera_angle_radians) * 6.0f + camera_position_z, 
  //             0, 0, 0, 0.0f, 1.0f, 0.0f

  //             ); // move camera
  // gluLookAt(cam_pos_x, cam_pos_y, cam_pos_z, center_x, center_y, center_z, 0.0f, 1.0f, 0.0f);

  glPushMatrix ();
  glTranslatef (0, 0, 0);
  glColor3f (1.0f, 0.0f, 1.0f);

  // glBegin(GL_LINE_STRIP);  
  glBegin(GL_LINES);
    for (GLsizei j = 1; j < MAX_DEPTH - 1; j++) {
        for (GLsizei i = 1; i < MAX_LENGTH - 1; i++) {
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i+1, terrain[i+1][j], j);
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i+1, terrain[i+1][j+1], j+1);
            glVertex3f(i, terrain[i][j], j);
            glVertex3f(i, terrain[i][j+1], j+1);
        }
    }

  //   // (float) (rand() / 3.0f)
  //   // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
  //   // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
  //   // glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
  //   // glVertex3f(-0.5f, -0.25f, 0.0f); // vertex 2
  //   // glVertex3f(0.5f, -0.25f, 0.0f); // vertex 3
  //   // glVertex3f(1.0f, -1.0f, 0.0f);
  glEnd();
  // glutSolidTeapot(sphere_radius);
  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();

  glPushMatrix ();
  // glRotated(camera_angle_degrees * 2, 0, 0, 0);
  glTranslatef (center_x, center_y, center_z);
  glRotatef(tea_angle, 0, 1, 0);
  glTranslatef (0, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f);
  glutSolidTeapot(sphere_radius);

  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();

  glPushMatrix ();
  glTranslatef (0, 0, 0);
  glColor3f (0.0f, 0.0f, 1.0f);
  glutSolidTeapot(sphere_radius);
  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();

  // GLfloat lightPos1[4]     = {camera_position_x, camera_position_y, camera_position_z, 0.0};
  GLfloat lightPos1[4]     = {light_pos_x, light_pos_y, light_pos_z, 0.0};
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

void mouse (int mouse_button, int state, int x, int y)
{

}


void keyboard (unsigned char key, int x, int y)
{

  #if DEBUG_PRINT
    printCoords();
  #endif DEBUG_PRINT

  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'w':
      camera_position_z++;
      center_z++;
      glutPostRedisplay();
      break;
    case 'a':
      camera_position_x++;
      center_x++;
      glutPostRedisplay();
      break;
    case 's':
      camera_position_z--;
      center_z--;
      glutPostRedisplay();
      break;
    case 'd':
      camera_position_x--;
      center_x--;
      glutPostRedisplay();
      break;
    default:
    break;
  }

}

void arrow_keys (int key, int x, int y)
{

  #if DEBUG_PRINT
    printCoords();
  #endif DEBUG_PRINT

  switch (key)
  {
    case GLUT_KEY_UP:
      camera_position_y++;
      center_y++;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      camera_position_y--;
      center_y--;
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      camera_angle_degrees--;
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      camera_angle_degrees++;
      glutPostRedisplay();
      break;
    default:
      break;
  }
}

void printCoords() {
  printf("D/Camera - X: %.2f Y: %.2f Z: %.2f theta: %.2f tea-angle: %.2f Center - X: %.2f Y: %.2f Z: %.2f\n", 
    camera_position_x, camera_position_y, camera_position_z, camera_angle_degrees, tea_angle, center_x, center_y,
    center_z); 
}


int main (int argc, char *argv[]) 
{
  int window;

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize (1280, 720 ); 
  glutInitWindowPosition (0, 0);
  window = glutCreateWindow ("Flying Camera Demo");
  init ();
  glutDisplayFunc (display);  
  glutReshapeFunc (reshape);
  glutMouseFunc (mouse);
  glutKeyboardFunc (keyboard);
  glutSpecialFunc (arrow_keys);
  glutMainLoop ();
}

