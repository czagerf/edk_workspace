/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Ref Pointer class (referenced holder).
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */
 

#ifndef __EDK_REF_PTR_H__
#define __EDK_REF_PTR_H__


namespace EDK {

template<class T>
class ref_ptr {
 public:
  typedef T element_type;

  ref_ptr() :ptr_(0L) {}
  ref_ptr(T* t):ptr_(t) { if (ptr_) ptr_->ref(); }
  ref_ptr(const ref_ptr& rp):ptr_(rp.ptr_)  { if (ptr_) ptr_->ref(); }
  ~ref_ptr() {
    if (ptr_) ptr_->unref();
    ptr_ = 0L;
  }

  element_type* alloc() { (*this) =new T(); return ptr_; }

  template<class SubT>
  SubT* allocT() {
    SubT *i = new SubT();
    (*this) = i;
    return i;
  }

  void release() { ref_ptr empty; swap(empty); }

  ref_ptr& operator = (const ref_ptr& rp) {
    if (ptr_ == rp.ptr_) return *this;
    T* tmp_ptr = ptr_;
    ptr_ = rp.ptr_;
    if (ptr_) ptr_->ref();
    if (tmp_ptr) tmp_ptr->unref();
    return *this;
  }

  void swap(ref_ptr &other) {
    T* tmp = other.ptr_;
    other.ptr_ = ptr_;
    ptr_ = tmp;
  }

  // comparison operators for ref_ptr.
  bool operator == (const ref_ptr& rp) const { return (ptr_ == rp.ptr_); }
  bool operator != (const ref_ptr& rp) const { return (ptr_ != rp.ptr_); }
  bool operator < (const ref_ptr& rp) const { return (ptr_ < rp.ptr_); }
  bool operator > (const ref_ptr& rp) const { return (ptr_ > rp.ptr_); }

  // comparison operator for const T*.
  bool operator == (const T* ptr) const { return (ptr_ == ptr); }
  bool operator != (const T* ptr) const { return (ptr_ != ptr); }
  bool operator < (const T* ptr) const { return (ptr_ < ptr); }
  bool operator > (const T* ptr) const { return (ptr_ > ptr); }

  T& operator*() { return *ptr_; }
  const T& operator*() const { return *ptr_; }
  T* operator->() { return ptr_; }
  const T* operator->() const { return ptr_; }
  bool operator!() const { return ptr_ == 0L; }
  bool valid() const { return ptr_ != 0L; }
  T* get() { return ptr_; }
  const T* get() const { return ptr_; }

 private:
  T* ptr_;
}; //ref_ptr

template<class T>
class const_ref_ptr {
 public:
  typedef T element_type;

  const_ref_ptr() :ptr_(0L) {}
  const_ref_ptr(const T* t):ptr_(t) { if (ptr_) const_cast<T*>(ptr_)->ref(); }
  const_ref_ptr(const const_ref_ptr& rp):ptr_(rp.ptr_)  { if (ptr_) const_cast<T*>(ptr_)->ref(); }
  const_ref_ptr(const ref_ptr<T>& rp): ptr_(rp.get())  { if (ptr_) const_cast<T*>(ptr_)->ref(); }
  ~const_ref_ptr() {
    if (ptr_) const_cast<T*>(ptr_)->unref();
    ptr_ = 0L;
  }

  void release() { const_ref_ptr empty; swap(empty); }

  const_ref_ptr& operator = (const const_ref_ptr& rp) {
    if (ptr_ == rp.ptr_) return *this;
    const T* tmp_ptr = ptr_;
    ptr_ = rp.ptr_;
    if (ptr_) const_cast<T*>(ptr_)->ref();
    if (tmp_ptr) const_cast<T*>(tmp_ptr)->unref();
    return *this;
  }

  void swap(const_ref_ptr &other) {
    const T* tmp = other.ptr_;
    other.ptr_ = ptr_;
    ptr_ = tmp;
  }

  // comparison operators for const_ref_ptr.
  bool operator == (const const_ref_ptr& rp) const { return (ptr_ == rp.ptr_); }
  bool operator != (const const_ref_ptr& rp) const { return (ptr_ != rp.ptr_); }
  bool operator < (const const_ref_ptr& rp) const { return (ptr_ < rp.ptr_); }
  bool operator > (const const_ref_ptr& rp) const { return (ptr_ > rp.ptr_); }

  // comparison operator for const T*.
  bool operator == (const T* ptr) const { return (ptr_ == ptr); }
  bool operator != (const T* ptr) const { return (ptr_ != ptr); }
  bool operator < (const T* ptr) const { return (ptr_ < ptr); }
  bool operator > (const T* ptr) const { return (ptr_ > ptr); }

  const T& operator*() const { return *ptr_; }
  const T* operator->() const { return ptr_; }
  bool operator!() const { return ptr_ == 0L; }
  bool valid() const { return ptr_ != 0L; }
  const T* get() const { return ptr_; }

 private:
  const T* ptr_;
}; //const_ref_ptr

}  //EDK

#endif  //__EDK_REF_PTR_H__
