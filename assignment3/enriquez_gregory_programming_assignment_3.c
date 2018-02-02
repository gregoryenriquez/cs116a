/* Gregory Enriquez */

/*
  Compiled on Mac OSX

  Build commands:
  gcc -Wno-deprecated-declarations  -c enriquez_gregory_programming_assignment_3.c  -o enriquez_gregory_programming_assignment_3.o
  gcc -Wno-deprecated-declarations  -o enriquez_gregory_programming_assignment_3 enriquez_gregory_programming_assignment_3.o  -framework GLUT -framework OpenGL -framework Cocoa -lm

  Controls:
  Up Arrow - Increase camera Y position
  Down Arrow - Decrease camera Y position
  Left Arrow - Rotate camera left around center (bunny)
  Right Arrow - Rotate camera right around center
  w - Change to Wire display mode
  s - Change to Solid display mode
  a - Change to Solid display mode with wire frame

  Input:
  Accepts additional parameter different .obj files, e.g. "./program frog.obj" but defaults to "bunny.obj"

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
#include <string.h>

#define DEBUG 0
#define X_RESOLUTION 640 
#define Y_RESOLUTION 480
const int MAX_INPUT_VERTICES = 3000;
const int MAX_INPUT_FACES = 5000;
const float MOVE_STEP = 0.2f;
const int SCALE = 10;

volatile int mode = 1; // 1 = wire, 2 = fill, 3 = file + wire

char model_file_path[256] = "bunny.obj";
float camera_angle_degrees = 0;
float center_x = -0.02f, center_y = 1.0f, center_z = 0.009f;
float camera_angle_radians = 0;
float vertices[MAX_INPUT_VERTICES][3] = {0}; 
int faces[MAX_INPUT_FACES][3] = {0};
int vCount = 1;
int fCount = 0;



void init (char* file_path)
{

  glShadeModel (GL_SMOOTH);
  glClearColor (1.0f, 1.0f, 1.0f, 0.0f);        
  glClearDepth (1.0f);
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  glEnable (GL_COLOR_MATERIAL);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable (GL_LIGHTING);
  // glDisable(GL_LIGHTING);

  glEnable (GL_LIGHT0);
  GLfloat lightPos[4] = {1.0, 1.0, 1.0, 1.0};
  glLightfv (GL_LIGHT0, GL_POSITION, (GLfloat *) &lightPos);
  // glDisable(GL_LIGHT0);

  glEnable (GL_LIGHT1);
  GLfloat lightAmbient1[4] = {0, 0, 0, 0};
  GLfloat lightPos1[4]     = {-1.0f, 0.0f, 1.0, 1.0 };
  GLfloat lightDiffuse1[4] = {1.0f, 1.0f, 1.0f, 0.0 };
  glLightfv (GL_LIGHT1,GL_POSITION, (GLfloat *) &lightPos1);
  glLightfv (GL_LIGHT1,GL_AMBIENT, (GLfloat *) &lightAmbient1);
  glLightfv (GL_LIGHT1,GL_DIFFUSE, (GLfloat *) &lightDiffuse1);
  // glDisable (GL_LIGHT1);

  // glEnable (GL_LIGHT2);
  // GLfloat lightPos2[4]     = {1.0f, 0.5f, 1.0f, 1.0 };
  // GLfloat lightSpecular1[4] = {0.5, 0.5, 0.5, 1.0 };
  // glLightfv (GL_LIGHT2, GL_POSITION, (GLfloat *) &lightPos2);
  // glLightfv (GL_LIGHT2, GL_SPECULAR, (GLfloat *) &lightSpecular1);
  // glDisable(GL_LIGHT2);

  glDisable(GL_LIGHT3);
  glDisable(GL_LIGHT4);
  glDisable(GL_LIGHT5);
  glDisable(GL_LIGHT6);
  glDisable(GL_LIGHT7);

  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  time_t t;
  srand((unsigned) time(&t));

  // START: read and load object file
  FILE* file = fopen(file_path, "r");

  if (!file) {
    printf("File path does not exist: %s\n", file_path);
    exit(1);
  }

  char line[256];
  char firstChar[2];


  while (fgets(line, sizeof(line), file)) {

    fscanf(file, "%s", firstChar);
    if (strncmp("v", firstChar, 1) == 0) {
      if (vCount < MAX_INPUT_VERTICES) {
        fscanf(file, "%f %f %f", &vertices[vCount][0], &vertices[vCount][1], &vertices[vCount][2]);
        for (int i = 0; i < 3; i++) {
          vertices[vCount][i] *= SCALE;
        }
        vCount++;        
      } else {
        printf("Maximum number of input vertices reached: %d\n", MAX_INPUT_VERTICES);
        break;
      }

    } else if (strncmp("f", firstChar, 1) == 0) {
      if (fCount < MAX_INPUT_FACES) {
        fscanf(file, "%d %d %d", &faces[fCount][0], &faces[fCount][1], &faces[fCount][2]);
        fCount++;
      } else {
        printf("Maximum number of input faces reached: %d\n", MAX_INPUT_FACES);
        break;
      }
    }
  }

  fclose(file);
  // END


  #if DEBUG
    for (int i = 0; i < vCount; i++) {
      printf("%c %f %f %f\n", 'v', vertices[i][0], vertices[i][1], vertices[i][2]);
    }

    for (int i = 0; i < fCount - 1; i++) {
      printf("%c %d %d %d\n", 'f', faces[i][0], faces[i][1], faces[i][2]);
    }
  #endif

}


void display ()
{
  if (camera_angle_degrees >= 360.0f) {
    camera_angle_degrees = 0;
  }

  camera_angle_radians = camera_angle_degrees * M_PI / 180.0f;
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();

  gluLookAt ( sin(camera_angle_radians) * 1.5f + center_x, 
              center_y + 1.0f, 
              cos(camera_angle_radians) * 1.5f + center_z, 
              center_x, center_y, center_z, 0.0f, 1.0f, 0.0f
              ); // move camera

  // START: draw a floor to see how lighting works?
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glColor3f(1, 0.5, 0.5);
  glBegin(GL_POLYGON);

    glVertex3f(center_x + 10, 0, center_z + 10);
    glVertex3f(center_x + -10, 0, center_z + 10);
    glVertex3f(center_x + -10, 0, center_z + -10);
    glVertex3f(center_x + 10, 0, center_z + 0);
    glVertex3f(center_x + 10, 0, center_z + 10);

  glEnd();
  glPopMatrix();
  // END


  // START: logic to display points on model
  glPushMatrix ();
  glTranslatef (0, 0, 0);
  glColor3f (0.2f, 0.2f, 1.0f);
  glBegin(GL_POINTS);
  for (GLsizei i = 0; i < vCount; i++) {
    glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
  }
  glEnd();
  glPopMatrix ();
  // END

    // START: logic to display wire logic
  if (mode == 1 || mode == 3) {

    glPushMatrix ();
    glTranslatef (0, 0, 0);

    glColor3f (0.0, 0.0, 0.0);

    int v1 = 0;
    int v2 = 0;
    int v3 = 0;
    for (GLsizei i = 0; i < fCount; i++) {
      v1 = faces[i][0];
      v2 = faces[i][1];
      v3 = faces[i][2];
      glBegin(GL_LINE_LOOP);
        glVertex3f(vertices[v1][0], vertices[v1][1], vertices[v1][2]);
        glVertex3f(vertices[v2][0], vertices[v2][1], vertices[v2][2]);
        glVertex3f(vertices[v3][0], vertices[v3][1], vertices[v3][2]); 
      glEnd();
    }

    glPopMatrix ();
    // END

  }

    // START: logic to display faces on model
  if (mode == 2 || mode == 3) {
    glPushMatrix ();
    glTranslatef (0, 0, 0);

    float num1 = 1.0f;
    float num2 = 0.2f;
    float num3 = 0.6f;

    glColor3f (num1, num2, num3);
    glBegin(GL_TRIANGLES);
    int v1 = 0;
    int v2 = 0;
    int v3 = 0;
    for (GLsizei i = 0; i < fCount; i++) {
      v1 = faces[i][0];
      v2 = faces[i][1];
      v3 = faces[i][2];

      glVertex3f(vertices[v1][0], vertices[v1][1], vertices[v1][2]);
      glVertex3f(vertices[v2][0], vertices[v2][1], vertices[v2][2]);
      glVertex3f(vertices[v3][0], vertices[v3][1], vertices[v3][2]); 
    }
    glEnd();
    glPopMatrix ();
    // END
  }

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
    gluPerspective (80, (float) w / (float) h, 0.1, 5000.0); // near and far clipping planes
  }
  glMatrixMode (GL_MODELVIEW);  
  glLoadIdentity (); 
}

void keyboard (unsigned char key, int x, int y)
{

  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'w': // wire mode
      mode = 1;
      break;
    case 'a': // wire + solid mode
      mode = 3;
      break;
    case 's': // solid mode
      mode = 2;
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
      center_y += MOVE_STEP; // move camera up
      break;
    case GLUT_KEY_DOWN:
      center_y -= MOVE_STEP; // move camera down
      break;
    case GLUT_KEY_LEFT: // rotate camera left
      camera_angle_degrees += 15;
      break;
    case GLUT_KEY_RIGHT: // rotate camera right
      camera_angle_degrees -= 15;
      break;
    default:
      break;
  }

  #if DEBUG == 1
    printf("center_x: %.2f center_y: %.2f center_z: %.2f \n", center_x, center_y, center_z);
  #endif
}

int main (int argc, char *argv[]) 
{
  int window;
  char obj_path[256];

  if (argc == 2) {
    strcpy(model_file_path, argv[1]);
    printf("Path to OBJ: %s\n", model_file_path);
  }


  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize (1280, 720 ); 
  glutInitWindowPosition (0, 0);
  window = glutCreateWindow ("Bunny Model Loading Demo (genriquez)" );
  init (model_file_path);
  glutDisplayFunc (display);  
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutSpecialFunc (arrow_keys);
  glutMainLoop ();
}

