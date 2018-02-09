#include "heap.hpp"

#include <iostream>
#include <map>
#include <cstdlib>
#include <cstring>

using namespace std;

Heap::Heap(int32_t heap_size) : heap_size(heap_size), root_set() {
  heap = new byte[heap_size];
  from = heap;
  to = heap + heap_size / 2;
  bump_ptr = 0;
}

Heap::~Heap() {
  delete[] heap;
}

// This method should print out the state of the heap.
// It's not mandatory to implement it, but would be a very useful
// debugging tool. It's called whenever the DEBUG command is found
// in the input program.
void Heap::debug_heap_print(){
  cout << endl;
  cout << "HEAP!!!!!" << endl;
  cout << "from space" << endl;
  byte *position = from;
  while(position < from+heap_size/2){
    int32_t p = local_address(position);
    object_type type = *reinterpret_cast<object_type*>(position);
    switch(type) {
      case FOO: {
        auto obj = reinterpret_cast<Foo*>(position);
        cout << p << " = FOO" << endl;
        position += sizeof(Foo);
        break;
      }
      case BAR: {
        auto obj = reinterpret_cast<Bar*>(position);
        cout << p << " = BAR" << endl;
        position += sizeof(Bar);
        break;
      }
      case BAZ: {
        auto obj = reinterpret_cast<Baz*>(position);
        cout << p << " = BAZ" << endl;
        position += sizeof(Baz);
        break;
      }
    }
  }
  cout << "------- other half ----- " << endl;
  cout << "to space" << endl;
  position = to;
  while(position < to+heap_size/2){
    int32_t p = local_address(position);
    object_type type = *reinterpret_cast<object_type*>(position);
    cout << "type " << type << endl;
   
    switch(type) {
      case FOO: {
        auto obj = reinterpret_cast<Foo*>(position);
        cout << p << " = FOO" << endl;
        position += sizeof(Foo);
        break;
      }
      case BAR: {
        auto obj = reinterpret_cast<Bar*>(position);
        cout << p << " = BAR" << endl;
        position += sizeof(Bar);
        break;
      }
      case BAZ: {
        auto obj = reinterpret_cast<Baz*>(position);
        cout << p << " = BAZ" << endl;
        position += sizeof(Baz);
        break;
      }
      // case FOL: {
      //   cout << "debugging following pointer" << endl;
      //   auto obj = reinterpret_cast<Fol*>(position);
 
      //   cout << p << " = FOL" << " " << obj->id<< endl;
      
      //   position += obj->size;
      //   break;
      // }
    }
  }
  cout << "!!!!!!!!!" << endl;
  cout << endl;
}

void Heap::debug() {
  
  cout << "\n--------DEBUGGING--------\n" << endl;
  
  cout << "PRINT" << endl;
  print();
  cout << "-----------" << endl;
  cout << "from = " << local_address(from) << endl;
  cout << "bump_ptr = " << bump_ptr << endl;
  cout << "to = " << local_address(to) << endl;
  
  //object_type type = *reinterpret_cast<object_type*>(from);
  //auto obj = reinterpret_cast<Foo*>(from);
  vector <obj_ptr> root_ptr ;
  for(auto elem : root_set){
    std::cout << elem.first << " " << elem.second << " " << "\n";
    root_ptr.push_back(elem.second);
  }
  
  debug_heap_print();
  
}

// The allocate method allocates a chunk of memory of given size.
// It returns an object pointer, which is local to the from space.
// For example, the first allocated object would have the address 0.
// IMPORTANT: This method should initiate garbage collection when there is not
// enough memory. If there is still insufficient memory after garbage collection,
// this method should throw an out_of_memory exception.
obj_ptr Heap::allocate(int32_t size) {
  // Implement me
  cout << "initial bump pointer is:   " << bump_ptr << endl;
  
  obj_ptr local_pos = bump_ptr; // save the initial bump pointer
  bump_ptr += size;             // allocate the bump pointer
  cout << "stop ====" << endl;
  if((from+bump_ptr) >= (from+heap_size/2)){ //} || bump_ptr >= heap_size){ // if there is not enough memory 'from' space
    collect();
    local_pos = bump_ptr; // new bump pointer
    bump_ptr += size; 
  }
  cout << "never ++++ " << endl;
  if((from+bump_ptr) >= (from+heap_size/2)){
      throw OutOfMemoryException();
  }

  cout << "forever ***** "<< endl;

  return local_pos;             // return the initial bump pointer before it was allocated
}

