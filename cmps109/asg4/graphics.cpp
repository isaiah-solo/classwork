// $Id: graphics.cpp,v 1.11 2014-05-15 16:42:55-07 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
int window::pixels = 4;
int object::object_num = 0;
rgbcolor window::border_color {255, 0, 0};
int window::border_thickness {4};
vector<object> window::objects;
size_t window::selected_obj = 0;
mouse window::mus;

// Executed when window system signals to shut down.
void window::close() {
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
              << ", height=" << window::height);
   }
   glutPostRedisplay();
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   for (auto& object: window::objects) object.draw();
   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   window::width = width;
   window::height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}


// Executed when a regular keyboard key is pressed.
enum {BS=8, TAB=9, ESC=27, SPACE=32, DEL=127};
void window::keyboard (GLubyte key, int x, int y) {
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:
         window::close();
         break;
      case 'H': case 'h':
         window::move_selected_object (-1, 0);
         break;
      case 'J': case 'j':
         window::move_selected_object (0, -1);
         break;
      case 'K': case 'k':
         window::move_selected_object (0, 1);
         break;
      case 'L': case 'l':
         window::move_selected_object (1, 0);
         break;
      case 'N': case 'n': case SPACE: case TAB:
         if (window::get_selected_object() + 1 <
             window::objects.size())
            window::select_object(window::get_selected_object() + 1);
            break;
      case 'P': case 'p': case BS:
         if (window::get_selected_object() > 0)
         window::select_object(window::get_selected_object() - 1);
         break;
      case '0'...'9':
         if (static_cast<unsigned int>(key - '0') <
            window::objects.size())
            window::select_object (key - '0');
         break;
      default:
         cerr << (unsigned)key << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}


// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: window::move_selected_object (-1, 0); break;
      case GLUT_KEY_DOWN: window::move_selected_object (0, -1); break;
      case GLUT_KEY_UP: window::move_selected_object (0, 1); break;
      case GLUT_KEY_RIGHT: window::move_selected_object (1, 0); break;
      case GLUT_KEY_F1: if (1 < window::objects.size())
                        window::select_object (1); break;
      case GLUT_KEY_F2: if (2 < window::objects.size())
                        window::select_object (2); break;
      case GLUT_KEY_F3: if (3 < window::objects.size())
                        window::select_object (3); break;
      case GLUT_KEY_F4: if (4 < window::objects.size())
                        window::select_object (4); break;
      case GLUT_KEY_F5: if (5 < window::objects.size())
                        window::select_object (5); break;
      case GLUT_KEY_F6: if (6 < window::objects.size())
                        window::select_object (6); break;
      case GLUT_KEY_F7: if (7 < window::objects.size())
                        window::select_object (7); break;
      case GLUT_KEY_F8: if (8 < window::objects.size())
                        window::select_object (8); break;
      case GLUT_KEY_F9: if (9 < window::objects.size())
                        window::select_object (9); break;
      case GLUT_KEY_F10: if (10 < window::objects.size())
                         window::select_object (10); break;
      case GLUT_KEY_F11: if (11 < window::objects.size())
                         window::select_object (11); break;
      case GLUT_KEY_F12: if (12 < window::objects.size())
                         window::select_object (12); break;
      default:
         cerr << (unsigned)key << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}


void window::motion (int x, int y) {
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   glutMainLoop();
}


void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      glutBitmapString (font, (GLubyte*) text.str().c_str());
   }
}

void window::move_selected_object (int x, int y) {
   window::objects[window::selected_obj].move(x * window::pixels,
                   y * window::pixels);
}

void window::select_object (size_t selected_object) {
   window::objects[window::selected_obj].selected = false;
   window::selected_obj = selected_object;
   window::objects[window::selected_obj].selected = true;
}
