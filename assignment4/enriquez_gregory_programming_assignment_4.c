/* Gregory Enriquez */

/*
  Compiled on Mac OSX
  Build commands:
  gcc -Wno-deprecated-declarations  -c enriquez_gregory_programming_assignment_4.c  -o enriquez_gregory_programming_assignment_4.o
  gcc -Wno-deprecated-declarations  -o enriquez_gregory_programming_assignment_4 enriquez_gregory_programming_assignment_4.o  -framework GLUT -framework OpenGL -framework Cocoa -lm

  Controls:
  Left Mouse - Open menu options
  Right Mouse - Select points:
                  Rectangle - 2 points
                  Ellipse - 2 points
                  Line - 2 points
                  Bezier Curve - 4 points
*/


#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#elif __linux__
  #include <GL/glut.h>
#endif

#define DEBUG_PRINT 1
#define X_RESOLUTION 640 
#define Y_RESOLUTION 480
#define LEFT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
#define RIGHT_MOUSE_BUTTON 3
#define MAX_SHAPES 1000

// from mouse_keyboard demo
GLsizei left_mouse_button_x = 120, left_mouse_button_y = 240;
GLsizei middle_mouse_button_x = 220, middle_mouse_button_y = 240;
GLsizei right_mouse_button_x = 320, right_mouse_button_y = 240;
int last_mouse_button_pressed = RIGHT_MOUSE_BUTTON;

// Constants
const int NUM_COLORS = 8;
const int NUM_SHAPES = 4;
const int NUM_FILLS = 2;
const char* COLORS[NUM_COLORS] = {"red", "green", "blue", "yellow", "purple", "orange", "white", "black"};
const char* FILLS[NUM_FILLS] = {"filled", "outlined"};
const char* SHAPES[NUM_SHAPES] = {"add rectangle", "add ellipse", "add line", "add bezier curve"};

// Global states
int shape_state = 0; // 0 = none/default, 1 = rectangle, 2 = ellipse, 3 = line, 4 = curve
int fill_state = 0; // 0 = fill, 1 = outline
int color_state = 0; // 0 = red, 1 = green, 2 = blue, 3 = yellow, 4 = purple, 5 = orange, 6 = white, 7 = black
int previous_state = 0;

// New structs
struct Color {
  float r, g, b;
};

struct Vertex {
  float x, y;
};

struct Shape {
  int shape; // 0 = rectangle, 1 = ellipse, 2 = line, 3 = curve
  struct Color rgb;
  int fill;                 
  struct Vertex vertices[4]; // max vertices is 4 for curve
  int num_vertices;
};

// More global variables
struct Color COLOR_VALUES[8];
struct Vertex first_point;
struct Vertex second_point;
struct Vertex third_point;
struct Vertex fourth_point;
struct Shape shapes[MAX_SHAPES];
int number_of_shapes = 0;

// struct 'Constructor' functions
void newColor(struct Color* clr, float red, float green, float blue) {
  clr->r = red;
  clr->g = green;
  clr->b = blue;
}

void newShape(struct Shape* obj, int s, float r, float g, float b, int f, struct Vertex v[], int nv) {
  obj->shape = s;
  newColor(&obj->rgb, r, g, b);
  obj->fill = f;

  for (int i = 0; i < nv; i++) {
    obj->vertices[i] = v[i];
  }
  obj->num_vertices = nv;
}

// Function Prototypes
void dumpShape(struct Shape s);
void binomialCoeffs(GLsizei, GLsizei* C);
void computeBezPt(GLfloat u, struct Vertex* bezPt, GLsizei nCtrlPts, struct Vertex* ctrlPts, GLsizei* C);
void bezier(struct Vertex* ctrlPts, GLsizei nCtrlPts, GLsizei nBezCurvePts);