// This method should implement the actual semispace garbage collection.
// As a final result this method *MUST* call print();
void Heap::collect() {
  // Implement me
  cout << "COLLECTION IS CALLED!!!" << endl;
  
  byte *new_bump = to;
  map<obj_ptr,obj_ptr> copy;
  for(auto elem : root_set){
    cout << " variable: " << elem.first;
    cout << " position: " << elem.second;
    byte *position = from + elem.second;
    object_type type = *reinterpret_cast<object_type*>(position);
    cout << " | type: " << type << " !!! ";
    
    switch(type) {
    case FOO: {
        auto obj = reinterpret_cast<Foo*>(position);
        cout << " | FOO" << endl;
        //new (new_bump) Foo(obj->id);
        memcpy(new_bump, obj, sizeof(Foo));
        copy[local_address(obj)] = new_bump - to; 
	      // if(local_address(new_bump)>=heap_size/2)
	      //   root_set[elem.first] = local_address(new_bump) - heap_size/2;
	      // else if(local_address(new_bump)<heap_size/2)
        //   root_set[elem.first] = local_address(new_bump) + heap_size/2;
        // else
        //   root_set[elem.first] = local_address(new_bump);
	      //new (position) Fol(obj->id);
        //auto follow = reinterpret_cast<Fol*>(position);
        //follow->real_pos = *new_bump;
        //follow->size = sizeof(Baz);
        new_bump += sizeof(Foo);
        //cout << "new location is : " << local_address(new_bump) << endl;
        //cout << *reinterpret_cast<object_type*>(new_bump - sizeof(Foo)) << endl;
        break;
      }
    case BAR: {
        auto obj = reinterpret_cast<Bar*>(position);
        cout << " | BAR" << endl;
        // new (new_bump) Bar(obj->id);
        memcpy(new_bump, obj, sizeof(Bar));
        copy[local_address(obj)] = new_bump - to;
        // std::map<obj_ptr, obj_ptr>
        //if (obj->c != nil_ptr) obj->c = change[obj->c]
	      // if(local_address(new_bump)>=heap_size/2)
	      //   root_set[elem.first] = local_address(new_bump) - heap_size/2;
        // else if(local_address(new_bump)<heap_size/2)
        //   root_set[elem.first] = local_address(new_bump) + heap_size/2;
        // else
        //   root_set[elem.first] = local_address(new_bump);
        // new (position) Fol(obj->id);
        // auto follow = reinterpret_cast<Fol*>(position);
        // follow->real_pos = *new_bump;
        // follow->size = sizeof(Baz);
        new_bump += sizeof(Bar);
        // cout << "new location is : " << local_address(new_bump) << endl;
        // cout <<  *reinterpret_cast<object_type*>(new_bump - sizeof(Bar)) << endl;
        break;
      }
    case BAZ: {
        auto obj = reinterpret_cast<Baz*>(position);
        cout <<  " | BAZ" << endl;
        memcpy(new_bump, obj, sizeof(Baz));
        copy[local_address(obj)] = new_bump - to;
        // new (new_bump) Baz(obj->id);
        // if(local_address(new_bump)>=heap_size/2)
	      //   root_set[elem.first] = local_address(new_bump) - heap_size/2;
	      // else if(local_address(new_bump)<heap_size/2)
        //   root_set[elem.first] = local_address(new_bump) + heap_size/2;
        // else
        //   root_set[elem.first] = local_address(new_bump);
      	// new (position) Fol(obj->id);
        // auto follow = reinterpret_cast<Fol*>(position);
        // follow->real_pos = *new_bump;
        // follow->size = sizeof(Baz);
        new_bump += sizeof(Baz);
        // cout << "new bump ptr is : " << local_address(new_bump) << endl;
        // cout << *reinterpret_cast<object_type*>(new_bump - sizeof(Baz)) << endl;
        break;
      }
    }
  }


  for(auto fol : copy){
    byte *position = to + fol.second;
    object_type type = *reinterpret_cast<object_type*>(position);
    switch(type) {
    case FOO: {
      auto obj = reinterpret_cast<Foo*>(position);
      if(obj->c != nil_ptr) obj->c = copy[obj->c];
      if(obj->d != nil_ptr) obj->d = copy[obj->d];
      break; 
    }
    case BAR: {
      auto obj = reinterpret_cast<Bar*>(position);
      if(obj->c != nil_ptr) obj->c = copy[obj->c];
      if(obj->f != nil_ptr) obj->f = copy[obj->f];
      break;
    }
    case BAZ: {
      auto obj = reinterpret_cast<Baz*>(position);
      if(obj->c != nil_ptr) obj->c = copy[obj->c];
      if(obj->b != nil_ptr) obj->b = copy[obj->b];
      break;
    }
    }
  }

  for(auto elem : root_set){
    root_set[elem.first] = copy[elem.second];
  }

  bump_ptr = new_bump - to; 
  // if(bump_ptr >= heap_size/2)
  //   bump_ptr -= heap_size/2;
  // if(bump_ptr < 0)
  //   bump_ptr += heap_size/2;
  
  byte *temp = from; 
  from = to;
  to = temp; 
  
  cout << "from : " << local_address(from) << endl;
  cout << "bump : " << bump_ptr << endl;
  cout << "to : " << local_address(to) << endl;
  
  // Please do not remove the call to print, it has to be the final
  // operation in the method for your assignment to be graded.
  print();
}

