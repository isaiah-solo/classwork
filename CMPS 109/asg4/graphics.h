// $Id: graphics.h,v 1.9 2014-05-15 16:42:55-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
   public:
      // Default copiers, movers, dtor all OK.
      void draw() { pshape->draw (center, color, this_object_num); }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
      bool selected = false;
      void set_object (const shared_ptr<shape>& new_pshape,
      const vertex& new_center, const rgbcolor& new_color){
         pshape = new_pshape;
         center = new_center;
         color = new_color;
         this_object_num = object_num++;
      };
      static int object_num;
      int this_object_num {};
};

class mouse {
   friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};


class window {
   friend class mouse;
   private:
      static int width;         // in pixels
      static int height;        // in pixels
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static void push_back (const object& obj) {
                             objects.push_back (obj); }
      static int getwidth () { return width; }
      static int getheight () { return height; }
      static int pixels;
      static rgbcolor border_color;
      static int border_thickness;
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void move_selected_object (int x, int y);
      static size_t get_selected_object () { return selected_obj; };
      static void select_object (size_t selected_object);
      static vector<object> get_objects () { return objects; };
      static void main();
};

#endif