// Functions
void ReSizeGLScene (int width, int height)
{
  glViewport (0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0.0, X_RESOLUTION, Y_RESOLUTION, 0, -1.0, 1.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

void menu (int value)
{
  shape_state = value / 100;
  fill_state = (value % 100) / 10;
  color_state = value % 10;

  if (previous_state != value) { // reset drawing points
    first_point.x = -1.0f;
    second_point.x = -1.0f;
    third_point.x = -1.0f;    
  }


  #if DEBUG_PRINT == 1
    printf("Menu Selection: State: %d, Shape: %s, Fill: %d, RGB: %.2f, %.2f, %.2f\n", 
          value, SHAPES[shape_state-1], fill_state, COLOR_VALUES[color_state - 1].r, 
          COLOR_VALUES[color_state - 1].g, COLOR_VALUES[color_state - 1].b);
  #endif

  previous_state = value;
  glutPostRedisplay();
}

void mouse (int mouse_button, int state, int x, int y)
{

  #if DEBUG_PRINT == 1
    printf("mouse x: %d y: %d\n", x, y);
  #endif

  if (x < 0) {
    x = 0;
  } else if (x > (X_RESOLUTION)) {
    x = X_RESOLUTION;
  } else if (y < 0) {
    y = 0;
  } else if (y > (Y_RESOLUTION)) {
    y = Y_RESOLUTION;
  }

  struct Vertex v[4];
  float r = COLOR_VALUES[color_state - 1].r;
  float g = COLOR_VALUES[color_state - 1].g;
  float b = COLOR_VALUES[color_state - 1].b;

  switch(shape_state) {
    case 1: // rectangle
      if (last_mouse_button_pressed == RIGHT_MOUSE_BUTTON && state == GLUT_DOWN) {
        if (first_point.x == -1.0f) {
          first_point.x = x;
          first_point.y = y;          
        } else {
          second_point.x = x;
          second_point.y = y;          

          v[0].x = first_point.x;
          v[0].y = first_point.y;
          v[1].x = second_point.x;
          v[1].y = first_point.y;
          v[2].x = second_point.x;
          v[2].y = second_point.y;
          v[3].x = first_point.x;
          v[3].y = second_point.y;

          newShape(&shapes[number_of_shapes], shape_state, r, g, b, fill_state, v, 4);
          dumpShape(shapes[number_of_shapes]);
          number_of_shapes++;
          first_point.x = -1.0f;
          first_point.y = -1.0f;
        }
      }
      break;
    case 2: // ellipse 
      if (last_mouse_button_pressed == RIGHT_MOUSE_BUTTON && state == GLUT_DOWN) {
        if (first_point.x == -1.0f) {
          first_point.x = x;
          first_point.y = y;          
        } else {
          second_point.x = x;
          second_point.y = y;          

          v[0].x = first_point.x;
          v[0].y = first_point.y;
          v[1].x = second_point.x;
          v[1].y = second_point.y;

          newShape(&shapes[number_of_shapes], shape_state, r, g, b, fill_state, v, 2);
          dumpShape(shapes[number_of_shapes]);
          number_of_shapes++;
          first_point.x = -1.0f;
          first_point.y = -1.0f;
        }
      }
      break;
    case 3:
      if (last_mouse_button_pressed == RIGHT_MOUSE_BUTTON && state == GLUT_DOWN) {
        if (first_point.x == -1.0f) {
          first_point.x = x;
          first_point.y = y;          
        } else {
          second_point.x = x;
          second_point.y = y;          

          v[0].x = first_point.x;
          v[0].y = first_point.y;
          v[1].x = second_point.x;
          v[1].y = second_point.y;

          newShape(&shapes[number_of_shapes], shape_state, r, g, b, fill_state, v, 2);
          dumpShape(shapes[number_of_shapes]);
          number_of_shapes++;
          first_point.x = -1.0f;
          first_point.y = -1.0f;
        }
      }
      break;
    case 4:
      if (last_mouse_button_pressed == RIGHT_MOUSE_BUTTON && state == GLUT_DOWN) {
        if (first_point.x == -1.0f) {
          first_point.x = x;
          first_point.y = y;      
        } else if (second_point.x == -1.0f) {
          second_point.x = x;
          second_point.y = y;
        } else if (third_point.x == -1.0f) {
          third_point.x = x;
          third_point.y = y;
        } else {
          fourth_point.x = x;
          fourth_point.y = y;          

          v[0].x = first_point.x;
          v[0].y = first_point.y;
          v[1].x = second_point.x;
          v[1].y = second_point.y;
          v[2].x = third_point.x;
          v[2].y = third_point.y;
          v[3].x = fourth_point.x;
          v[3].y = fourth_point.y;

          newShape(&shapes[number_of_shapes], shape_state, r, g, b, fill_state, v, 4);
          dumpShape(shapes[number_of_shapes]);
          number_of_shapes++;
          first_point.x = -1.0f;
          first_point.y = -1.0f;
          second_point.x = -1.0f;
          second_point.y = -1.0f;
          third_point.x = -1.0f;
          third_point.y = -1.0f;
        }
      }
      break;
    default:
      break;
  } 

  if ((mouse_button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
    last_mouse_button_pressed = LEFT_MOUSE_BUTTON;
  } else if ((mouse_button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN)) {
    last_mouse_button_pressed = MIDDLE_MOUSE_BUTTON;
  } else if ((mouse_button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
    last_mouse_button_pressed = RIGHT_MOUSE_BUTTON;
  }
  glutPostRedisplay ();
}

void DrawGLScene ()
{

  glClearColor (0.2, 0.2, 0.2, 1.0);
  glClear (GL_COLOR_BUFFER_BIT);
  glLoadIdentity ();

  // Loop over shapes
  for (int i = 0; i < number_of_shapes; i++) {
    struct Shape* s = &shapes[i];
    struct Vertex* v = s->vertices;
    int num_verts = s->num_vertices;

    glPushMatrix();
    glColor3f(s->rgb.r, s->rgb.g, s->rgb.b);

    switch(s->shape) {
      case 1: // draw rectangle
        if (s->fill == 1) {
          glBegin(GL_POLYGON);
        } else {
          glBegin(GL_LINE_LOOP);
        }

        for (int j = 0; j < num_verts; j++) {
          glVertex2f(v[j].x, v[j].y);
        }

        break;
      case 2: // draw ellipse
        if (s->fill == 1) {
          glBegin(GL_POLYGON);
        } else {
          glBegin(GL_LINE_LOOP);
        }

        struct Vertex start = v[0];
        struct Vertex end = v[1];

        float radX = fabs(v[1].x - v[0].x);
        float radY = fabs(v[1].y - v[0].y);

        for (int i = 0; i < 360; i++) {
          float rad = i * M_PI / 180.0f;
          glVertex2f(v[0].x + cos(rad) * radX, v[0].y + sin(rad) * radY);
        }
        break;
      case 3: // draw line
        glBegin(GL_LINES);

        for (int j = 0; j < num_verts; j++) {
          glVertex2f(v[j].x, v[j].y);
        }
        break;
      case 4: // TODO draw curve
        glBegin(GL_POINTS);

        GLsizei nCtrlPts = 4;
        GLsizei nBezCurvePts = 1000;
        struct Vertex ctrlPts[4];
        for (int i = 0; i < num_verts; i++) {
          ctrlPts[i].x = v[i].x;
          ctrlPts[i].y = v[i].y;
        }

        bezier(ctrlPts, nCtrlPts, nBezCurvePts);
        break;
      default:
        break;
    }
    glEnd();
    glPopMatrix();
  }

  glutSwapBuffers();

}

/**
  NOTE:
  The following functions are copied or are heavily influenced by the course book, pp 426-427
*/
void binomialCoeffs(GLsizei n, GLsizei* C) {
  GLsizei k, j;
  for (k = 0; k <= n; k++) {
    C[k] = 1;
    for (j = n; j >= k+1; j--) {
      C[k] *= j;
    }
    for (j = n - k; j >= 2; j--) {
      C[k] /= j;
    }
  }
}

void computeBezPt(GLfloat u, struct Vertex* bezPt, GLsizei nCtrlPts, struct Vertex* ctrlPts, GLsizei* C) {
  GLsizei k, n = nCtrlPts - 1;
  GLfloat bezBlendFcn;
  bezPt->x = bezPt->y = 0.0f;

  for (k = 0; k < nCtrlPts; k++) {
    bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
    bezPt->x += ctrlPts[k].x * bezBlendFcn;
    bezPt->y += ctrlPts[k].y * bezBlendFcn;
  }
}

void bezier(struct Vertex* ctrlPts, GLsizei nCtrlPts, GLsizei nBezCurvePts) {
  struct Vertex bezCurvePt;
  GLfloat u;
  GLsizei *C, k;

  C = (GLsizei*)malloc(sizeof(GLsizei) * nCtrlPts);
  binomialCoeffs(nCtrlPts - 1, C);
  for (k = 0; k <= nBezCurvePts; k++) {
    u = (GLfloat)k/(GLfloat)nBezCurvePts;
    computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
    glVertex2f(bezCurvePt.x, bezCurvePt.y);
  }
  free(C);
}

void dumpShape(struct Shape s) {
  #if DEBUG_PRINT == 1
    printf("Shape: %s Fill: %d, RGB: %.2f, %.2f, %.2f\n", SHAPES[s.shape-1], s.fill, s.rgb.r, s.rgb.g, s.rgb.b);
  #endif
}

void init() {
  newColor(&COLOR_VALUES[0], 1.0f, 0.0f, 0.0f);
  newColor(&COLOR_VALUES[1], 0.0f, 1.0f, 0.0f);
  newColor(&COLOR_VALUES[2], 0.0f, 0.0f, 1.0f);
  newColor(&COLOR_VALUES[3], 1.0f, 1.0f, 0.0f);
  newColor(&COLOR_VALUES[4], 0.5f, 0.0f, 1.0f);
  newColor(&COLOR_VALUES[5], 1.0f, 0.5f, 0.0f);
  newColor(&COLOR_VALUES[6], 1.0f, 1.0f, 1.0f);
  newColor(&COLOR_VALUES[7], 0.0f, 0.0f, 0.0f);

  first_point.x = -1.0f;
  first_point.y = -1.0f;

  second_point.x = -1.0f;
  second_point.y = -1.0f;

  third_point.x = -1.0f;
  third_point.y = -1.0f;

  fourth_point.x = -1.0f;
  fourth_point.y = -1.0f;
}

int main (int argc, char *argv[]) 
{  
  int window;
  int glut_sub_menu, glut_sub_sub_menu;
  int glut_colors_menu;
  int glut_shapes_menu;
  int glut_fills_menu;

  init();
  glutInit(&argc, argv);  
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (X_RESOLUTION, Y_RESOLUTION);
  glutInitWindowPosition(0, 0);  


  window = glutCreateWindow("Greg's Doodles");  

  int state_code = 0;
  glutCreateMenu(menu);
  glut_shapes_menu = glutCreateMenu(menu);

  for (int i = 0; i < NUM_SHAPES - 2; i++) { // only rectangle and ellipse have fills
    glut_fills_menu = glutCreateMenu(menu);
    for (int j = 0; j < NUM_FILLS; j++) {
      glut_colors_menu = glutCreateMenu(menu);
      for (int h = 0; h < NUM_COLORS; h++) {
        state_code = (i+1) * 100 + (j+1) * 10 + (h+1);
        glutAddMenuEntry(COLORS[h], state_code);
      }
      glutSetMenu(glut_fills_menu);
      glutAddSubMenu(FILLS[j], glut_colors_menu);
    }
    glutSetMenu(glut_shapes_menu);
    glutAddSubMenu(SHAPES[i], glut_fills_menu);
  }

  for (int i = 2; i < NUM_SHAPES; i++) {
    glut_colors_menu = glutCreateMenu(menu);
    for (int h = 0; h < NUM_COLORS; h++) {
      state_code = (i + 1) * 100 + (h+1);
      glutAddMenuEntry(COLORS[h], state_code);
    }
    glutSetMenu(glut_shapes_menu);
    glutAddSubMenu(SHAPES[i], glut_colors_menu);
  }

  glutAttachMenu(GLUT_LEFT_BUTTON);
  glutDisplayFunc (&DrawGLScene);  
  glutReshapeFunc (&ReSizeGLScene);
  glutMouseFunc (mouse);  
  glutMainLoop ();  
}

