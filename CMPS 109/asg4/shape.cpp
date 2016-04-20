// $Id: shape.cpp,v 1.7 2014-05-08 18:32:56-07 - - $

#include <cmath>
#include <typeinfo>
#include <unordered_map>
using namespace std;

#include "graphics.h"
#include "shape.h"
#include "util.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
}

text::text (string glut_bitmap_font, const string& textdata):
   glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
}

ellipse::ellipse (GLfloat width, GLfloat height):
   dimension ({width, height}) {
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
}

rectangle::rectangle (GLfloat width, GLfloat height):
   polygon({{-width/2, -height/2}, {width/2, -height/2},
   {width/2, height/2}, {-width/2, height/2}}) {
}

square::square (GLfloat width): rectangle (width, width) {
}

diamond::diamond (GLfloat width, GLfloat height):
   polygon({{0, height/2}, {width/2, 0},
   {0, -height/2}, {-width/2, 0}}) {
}

triangle::triangle (const vertex_list& vertices):
   polygon(vertices) {
}

right_triangle::right_triangle (GLfloat width, GLfloat height):
   polygon({{-width/2, -height/2}, {-width/2, height/2},
   {width/2, -height/2}}) {
}

isosceles::isosceles (GLfloat width, GLfloat height):
   polygon({{0, height/2}, {width/2, -height/2},
   {-width/2, -height/2}}) {
}

equilateral::equilateral (GLfloat width):
   isosceles(width, sqrt(3) * width/2) {
}

void text::draw (const vertex& center, const rgbcolor& color,
                 const int& num) const {
   glColor3ub (color.red, color.green, color.blue);
   glRasterPos2f (center.xpos, center.ypos);
   auto itor = fontcode.find(glut_bitmap_font);
   for (auto ch: textdata) glutBitmapCharacter (itor->second, ch);
   int num_ = num;
   num_ = num_;
}

void ellipse::draw (const vertex& center, const rgbcolor& color,
                    const int& num) const {
   if (window::get_objects()[num].selected == true)
   {
      glBegin(GL_POLYGON);
      glColor3ub (window::border_color.red, window::border_color.green,
                  window::border_color.blue);
      const float delta = 2 * M_PI / 32;
      for (float theta = 0; theta < 2 * M_PI; theta += delta) {
         float new_xpos = (dimension.xpos + window::border_thickness)
                           * cos (theta) + center.xpos;
         float new_ypos = (dimension.ypos + window::border_thickness)
                           * sin (theta) + center.ypos;
         glVertex2f (new_xpos, new_ypos);
      }
      glEnd();
   }
   glBegin(GL_POLYGON);
   glColor3ub (color.red, color.green, color.blue);
   const float delta = 2 * M_PI / 32;
   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float new_xpos = dimension.xpos * cos (theta) + center.xpos;
      float new_ypos = dimension.ypos * sin (theta) + center.ypos;
      glVertex2f (new_xpos, new_ypos);
   }
   glEnd();
}

void polygon::draw (const vertex& center, const rgbcolor& color,
                    const int& num) const {
   auto true_center = make_shared<vertex> ();
   true_center->xpos = 0;
   true_center->ypos = 0;
   float lowest_x = 640, lowest_y = 433;
   if (window::get_objects()[num].selected == true)
   {
      for (auto itor = vertices.begin();
           itor != vertices.end(); ++itor)
      {
         if (lowest_x > itor->xpos) lowest_x = itor->xpos;
         if (lowest_y > itor->ypos) lowest_y = itor->ypos;
      }
      for (auto itor = vertices.begin();
           itor != vertices.end(); ++itor)
      {
         true_center->xpos += ((itor->xpos-lowest_x)/vertices.size());
         true_center->ypos += ((itor->ypos-lowest_y)/vertices.size());
      }
      glBegin(GL_POLYGON);
      glColor3ub (window::border_color.red, window::border_color.green,
                  window::border_color.blue);
      for (auto itor = vertices.begin();
           itor != vertices.end(); ++itor)
      {
         float border_x = 0, border_y = 0;
         if (itor->xpos == 0 and itor->ypos == 0)
         {
            border_y = -sqrt(pow(window::border_thickness, 2)
                            - pow(border_y, 2));
            border_x = -sqrt(pow(window::border_thickness, 2)
                            - pow(border_y, 2));
         }
         else
         {
            border_y = ((window::border_thickness * itor->ypos)
                       / sqrt(pow(itor->xpos, 2) + pow(itor->ypos, 2)));
            border_x = sqrt(pow(window::border_thickness, 2)
                            - pow(border_y, 2));
         }
         if (itor->xpos < 0) border_x = -border_x;
         glVertex2f (border_x + itor->xpos + center.xpos
                     - lowest_x - true_center->xpos,
                     border_y + itor->ypos + center.ypos
                     - lowest_y - true_center->ypos);
      }
      glEnd();
   }
   true_center->xpos = 0;
   true_center->ypos = 0;
   lowest_x = 640; lowest_y = 433;
   for (auto itor = vertices.begin(); itor != vertices.end(); ++itor)
   {
      if (lowest_x > itor->xpos) lowest_x = itor->xpos;
      if (lowest_y > itor->ypos) lowest_y = itor->ypos;
   }
   for (auto itor = vertices.begin(); itor != vertices.end(); ++itor)
   {
      true_center->xpos += ((itor->xpos - lowest_x)/vertices.size());
      true_center->ypos += ((itor->ypos - lowest_y)/vertices.size());
   }
   glBegin (GL_POLYGON);
   glColor3ub (color.red, color.green, color.blue);
   for (auto itor = vertices.begin(); itor != vertices.end(); ++itor)
   {
   glVertex2f (itor->xpos + center.xpos - lowest_x - true_center->xpos,
               itor->ypos + center.ypos - lowest_y - true_center->ypos);
   }
   glEnd();
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << glut_bitmap_font
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

