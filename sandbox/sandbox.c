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
float camera_angle_degrees = 0;


#define X_RESOLUTION 640 
#define Y_RESOLUTION 480
#define LEFT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
#define RIGHT_MOUSE_BUTTON 3


GLsizei cam_pos_x = 0, cam_pos_y = 0, cam_pos_z = 0;
GLsizei light_pos_x = 0, light_pos_y = 0, light_pos_z = 0;
GLsizei cir_offset = 0;
GLsizei speed = 5;

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
}


void display (void)
{


  float sphere_radius;
  float red_sphere_position_x, red_sphere_position_y, red_sphere_position_z;
  float green_sphere_position_x, green_sphere_position_y, green_sphere_position_z;
  float blue_sphere_position_x, blue_sphere_position_y, blue_sphere_position_z;
  float camera_position_x, camera_position_y, camera_position_z;
  float center_x, center_y, center_z;
  float camera_angle_radians;

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
  center_x = (red_sphere_position_x + green_sphere_position_x + blue_sphere_position_x) / 3.0f;
  center_y = -6.0f;
  center_z = (red_sphere_position_z + green_sphere_position_z + blue_sphere_position_z) / 3.0f;
  if (camera_angle_degrees >= 360.0f)
  {
    camera_angle_degrees = 0;
  }
  else
  {
    // camera_angle_degrees = cir_offset * speed;
    camera_angle_degrees = camera_angle_degrees + 1.0f;
  }
  camera_angle_radians = camera_angle_degrees * M_PI / 180.0f;
  camera_position_x = sin(camera_angle_radians) * 6.0f + 6.0f;
  camera_position_y = -6.0f;
  camera_position_z = cam_pos_z + cos(camera_angle_radians) * 6.0f - 4.50f;
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  gluLookAt (camera_position_x, camera_position_y, camera_position_z, center_x, center_y, center_z, 0.0f, 1.0f, 0.0f); // move camera

  glPushMatrix ();
  glTranslatef (red_sphere_position_x, red_sphere_position_y, red_sphere_position_z);
  glColor3f (1.0f, 0.0f, 1.0f);
  glBegin(GL_POLYGON);
    // (float) (rand() / 3.0f)
    glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    glVertex3f((float)(rand() % 3), (float)(rand() % 3), (float)(rand() % 3)); // vertex 1
    // glVertex3f(-0.5f, -0.25f, 0.0f); // vertex 2
    // glVertex3f(0.5f, -0.25f, 0.0f); // vertex 3
    // glVertex3f(1.0f, -1.0f, 0.0f);
  glEnd();
  // glutSolidTeapot(sphere_radius);
  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();

  glPushMatrix ();
  // glRotated(camera_angle_degrees * 2, 0, 0, 0);
  glTranslatef (green_sphere_position_x, green_sphere_position_y, green_sphere_position_z);
  glColor3f (0.0f, 1.0f, 0.0f);
  glutSolidTeapot(sphere_radius);

  // glutSolidSphere (sphere_radius, 50, 50);
  glPopMatrix ();

  glPushMatrix ();
  glTranslatef (blue_sphere_position_x, blue_sphere_position_y, blue_sphere_position_z);
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
  if (x < 20)
  {
    x = 20;
  }
  if (x > (X_RESOLUTION - 20))
  {
    x = X_RESOLUTION - 20;
  }
  if (y < 20)
  {
    y = 20;
  }
  if (y > (Y_RESOLUTION - 20))
  {
    y = Y_RESOLUTION - 20;
  }
  if ((mouse_button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
  {
    // last_mouse_button_pressed = LEFT_MOUSE_BUTTON;
    // left_mouse_button_x = x;
    // left_mouse_button_y = y;
    // glutPostRedisplay ();
  }
  if ((mouse_button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN))
  {
    // last_mouse_button_pressed = MIDDLE_MOUSE_BUTTON;
    // middle_mouse_button_x = x;
    // middle_mouse_button_y = y;
    // glutPostRedisplay ();
  }
  if ((mouse_button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
  {
    // last_mouse_button_pressed = RIGHT_MOUSE_BUTTON;
    // right_mouse_button_x = x;
    // right_mouse_button_y = y;
    // glutPostRedisplay ();
  }
}


void keyboard (unsigned char key, int x, int y)
{

  // printf("Key input: %c\n", key);
  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'w':
      printf("Key input: %c\n", key);
      light_pos_z -= 1.0;
      break;
    case 'a':
      printf("Key input: %c\n", key);
      light_pos_x += 1.0;
      break;
    case 's':
      printf("Key input: %c\n", key);
      light_pos_z += 1.0;
      break;
    case 'd':
      printf("Key input: %c\n", key);
      light_pos_x -= 1.0;
      break;
    default:
    break;
  }

}

void arrow_keys (int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      cam_pos_z -= 1;
      printf("GLUT_KEY_UP cam_pos_z = %d\n", cam_pos_z);
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      cam_pos_z += 1;
      printf("GLUT_KEY_DOWN cam_pos_z = %d\n", cam_pos_z);
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      cir_offset += 1;
      printf("GLUT_KEY_LEFT cir_offset = %d\n", cir_offset);
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      cir_offset -= 1;
      printf("GLUT_KEY_RIGHT cir_offset = %d\n", cir_offset);
      glutPostRedisplay();
      break;
    default:
      break;

  }

  // switch (key)
  // {
  //   case GLUT_KEY_UP:
  //     switch (last_mouse_button_pressed)
  //     {
  //       case LEFT_MOUSE_BUTTON:
  //         if (left_mouse_button_y >= 20)
  //         {
  //           left_mouse_button_y--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case MIDDLE_MOUSE_BUTTON:
  //         if (middle_mouse_button_y >= 20)
  //         {
  //           middle_mouse_button_y--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case RIGHT_MOUSE_BUTTON:
  //         if (right_mouse_button_y >= 20)
  //         {
  //           right_mouse_button_y--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       default:
  //       break;
  //     }
  //   break;
  //   case GLUT_KEY_DOWN:
  //     switch (last_mouse_button_pressed)
  //     {
  //       case LEFT_MOUSE_BUTTON:
  //         if (left_mouse_button_y < Y_RESOLUTION - 20)
  //         {
  //           left_mouse_button_y++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case MIDDLE_MOUSE_BUTTON:
  //         if (middle_mouse_button_y < Y_RESOLUTION - 20)
  //         {
  //           middle_mouse_button_y++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case RIGHT_MOUSE_BUTTON:
  //         if (right_mouse_button_y < Y_RESOLUTION - 20)
  //         {
  //           right_mouse_button_y++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       default:
  //       break;
  //     }
  //   break;
  //   case GLUT_KEY_LEFT:
  //     switch (last_mouse_button_pressed)
  //     {
  //       case LEFT_MOUSE_BUTTON:
  //         if (left_mouse_button_x > 20)
  //         {
  //           left_mouse_button_x--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case MIDDLE_MOUSE_BUTTON:
  //         if (middle_mouse_button_x > 20)
  //         {
  //           middle_mouse_button_x--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case RIGHT_MOUSE_BUTTON:
  //         if (right_mouse_button_x > 20)
  //         {
  //           right_mouse_button_x--;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       default:
  //       break;
  //     }
  //   break;
  //   case GLUT_KEY_RIGHT:
  //     switch (last_mouse_button_pressed)
  //     {
  //       case LEFT_MOUSE_BUTTON:
  //         if (left_mouse_button_x < X_RESOLUTION - 20)
  //         {
  //           left_mouse_button_x++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case MIDDLE_MOUSE_BUTTON:
  //         if (middle_mouse_button_x < X_RESOLUTION - 20)
  //         {
  //           middle_mouse_button_x++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //       case RIGHT_MOUSE_BUTTON:
  //         if (right_mouse_button_x < X_RESOLUTION - 20)
  //         {
  //           right_mouse_button_x++;
  //           glutPostRedisplay ();
  //         }
  //       break;
  //     }
  //   break;
  //   default:
  //   break;
  // }
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

