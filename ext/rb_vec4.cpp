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

#include <cstdlib>
#include <stdint.h>

#include <algorithm>
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>

#include "rb_openexr.h"
#include "rb_vec4.h"

using namespace std;
using namespace Imath;

VALUE class_V4d;
VALUE class_V4i;

//rb_gc_mark(VALUE...)

// No references, do nothing
// Might make native Ruby value vector binding...
//void * V4dMark(void * st) {}

template<typename T>
static VALUE Vec4_allocate(VALUE klass) {
    Vec4<T> * cval = new Vec4<T>(0, 0, 0, 0);
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Vec4<T> >, (void *)cval);
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_init(int argc, VALUE * argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(numeric val)
    // initialize(numeric x, numeric y, numeric z)
    // initialize(V4d vect)
    // initialize(V4d dir, numeric len)
    // initialize(Array vect)
    // initialize(Array dir, numeric len)
    if(argc == 0)
    {
        return self;
    }
    
    VALUE a, b, c, d;
    rb_scan_args(argc, argv, "04", &a, &b, &c, &d);
    
    Vec4<T> * selfval = GetVec4<T>(self), vval;
    if(argc == 1)
    {
        if(ToVec4(a, vval))
            *selfval = vval;
        else
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec4, or Array");
    }
    else if(argc == 2)
    {
        if(IsNumType(a)) {
            selfval->x = rbpp_num_to<T>(a);
            selfval->y = rbpp_num_to<T>(b);
        }
        else if(ToVec4(a, vval)) {
            *selfval = (vval)*(rbpp_num_to<T>(b)/vval.length());
        }
        else {
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec4, or Array");
        }
    }
    else if(argc == 3)
    {
        selfval->x = rbpp_num_to<T>(a);
        selfval->y = rbpp_num_to<T>(b);
        selfval->z = rbpp_num_to<T>(c);
        selfval->w = 0;
    }
    else if(argc == 4)
    {
        selfval->x = rbpp_num_to<T>(a);
        selfval->y = rbpp_num_to<T>(b);
        selfval->z = rbpp_num_to<T>(c);
        selfval->w = rbpp_num_to<T>(d);
    }
    else {
    	rb_raise(rb_eArgError, "Expected 1, 2, 3, or 4 arguments");
    }
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_add(VALUE self, VALUE rhs) {
    Vec4<T> * selfval = GetVec4<T>(self), rhsval;
    if(ToVec4(rhs, rhsval))
        return Vec4_new(*selfval + rhsval);
    else rb_raise(rb_eArgError, "Expected a V4d, Array, or numeric type");
}

template<typename T>
static VALUE Vec4_sub(VALUE self, VALUE rhs) {
    Vec4<T> * selfval = GetVec4<T>(self), rhsval;
    if(ToVec4(rhs, rhsval))
        return Vec4_new(*selfval - rhsval);
    else rb_raise(rb_eArgError, "Expected a V4d, Array, or numeric type");
}

template<typename T>
static VALUE Vec4_mul(VALUE self, VALUE rhs) {
    Vec4<T> * selfval = GetVec4<T>(self), rhsval;
    if(ToVec4(rhs, rhsval))
        return Vec4_new(*selfval * rhsval);
    else rb_raise(rb_eArgError, "Expected a V4d, Array, or numeric type");
}

template<typename T>
static VALUE Vec4_div(VALUE self, VALUE rhs) {
    Vec4<T> * selfval = GetVec4<T>(self), rhsval;
    if(ToVec4(rhs, rhsval))
        return Vec4_new(*selfval / rhsval);
    else rb_raise(rb_eArgError, "Expected a V4d, Array, or numeric type");
}

template<typename T>
static VALUE Vec4_neg(VALUE self) {
    return Vec4_new(-*GetVec4<T>(self));
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_coerce(VALUE self, VALUE lhs) {
    Vec4<T> lhsval;
    if(!ToVec4(lhs, lhsval))
    	rb_raise(rb_eArgError, "LHS must be a Vec4, Array, or numeric type");
    
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, Vec4_new(lhsval));
    rb_ary_push(arr, self);
    return arr;
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_dot(VALUE self, VALUE rhs) {
    Vec4<T> * selfval = GetVec4<T>(self), rhsval;
    if(ToVec4(rhs, rhsval))
        return rbpp_new(selfval->dot(rhsval));
    else
    	rb_raise(rb_eArgError, "Expected a Vec4, Array, or numeric type");
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_normalize(VALUE self) {
    GetVec4<T>(self)->normalize();
    return self;
}
template<typename T>
static VALUE Vec4_normalized(VALUE self) {
    return Vec4_new(GetVec4<T>(self)->normalized());
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_xeq(VALUE self, VALUE rhs) {
    GetVec4<T>(self)->x = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec4_yeq(VALUE self, VALUE rhs) {
    GetVec4<T>(self)->y = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec4_zeq(VALUE self, VALUE rhs) {
    GetVec4<T>(self)->z = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec4_weq(VALUE self, VALUE rhs) {
    GetVec4<T>(self)->w = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec4_idxeq(VALUE self, VALUE idx, VALUE rhs) {
    (*GetVec4<T>(self))[rbpp_num_to<int>(idx)] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_x(VALUE self) {return rbpp_new(GetVec4<T>(self)->x);}
template<typename T>
static VALUE Vec4_y(VALUE self) {return rbpp_new(GetVec4<T>(self)->y);}
template<typename T>
static VALUE Vec4_z(VALUE self) {return rbpp_new(GetVec4<T>(self)->z);}
template<typename T>
static VALUE Vec4_w(VALUE self) {return rbpp_new(GetVec4<T>(self)->w);}

template<typename T>
static VALUE Vec4_idx(VALUE self, VALUE idx) {
    return rbpp_new((*GetVec4<T>(self))[rbpp_num_to<int>(idx)]);
}

//*******************************************************************************

template<typename T>
static VALUE Vec4_length(VALUE self) {return rbpp_new(GetVec4<T>(self)->length());}

template<typename T>
static VALUE Vec4_length2(VALUE self) {return rbpp_new(GetVec4<T>(self)->length2());}

//*******************************************************************************

template<typename T>
static VALUE Vec4_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetVec4<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Vec4_to_a(VALUE self, VALUE rhs)
{
    Vec4<T> * selfval = GetVec4<T>(self);
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, rbpp_new(selfval->x));
    rb_ary_push(arr, rbpp_new(selfval->y));
    rb_ary_push(arr, rbpp_new(selfval->z));
    rb_ary_push(arr, rbpp_new(selfval->w));
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Vec4_Class(VALUE vec4)
{
    rb_define_alloc_func(vec4, Vec4_allocate<T>);
//    rb_define_singleton_method(vec4, "allocate", (VALUE (*)(...))V4d_allocate, 0);
    DEF_MTHD(vec4, "initialize", Vec4_init<T>, -1);
    
    DEF_MTHD(vec4, "+", Vec4_add<T>, 1);
    DEF_MTHD(vec4, "-", Vec4_sub<T>, 1);
    DEF_MTHD(vec4, "*", Vec4_mul<T>, 1);
    DEF_MTHD(vec4, "/", Vec4_div<T>, 1);
    
    DEF_MTHD(vec4, "-@", Vec4_neg<T>, 0);
    
    DEF_MTHD(vec4, "coerce", Vec4_coerce<T>, 1);
    
    DEF_MTHD(vec4, "dot", Vec4_dot<T>, 1);
    
    DEF_MTHD(vec4, "normalize", Vec4_normalize<T>, 0);
    DEF_MTHD(vec4, "normalized", Vec4_normalized<T>, 0);
    
    DEF_MTHD(vec4, "x", Vec4_x<T>, 0);
    DEF_MTHD(vec4, "y", Vec4_y<T>, 0);
    DEF_MTHD(vec4, "z", Vec4_z<T>, 0);
    DEF_MTHD(vec4, "w", Vec4_w<T>, 0);
    DEF_MTHD(vec4, "[]", Vec4_idx<T>, 1);
    
    DEF_MTHD(vec4, "x=", Vec4_xeq<T>, 1);
    DEF_MTHD(vec4, "y=", Vec4_yeq<T>, 1);
    DEF_MTHD(vec4, "z=", Vec4_zeq<T>, 1);
    DEF_MTHD(vec4, "w=", Vec4_weq<T>, 1);
    DEF_MTHD(vec4, "[]=", Vec4_idxeq<T>, 2);
    
    DEF_MTHD(vec4, "length", Vec4_length<T>, 0);
    DEF_MTHD(vec4, "length2", Vec4_length2<T>, 0);
    
    DEF_MTHD(vec4, "to_s", Vec4_to_s<T>, 0);
    
    DEF_MTHD(vec4, "to_a", Vec4_to_a<T>, 0);
    DEF_MTHD(vec4, "to_ary", Vec4_to_a<T>, 0);
}

void Init_Vec4()
{
    class_V4d = rb_define_class_under(module_Imath, "V4d", rb_cObject);
    Init_Vec4_Class<double>(class_V4d);
    
    class_V4i = rb_define_class_under(module_Imath, "V4i", rb_cObject);
    Init_Vec4_Class<int>(class_V4i);
}


//*******************************************************************************

//template <class Vec> Vec        project (const Vec &s, const Vec &t);
//template <class Vec> Vec        orthogonal (const Vec &s, const Vec &t);
//template <class Vec> Vec        reflect (const Vec &s, const Vec &t);
//template <class Vec> Vec        closestVertex (const Vec &v0, const Vec &v1, const Vec &v2, const Vec &p);
