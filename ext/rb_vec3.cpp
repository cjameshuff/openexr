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
#include "rb_vec3.h"

using namespace std;
using namespace Imath;

VALUE class_V3d;
VALUE class_V3i;

//rb_gc_mark(VALUE...)

// No references, do nothing
// Might make native Ruby value vector binding...
//void * V3dMark(void * st) {}

template<typename T>
static VALUE Vec3_allocate(VALUE klass) {
    Vec3<T> * cval = new Vec3<T>(0, 0, 0);
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Vec3<T> >, (void *)cval);
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_init(int argc, VALUE *argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(numeric val)
    // initialize(numeric x, numeric y, numeric z)
    // initialize(V3d vect)
    // initialize(V3d dir, numeric len)
    // initialize(Array vect)
    // initialize(Array dir, numeric len)
    if(argc == 0)
    {
        return self;
    }
    
    VALUE a, b, c;
    rb_scan_args(argc, argv, "03", &a, &b, &c);
    
    Vec3<T> * selfval = GetVec3<T>(self), vval;
    
    if(argc == 1)
    {
        if(ToVec3(a, vval))
            *selfval = vval;
        else
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec3, or Array");
    }
    else if(argc == 2)
    {
        if(IsNumType(a)) {
            selfval->x = rbpp_num_to<T>(a);
            selfval->y = rbpp_num_to<T>(b);
        }
        else if(ToVec3(a, vval)) {
            *selfval = (vval)*(rbpp_num_to<T>(b)/vval.length());
        }
        else {
        	rb_raise(rb_eArgError, "Expected a numeric type, Vec3, or Array");
        }
    }
    else if(argc == 3)
    {
        selfval->x = rbpp_num_to<T>(a);
        selfval->y = rbpp_num_to<T>(b);
        selfval->z = rbpp_num_to<T>(c);
    }
    else {
    	rb_raise(rb_eArgError, "Expected 1, 2, or 3 arguments");
    }
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_add(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return Vec3_new(*selfval + rhsval);
    else rb_raise(rb_eArgError, "Expected a V3d, Array, or numeric type");
}

template<typename T>
static VALUE Vec3_sub(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return Vec3_new(*selfval - rhsval);
    else rb_raise(rb_eArgError, "Expected a V3d, Array, or numeric type");
}

template<typename T>
static VALUE Vec3_mul(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return Vec3_new(*selfval * rhsval);
    else rb_raise(rb_eArgError, "Expected a V3d, Array, or numeric type");
}

template<typename T>
static VALUE Vec3_div(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return Vec3_new(*selfval / rhsval);
    else rb_raise(rb_eArgError, "Expected a V3d, Array, or numeric type");
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_coerce(VALUE self, VALUE lhs) {
    Vec3<T> lhsval;
    if(!ToVec3(lhs, lhsval))
    	rb_raise(rb_eArgError, "LHS must be a Vec3, Array, or numeric type");
    
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, Vec3_new(lhsval));
    rb_ary_push(arr, self);
    return arr;
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_cross(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return Vec3_new(selfval->cross(rhsval));
    else
    	rb_raise(rb_eArgError, "Expected a Vec3, Array, or numeric type");
}

template<typename T>
static VALUE Vec3_dot(VALUE self, VALUE rhs) {
    Vec3<T> * selfval = GetVec3<T>(self), rhsval;
    if(ToVec3(rhs, rhsval))
        return rbpp_new(selfval->dot(rhsval));
    else
    	rb_raise(rb_eArgError, "Expected a Vec3, Array, or numeric type");
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_normalize(VALUE self) {
    GetVec3<T>(self)->normalize();
    return self;
}
template<typename T>
static VALUE Vec3_normalized(VALUE self) {
    return Vec3_new(GetVec3<T>(self)->normalized());
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_xeq(VALUE self, VALUE rhs) {
    GetVec3<T>(self)->x = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec3_yeq(VALUE self, VALUE rhs) {
    GetVec3<T>(self)->y = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec3_zeq(VALUE self, VALUE rhs) {
    GetVec3<T>(self)->z = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Vec3_idxeq(VALUE self, VALUE idx, VALUE rhs) {
    (*GetVec3<T>(self))[rbpp_num_to<int>(idx)] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_x(VALUE self) {return rbpp_new(GetVec3<T>(self)->x);}
template<typename T>
static VALUE Vec3_y(VALUE self) {return rbpp_new(GetVec3<T>(self)->y);}
template<typename T>
static VALUE Vec3_z(VALUE self) {return rbpp_new(GetVec3<T>(self)->z);}

template<typename T>
static VALUE Vec3_idx(VALUE self, VALUE idx) {
    return rbpp_new((*GetVec3<T>(self))[rbpp_num_to<int>(idx)]);
}

//*******************************************************************************

template<typename T>
static VALUE Vec3_length(VALUE self) {return rbpp_new(GetVec3<T>(self)->length());}

template<typename T>
static VALUE Vec3_length2(VALUE self) {return rbpp_new(GetVec3<T>(self)->length2());}

//*******************************************************************************

template<typename T>
static VALUE Vec3_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetVec3<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Vec3_to_a(VALUE self, VALUE rhs)
{
    Vec3<T> * selfval = GetVec3<T>(self);
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, rbpp_new(selfval->x));
    rb_ary_push(arr, rbpp_new(selfval->y));
    rb_ary_push(arr, rbpp_new(selfval->z));
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Vec3_Class(VALUE vec3)
{
    rb_define_alloc_func(vec3, Vec3_allocate<T>);
//    rb_define_singleton_method(vec3, "allocate", (VALUE (*)(...))V3d_allocate, 0);
    DEF_MTHD(vec3, "initialize", Vec3_init<T>, -1);
    
    DEF_MTHD(vec3, "+", Vec3_add<T>, 1);
    DEF_MTHD(vec3, "-", Vec3_sub<T>, 1);
    DEF_MTHD(vec3, "*", Vec3_mul<T>, 1);
    DEF_MTHD(vec3, "/", Vec3_div<T>, 1);
    
    DEF_MTHD(vec3, "coerce", Vec3_coerce<T>, 1);
    
    DEF_MTHD(vec3, "cross", Vec3_cross<T>, 1);
    DEF_MTHD(vec3, "dot", Vec3_dot<T>, 1);
    
    DEF_MTHD(vec3, "normalize", Vec3_normalize<T>, 0);
    DEF_MTHD(vec3, "normalized", Vec3_normalized<T>, 0);
    
    DEF_MTHD(vec3, "x", Vec3_x<T>, 0);
    DEF_MTHD(vec3, "y", Vec3_y<T>, 0);
    DEF_MTHD(vec3, "z", Vec3_z<T>, 0);
    DEF_MTHD(vec3, "[]", Vec3_idx<T>, 1);
    
    DEF_MTHD(vec3, "x=", Vec3_xeq<T>, 1);
    DEF_MTHD(vec3, "y=", Vec3_yeq<T>, 1);
    DEF_MTHD(vec3, "z=", Vec3_zeq<T>, 1);
    DEF_MTHD(vec3, "[]=", Vec3_idxeq<T>, 2);
    
    DEF_MTHD(vec3, "length", Vec3_length<T>, 0);
    DEF_MTHD(vec3, "length2", Vec3_length2<T>, 0);
    
    DEF_MTHD(vec3, "to_s", Vec3_to_s<T>, 0);
    
    DEF_MTHD(vec3, "to_a", Vec3_to_a<T>, 0);
    DEF_MTHD(vec3, "to_ary", Vec3_to_a<T>, 0);
}

void Init_Vec3()
{
    class_V3d = rb_define_class_under(module_Imath, "V3d", rb_cObject);
    Init_Vec3_Class<double>(class_V3d);
    
    class_V3i = rb_define_class_under(module_Imath, "V3i", rb_cObject);
    Init_Vec3_Class<int>(class_V3i);
}



//*******************************************************************************

//template <class Vec> Vec        project (const Vec &s, const Vec &t);
//template <class Vec> Vec        orthogonal (const Vec &s, const Vec &t);
//template <class Vec> Vec        reflect (const Vec &s, const Vec &t);
//template <class Vec> Vec        closestVertex (const Vec &v0, const Vec &v1, const Vec &v2, const Vec &p);
