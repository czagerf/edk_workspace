/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Referenced base class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */
 

#ifndef __EDK_REFERENCED_H__
#define __EDK_REFERENCED_H__

#include <cassert>


namespace EDK {

class Referenced {
public:
  unsigned int ref_counter() const { return ref_counter_; }
  __forceinline void ref() { ++ref_counter_; }
  __forceinline void unref() {
    assert(ref_counter_ > 0);
    --ref_counter_;
    if (ref_counter_ == 0) {
      delete this;
    }
  }

 protected:
  Referenced() : ref_counter_(0) {}
  virtual ~Referenced() {}

 private:
  unsigned int ref_counter_;
  Referenced(const Referenced &);
  Referenced& operator=(const Referenced &);
}; //Referenced

} //EDK

#endif  //__EDK_REFERENCED_H__

