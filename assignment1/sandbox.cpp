#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>




int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (1280, 720);
   glutCreateWindow ("Self-Squaring Fractal");

   // init ( );
   glutDisplayFunc ();
   glutReshapeFunc ();

   glutMainLoop ( );
   return 0;
}
