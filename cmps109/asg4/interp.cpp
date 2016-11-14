// $Id: interp.cpp,v 1.18 2015-02-19 16:50:37-08 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_moveby },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"            , &interpreter::make_text            },
   {"ellipse"         , &interpreter::make_ellipse         },
   {"circle"          , &interpreter::make_circle          },
   {"polygon"         , &interpreter::make_polygon         },
   {"rectangle"       , &interpreter::make_rectangle       },
   {"square"          , &interpreter::make_square          },
   {"diamond"         , &interpreter::make_diamond         },
   {"triangle"        , &interpreter::make_triangle        },
   {"right_triangle"  , &interpreter::make_right_triangle  },
   {"isosceles"       , &interpreter::make_isosceles       },
   {"equilateral"     , &interpreter::make_equilateral     },
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   if (params.size() < 2) throw runtime_error ("syntax error");
   param begin = params.cbegin();
   string command = *begin;
   if (command == "define" and params.size() < 4)
      throw runtime_error ("syntax error");
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   string name {};
   if (begin != end) name = *begin;
   int counter = 0;
   for (auto itor = begin; itor != end; ++itor)
      counter += 1;
   if ((begin[1].compare("polygon") == 0 and counter > 2
        and counter % 2 == 1) or
       (begin[1].compare("triangle") == 0 and counter - 8 != 0)or
       (begin[1].compare("square") == 0 and counter != 3)or
       (begin[1].compare("rectangle") == 0 and counter != 4)or
       (begin[1].compare("diamond") == 0 and counter != 4)or
       (begin[1].compare("right_triangle") == 0 and counter != 4)or
       (begin[1].compare("isosceles") == 0 and counter != 4)or
       (begin[1].compare("equilateral") == 0 and counter != 3)or
       (counter < 3))
      throw runtime_error ("syntax error");
      objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   rgbcolor color {begin[0]};
   auto new_object = make_shared<object> ();
   new_object->set_object(itor->second, where, color);
   window::push_back(*new_object);
}

void interpreter::do_border (param begin, param end) {
   rgbcolor new_color {begin[0]};
   window::border_color = new_color;
   window::border_thickness = stof(begin[1]);
   end = end;
}

void interpreter::do_moveby (param begin, param end) {
   window::pixels = stoi(begin[0]);
   end = end;
}

shape_ptr interpreter::make_shape (param begin, param end) {
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) throw runtime_error 
      (type + ": no such shape");
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   string text_data {};
   auto start = begin;
   ++start;
   text_data = *start;
   ++start;
   for (; start != end; ++start) text_data += " " + *start;
   return make_shared<text> (begin[0], text_data);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   end = end;
   return make_shared<ellipse> (stof(begin[0]), stof(begin[1]));
}

shape_ptr interpreter::make_circle (param begin, param end) {
   end = end;
   return make_shared<circle> (stof(begin[0]));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   auto new_list = make_shared<vertex_list> ();
   for (auto itor = begin; itor != end; ++itor)
   {
      auto new_vertex = make_shared<vertex> ();
      new_vertex->xpos = stof(*itor);
      new_vertex->ypos = stof(*(++itor));
      new_list->push_back(*new_vertex);
   }
   return make_shared<polygon> (*new_list);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   end = end;
   return make_shared<rectangle> (stof(begin[0]), stof(begin[1]));
}

shape_ptr interpreter::make_square (param begin, param end) {
   end = end;
   return make_shared<square> (stof(begin[0]));
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   end = end;
   return make_shared<diamond> (stof(begin[0]), stof(begin[1]));
}

shape_ptr interpreter::make_triangle (param begin, param end) {
   auto new_list = make_shared<vertex_list> ();
   for (auto itor = begin; itor != end; ++itor)
   {
      auto new_vertex = make_shared<vertex> ();
      new_vertex->xpos = stof(*itor);
      new_vertex->ypos = stof(*(++itor));
      new_list->push_back(*new_vertex);
   }
   return make_shared<triangle> (*new_list);
}

shape_ptr interpreter::make_right_triangle (param begin, param end) {
   end = end;
   return make_shared<right_triangle> (stof(begin[0]), stof(begin[1]));
}

shape_ptr interpreter::make_isosceles (param begin, param end) {
   end = end;
   return make_shared<isosceles> (stof(begin[0]), stof(begin[1]));
}

shape_ptr interpreter::make_equilateral (param begin, param end) {
   end = end;
   return make_shared<equilateral> (stof(begin[0]));
}

