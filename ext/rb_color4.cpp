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

#include <iostream>
#include <sstream>
#include <fstream>

#include "rb_openexr.h"
#include "rb_color4.h"

using namespace std;
using namespace Imath;

VALUE class_Color4f;

template<typename T>
static VALUE Color4_allocate(VALUE klass) {
    Color4<T> * cval = new Color4<T>(0, 0, 0, 0);
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Color4<T> >, (void *)cval);
}

//*******************************************************************************

template<typename T>
static VALUE Color4_init(int argc, VALUE *argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(numeric val)
    // initialize(numeric r, numeric g, numeric b)
    // initialize(numeric r, numeric g, numeric b, numeric a)
    // initialize(Color4 col)
    // initialize(Array col)
    if(argc == 0)
    {
        return self;
    }
    
    VALUE a, b, c, d;
    rb_scan_args(argc, argv, "04", &a, &b, &c, &d);
    
    Color4<T> * selfval = GetColor4<T>(self), vval;
    
    if(argc == 1)
    {
        if(ToColor4(a, vval))
            *selfval = vval;
        else
        	rb_raise(rb_eArgError, "Expected a numeric type, Color4, or Array");
    }
    else if(argc == 3)
    {
        selfval->r = rbpp_num_to<T>(a);
        selfval->g = rbpp_num_to<T>(b);
        selfval->b = rbpp_num_to<T>(c);
        selfval->a = 0;
    }
    else if(argc == 4)
    {
        selfval->r = rbpp_num_to<T>(a);
        selfval->g = rbpp_num_to<T>(b);
        selfval->b = rbpp_num_to<T>(c);
        selfval->a = rbpp_num_to<T>(d);
    }
    else {
    	rb_raise(rb_eArgError, "Expected 1, 3, or 4 arguments");
    }
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Color4_add(VALUE self, VALUE rhs) {
    Color4<T> * selfval = GetColor4<T>(self), rhsval;
    if(ToColor4(rhs, rhsval))
        return Color4_new(*selfval + rhsval);
    else rb_raise(rb_eArgError, "Expected a Color4, Array, or numeric type");
}

template<typename T>
static VALUE Color4_sub(VALUE self, VALUE rhs) {
    Color4<T> * selfval = GetColor4<T>(self), rhsval;
    if(ToColor4(rhs, rhsval))
        return Color4_new(*selfval - rhsval);
    else rb_raise(rb_eArgError, "Expected a Color4, Array, or numeric type");
}

template<typename T>
static VALUE Color4_mul(VALUE self, VALUE rhs) {
    Color4<T> * selfval = GetColor4<T>(self), rhsval;
    if(ToColor4(rhs, rhsval))
        return Color4_new(*selfval * rhsval);
    else rb_raise(rb_eArgError, "Expected a Color4, Array, or numeric type");
}

template<typename T>
static VALUE Color4_div(VALUE self, VALUE rhs) {
    Color4<T> * selfval = GetColor4<T>(self), rhsval;
    if(ToColor4(rhs, rhsval))
        return Color4_new(*selfval / rhsval);
    else rb_raise(rb_eArgError, "Expected a Color4, Array, or numeric type");
}

//*******************************************************************************

template<typename T>
static VALUE Color4_coerce(VALUE self, VALUE lhs) {
    Color4<T> lhsval;
    if(!ToColor4(lhs, lhsval))
    	rb_raise(rb_eArgError, "LHS must be a Color4, Array, or numeric type");
    
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, Color4_new(lhsval));
    rb_ary_push(arr, self);
    return arr;
}

//*******************************************************************************

template<typename T>
static VALUE Color4_req(VALUE self, VALUE rhs) {
    GetColor4<T>(self)->r = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Color4_geq(VALUE self, VALUE rhs) {
    GetColor4<T>(self)->g = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Color4_beq(VALUE self, VALUE rhs) {
    GetColor4<T>(self)->b = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Color4_aeq(VALUE self, VALUE rhs) {
    GetColor4<T>(self)->a = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Color4_idxeq(VALUE self, VALUE idx, VALUE rhs) {
    (*GetColor4<T>(self))[rbpp_num_to<int>(idx)] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Color4_r(VALUE self) {return rbpp_new(GetColor4<T>(self)->r);}
template<typename T>
static VALUE Color4_g(VALUE self) {return rbpp_new(GetColor4<T>(self)->g);}
template<typename T>
static VALUE Color4_b(VALUE self) {return rbpp_new(GetColor4<T>(self)->b);}
template<typename T>
static VALUE Color4_a(VALUE self) {return rbpp_new(GetColor4<T>(self)->a);}

template<typename T>
static VALUE Color4_idx(VALUE self, VALUE idx) {
    return rbpp_new((*GetColor4<T>(self))[rbpp_num_to<int>(idx)]);
}

//*******************************************************************************

template<typename T>
static VALUE Color4_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetColor4<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Color4_to_a(VALUE self, VALUE rhs)
{
    Color4<T> * selfval = GetColor4<T>(self);
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, rbpp_new(selfval->r));
    rb_ary_push(arr, rbpp_new(selfval->g));
    rb_ary_push(arr, rbpp_new(selfval->b));
    rb_ary_push(arr, rbpp_new(selfval->a));
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Color4_Class(VALUE col4)
{
    rb_define_alloc_func(col4, Color4_allocate<T>);
    DEF_MTHD(col4, "initialize", Color4_init<T>, -1);
    
    DEF_MTHD(col4, "+", Color4_add<T>, 1);
    DEF_MTHD(col4, "-", Color4_sub<T>, 1);
    DEF_MTHD(col4, "*", Color4_mul<T>, 1);
    DEF_MTHD(col4, "/", Color4_div<T>, 1);
    
    DEF_MTHD(col4, "coerce", Color4_coerce<T>, 1);
    
    DEF_MTHD(col4, "r", Color4_r<T>, 0);
    DEF_MTHD(col4, "g", Color4_g<T>, 0);
    DEF_MTHD(col4, "b", Color4_b<T>, 0);
    DEF_MTHD(col4, "a", Color4_a<T>, 0);
    DEF_MTHD(col4, "[]", Color4_idx<T>, 1);
    
    DEF_MTHD(col4, "r=", Color4_req<T>, 1);
    DEF_MTHD(col4, "g=", Color4_geq<T>, 1);
    DEF_MTHD(col4, "b=", Color4_beq<T>, 1);
    DEF_MTHD(col4, "a=", Color4_aeq<T>, 1);
    DEF_MTHD(col4, "[]=", Color4_idxeq<T>, 2);
    
    DEF_MTHD(col4, "to_s", Color4_to_s<T>, 0);
    
    DEF_MTHD(col4, "to_a", Color4_to_a<T>, 0);
    DEF_MTHD(col4, "to_ary", Color4_to_a<T>, 0);
}

void Init_Color4()
{
    class_Color4f = rb_define_class_under(module_Imath, "Color4f", rb_cObject);
    Init_Color4_Class<float>(class_Color4f);
}


//*******************************************************************************
