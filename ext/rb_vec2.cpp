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
#include "rb_vec2.h"

using namespace std;
using namespace Imath;

VALUE class_V2d;
VALUE class_V2i;


template<typename T>
static VALUE Vec2_allocate(VALUE klass) {
    Vec2<T> * cval = new Vec2<T>(0, 0);
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Vec2<T> >, (void *)cval);
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_init(int argc, VALUE * argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(numeric val)
    // initialize(numeric x, numeric y)
    // initialize(Vec2 vect)
    // initialize(Vec2 dir, numeric len)
    // initialize(Array vect)
    // initialize(Array dir, numeric len)
    if(argc == 0)
    {
        return self;
    }
    
    VALUE a, b;
    rb_scan_args(argc, argv, "02", &a, &b);
    
    Vec2<T> * selfval = GetVec2<T>(self), vval;
    
    if(argc == 1)
    {
        if(ToVec2(a, vval))
            *selfval = vval;
        else
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec2, or Array");
    }
    else if(argc == 2)
    {
        if(IsNumType(a)) {
            selfval->x = rbpp_num_to<T>(a);
            selfval->y = rbpp_num_to<T>(b);
        }
        else if(ToVec2(a, vval)) {
            *selfval = (vval)*(rbpp_num_to<T>(b)/vval.length());
        }
        else {
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec2, or Array");
        }
    }
    else {
    	rb_raise(rb_eArgError, "Expected 1 or 2 arguments");
    }
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_add(VALUE self, VALUE rhs) {
    Vec2<T> * selfval = GetVec2<T>(self), rhsval;
    if(ToVec2(rhs, rhsval))
        return Vec2_new(*selfval + rhsval);
    else rb_raise(rb_eArgError, "Expected a Vec2, Array, or numeric type");
}

template<typename T>
static VALUE Vec2_sub(VALUE self, VALUE rhs) {
    Vec2<T> * selfval = GetVec2<T>(self), rhsval;
    if(ToVec2(rhs, rhsval))
        return Vec2_new(*selfval - rhsval);
    else rb_raise(rb_eArgError, "Expected a Vec2, Array, or numeric type");
}

template<typename T>
static VALUE Vec2_mul(VALUE self, VALUE rhs) {
    Vec2<T> * selfval = GetVec2<T>(self), rhsval;
    if(ToVec2(rhs, rhsval))
        return Vec2_new(*selfval * rhsval);
    else rb_raise(rb_eArgError, "Expected a Vec2, Array, or numeric type");
}

template<typename T>
static VALUE Vec2_div(VALUE self, VALUE rhs) {
    Vec2<T> * selfval = GetVec2<T>(self), rhsval;
    if(ToVec2(rhs, rhsval))
        return Vec2_new(*selfval / rhsval);
    else rb_raise(rb_eArgError, "Expected a Vec2, Array, or numeric type");
}

template<typename T>
static VALUE Vec2_neg(VALUE self) {
    return Vec2_new(-*GetVec2<T>(self));
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_coerce(VALUE self, VALUE lhs) {
    Vec2<T> lhsval;
    if(!ToVec2(lhs, lhsval))
    	rb_raise(rb_eArgError, "LHS must be a Vec2, Array, or numeric type");
    
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, Vec2_new(lhsval));
    rb_ary_push(arr, self);
    return arr;
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_dot(VALUE self, VALUE rhs) {
    Vec2<T> * selfval = GetVec2<T>(self), rhsval;
    if(ToVec2(rhs, rhsval))
        return rbpp_new(selfval->dot(rhsval));
    else
    	rb_raise(rb_eArgError, "Expected a Vec2, Array, or numeric type");
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_normalize(VALUE self) {
    GetVec2<T>(self)->normalize();
    return self;
}
template<typename T>
static VALUE Vec2_normalized(VALUE self) {
    return Vec2_new(GetVec2<T>(self)->normalized());
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_xeq(VALUE self, VALUE rhs) {
    GetVec2<T>(self)->x = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec2_yeq(VALUE self, VALUE rhs) {
    GetVec2<T>(self)->y = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec2_idxeq(VALUE self, VALUE idx, VALUE rhs) {
    (*GetVec2<T>(self))[rbpp_num_to<int>(idx)] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_x(VALUE self) {return rbpp_new(GetVec2<T>(self)->x);}
template<typename T>
static VALUE Vec2_y(VALUE self) {return rbpp_new(GetVec2<T>(self)->y);}

template<typename T>
static VALUE Vec2_idx(VALUE self, VALUE idx) {
    return rbpp_new((*GetVec2<T>(self))[rbpp_num_to<int>(idx)]);
}

//*******************************************************************************

template<typename T>
static VALUE Vec2_length(VALUE self) {return rbpp_new(GetVec2<T>(self)->length());}

template<typename T>
static VALUE Vec2_length2(VALUE self) {return rbpp_new(GetVec2<T>(self)->length2());}

//*******************************************************************************

template<typename T>
static VALUE Vec2_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetVec2<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Vec2_to_a(VALUE self, VALUE rhs)
{
    Vec2<T> * selfval = GetVec2<T>(self);
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, rbpp_new(selfval->x));
    rb_ary_push(arr, rbpp_new(selfval->y));
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Vec2_Class(VALUE vec2)
{
    rb_define_alloc_func(vec2, Vec2_allocate<T>);
//    rb_define_singleton_method(vec2, "allocate", (VALUE (*)(...))V3d_allocate, 0);
    DEF_MTHD(vec2, "initialize", Vec2_init<T>, -1);
    
    DEF_MTHD(vec2, "+", Vec2_add<T>, 1);
    DEF_MTHD(vec2, "-", Vec2_sub<T>, 1);
    DEF_MTHD(vec2, "*", Vec2_mul<T>, 1);
    DEF_MTHD(vec2, "/", Vec2_div<T>, 1);
    
    DEF_MTHD(vec2, "-@", Vec2_neg<T>, 0);
    
    DEF_MTHD(vec2, "coerce", Vec2_coerce<T>, 1);
    
    DEF_MTHD(vec2, "dot", Vec2_dot<T>, 1);
    
    DEF_MTHD(vec2, "normalize", Vec2_normalize<T>, 0);
    DEF_MTHD(vec2, "normalized", Vec2_normalized<T>, 0);
    
    DEF_MTHD(vec2, "x", Vec2_x<T>, 0);
    DEF_MTHD(vec2, "y", Vec2_y<T>, 0);
    DEF_MTHD(vec2, "[]", Vec2_idx<T>, 1);
    
    DEF_MTHD(vec2, "x=", Vec2_xeq<T>, 1);
    DEF_MTHD(vec2, "y=", Vec2_yeq<T>, 1);
    DEF_MTHD(vec2, "[]=", Vec2_idxeq<T>, 2);
    
    DEF_MTHD(vec2, "length", Vec2_length<T>, 0);
    DEF_MTHD(vec2, "length2", Vec2_length2<T>, 0);
    
    DEF_MTHD(vec2, "to_s", Vec2_to_s<T>, 0);
    
    DEF_MTHD(vec2, "to_a", Vec2_to_a<T>, 0);
    DEF_MTHD(vec2, "to_ary", Vec2_to_a<T>, 0);
}

void Init_Vec2()
{
    class_V2d = rb_define_class_under(module_Imath, "V2d", rb_cObject);
    Init_Vec2_Class<double>(class_V2d);
    
    class_V2i = rb_define_class_under(module_Imath, "V2i", rb_cObject);
    Init_Vec2_Class<int>(class_V2i);
}

//*******************************************************************************

//template <class Vec> Vec        project (const Vec &s, const Vec &t);
//template <class Vec> Vec        orthogonal (const Vec &s, const Vec &t);
//template <class Vec> Vec        reflect (const Vec &s, const Vec &t);
//template <class Vec> Vec        closestVertex (const Vec &v0, const Vec &v1, const Vec &v2, const Vec &p);
