//*******************************************************************************
//    Copyright (c) 2011, Christopher James Huff
//    All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//*******************************************************************************

#ifndef RUBYHELPERS_H
#define RUBYHELPERS_H

static inline bool IsType(VALUE val, int type) {return TYPE(val) == type;}

static inline bool IsNumType(VALUE val) {
    return TYPE(val) == T_FLOAT || TYPE(val) == T_FIXNUM || TYPE(val) == T_BIGNUM;
}


// Generic free function for a C++ object, simply casts and calls delete
template<typename T>
static inline void CPP_DeleteFree(void * st) {delete static_cast<T *>(st);}


template<typename T> T rbpp_num_to(VALUE);
template<> static inline double rbpp_num_to<double>(VALUE val) {return NUM2DBL(val);}
template<> static inline int rbpp_num_to<int>(VALUE val) {return NUM2INT(val);}
template<> static inline unsigned int rbpp_num_to<unsigned int>(VALUE val) {return NUM2UINT(val);}

static inline VALUE rbpp_new(double val) {return rb_float_new(val);}
static inline VALUE rbpp_new(int val) {return rb_int_new(val);}
static inline VALUE rbpp_new(unsigned int val) {return rb_uint_new(val);}



static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(ANYARGS), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE,VALUE), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE,VALUE,VALUE), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE,VALUE,VALUE,VALUE), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_MTHD(VALUE obj, const char * name, VALUE(*fn)(int, VALUE*, VALUE), int n) {
    rb_define_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}



static inline void DEF_SING_MTHD(VALUE obj, const char * name, VALUE(*fn)(ANYARGS), int n) {
    rb_define_singleton_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_SING_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE), int n) {
    rb_define_singleton_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_SING_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE,VALUE), int n) {
    rb_define_singleton_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_SING_MTHD(VALUE obj, const char * name, VALUE(*fn)(VALUE,VALUE,VALUE), int n) {
    rb_define_singleton_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}
static inline void DEF_SING_MTHD(VALUE obj, const char * name, VALUE(*fn)(int, VALUE*, VALUE), int n) {
    rb_define_singleton_method(obj, name, RUBY_METHOD_FUNC(fn), n);
}


#endif RUBYHELPERS_H