obj_ptr Heap::get_root(const std::string& name) {
  auto root = root_set.find(name);
  if(root == root_set.end()) {
    throw std::runtime_error("No such root: " + name);
  }
  return root->second;
}

object_type Heap::get_object_type(obj_ptr ptr) {
  return *reinterpret_cast<object_type*>(from + ptr);
}

// Finds fields by path / name; used by get() and set().
obj_ptr *Heap::get_nested(const std::vector<std::string>& path) {
  obj_ptr init = get_root(path[0]);
  obj_ptr *fld = &init;
  
  for(int i = 1; i < path.size(); ++i) {
    auto addr = *fld;
    auto type = *reinterpret_cast<object_type*>(global_address<object_type>(addr));
    auto seg  = path[i];
    
    switch(type) {
    case FOO: {
      auto *foo = global_address<Foo>(addr);
      if(seg == "c") fld = &foo->c;
      else if(seg == "d") fld = &foo->d;
      else throw std::runtime_error("No such field: Foo." + seg);
      break;
      }
    case BAR: {
      auto *bar = global_address<Bar>(addr);
      if(seg == "c") fld = &bar->c;
      else if(seg == "f") fld = &bar->f;
      else throw std::runtime_error("No such field: Bar." + seg);
      break;
      }
    case BAZ: {
      auto *baz = global_address<Baz>(addr);
      if(seg == "b") fld = &baz->b;
      else if(seg == "c") fld = &baz->c;
      else throw std::runtime_error("No such field: Baz." + seg);
      break;
      }
    }
  }
  return fld;
}

obj_ptr Heap::get(const std::vector<std::string>& path) {
  if(path.size() == 1) {
    return get_root(path[0]);
  }
  else {
    return *get_nested(path);
  }
}

void Heap::set(const std::vector<std::string>& path, obj_ptr value) {
  if(path.size() == 1) {
    if(value < 0) root_set.erase(path[0]);
    else root_set[path[0]] = value;
  }
  else {
    *get_nested(path) = value;
  }
}

obj_ptr Heap::new_foo() {
  auto heap_addr = allocate(sizeof(Foo));
  new (from + heap_addr) Foo(object_id++);
  return heap_addr;
}

obj_ptr Heap::new_bar() {
  auto heap_addr = allocate(sizeof(Bar));
  new (from + heap_addr) Bar(object_id++);
  return heap_addr;
}

obj_ptr Heap::new_baz() {
  auto heap_addr = allocate(sizeof(Baz));
  new (from + heap_addr) Baz(object_id++);
  return heap_addr;
}

void Heap::print() {
  byte *position = from;
  std::map<int32_t, const char*> objects;
  while(position < (from + heap_size / 2) && position < (from + bump_ptr)) {
    object_type type = *reinterpret_cast<object_type*>(position);
    switch(type) {
    case FOO: {
        auto obj = reinterpret_cast<Foo*>(position);
        objects[obj->id] = "Foo";
        position += sizeof(Foo);
        break;
      }
    case BAR: {
        auto obj = reinterpret_cast<Bar*>(position);
        objects[obj->id] = "Bar";
        position += sizeof(Bar);
        break;
      }
    case BAZ: {
        auto obj = reinterpret_cast<Baz*>(position);
        objects[obj->id] = "Baz";
        position += sizeof(Baz);
        break;
      }
    }
  }
  
  std::cout << "Objects in from-space:\n";
  for(auto const& itr: objects) {
    std::cout << " - " << itr.first << ':' << itr.second << '\n';
  }
}
