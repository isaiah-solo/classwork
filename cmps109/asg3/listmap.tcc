// $Id: listmap.tcc,v 1.5 2014-07-09 11:50:34-07 - - $

//
// Isaiah Solomon
// 1344680
// icsolomo
// Mackey
// CS 109
//
// Assignment 3
//

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
}

//
// listmap::empty()
//
template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::empty() const {
   return anchor_.next == anchor_.prev->next;
}

//
// listmap::iterator listmap::begin()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::begin() {
   return iterator (anchor_.next);
}

//
// listmap::iterator listmap::end()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::end() {
   return iterator (anchor_.prev->next);
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   if (empty())
   {
      node* new_node = new node(anchor_.next, anchor_.prev, pair);
      anchor_.next->prev = new_node;
      anchor_.next = new_node;
      return iterator(new_node);
   }
   for (node* pointer = anchor_.prev; pointer != anchor_.prev->next;
        pointer = pointer->prev)
   {
      if (pointer->value.first == pair.first)
      {
         pointer->value.second = pair.second;
         return iterator(pointer);
      }
      else if (less(pointer->value.first, pair.first))
      {
         node* new_node = new node(pointer->next, pointer, pair);
         pointer->next->prev = new_node;
         pointer->next = new_node;
         return iterator(new_node);
      }
   }
   node* new_node = new node(anchor_.next, anchor_.prev->next, pair);
   anchor_.next->prev = new_node;
   anchor_.prev->next->next = new_node;
   return iterator(new_node);
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that,
                               bool& found) const {
   for (node* pointer = anchor_.next; 
        pointer != anchor_.prev->next; pointer = pointer->next)
   {
      if (pointer->value.first == that)
         return iterator(pointer);
   }
   found = false;
   return iterator(anchor_.prev->next);
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (const key_type& that) {
   bool found = false;
   node* pointer = anchor_.next;
   for (; pointer != anchor_.prev->next; pointer = pointer->next)
   {
      if (pointer->value.first == that)
      {
         found = true;
         break;
      }
   }
   if (found == true)
   {
      pointer->prev->next = pointer->next;
      pointer->next->prev = pointer->prev;
      delete pointer;
   }
   return iterator();
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase_all () {
   while (not empty())
   {
      node* pointer = anchor_.next;
      pointer->prev->next = pointer->next;
      pointer->next->prev = pointer->prev;
      delete pointer;
   }
   return iterator();
}

//
// iterator listmap::print ()
//

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::print () {
   for (node* pointer = anchor_.next; 
        pointer != anchor_.prev->next; pointer = pointer->next)
   {
      cout << pointer->value.first << " = " 
           << pointer->value.second << endl;
   }
   return iterator();
}

//
// iterator listmap::value_print (const mapped_type&)
//

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::value_print (const mapped_type& that) {
   for (node* pointer = anchor_.next; 
        pointer != anchor_.prev->next; pointer = pointer->next)
   {
      if (pointer->value.second == that)
         cout << pointer->value << endl;
   }
   return iterator();
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

