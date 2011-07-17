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
#include "rb_mat44.h"
#include "rb_vec3.h"
#include "rb_vec4.h"

using namespace std;
using namespace Imath;

VALUE class_M44d;

template<typename T>
static VALUE Matrix44_allocate(VALUE klass) {
    Matrix44<T> * cval = new Matrix44<T>();
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Matrix44<T> >, (void *)cval);
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_init(int argc, VALUE *argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(numeric val)
    // initialize(numeric x, numeric y, numeric z)
    // initialize(M44d vect)
    // initialize(M44d dir, numeric len)
    // initialize(Array vect)
    // initialize(Array dir, numeric len)
    if(argc == 0)
    {
        return self;
    }
    
    // VALUE a, b, c, d;
    // rb_scan_args(argc, argv, "04", &a, &b, &c, &d);
    // 
    // Matrix44<T> * selfval = GetMatrix44<T>(self), vval;
    // 
    // if(argc == 1)
    // {
    //     if(ToMatrix44(a, vval))
    //         *selfval = vval;
    //     else
    //      rb_raise(rb_eArgError, "Expected a numeric type, Matrix44, or Array");
    // }
    // else if(argc == 2)
    // {
    //     if(IsNumType(a)) {
    //         selfval->x = rbpp_num_to<T>(a);
    //         selfval->y = rbpp_num_to<T>(b);
    //     }
    //     else if(ToMatrix44(a, vval)) {
    //         *selfval = (vval)*(rbpp_num_to<T>(b)/vval.length());
    //     }
    //     else {
    //      rb_raise(rb_eArgError, "Expected a numeric type, Matrix44, or Array");
    //     }
    // }
    // else if(argc == 3)
    // {
    //     selfval->x = rbpp_num_to<T>(a);
    //     selfval->y = rbpp_num_to<T>(b);
    //     selfval->z = rbpp_num_to<T>(c);
    //     selfval->w = 0;
    // }
    // else if(argc == 4)
    // {
    //     selfval->x = rbpp_num_to<T>(a);
    //     selfval->y = rbpp_num_to<T>(b);
    //     selfval->z = rbpp_num_to<T>(c);
    //     selfval->w = rbpp_num_to<T>(d);
    // }
    // else {
    //  rb_raise(rb_eArgError, "Expected 1, 2, 3, or 4 arguments");
    // }
    return self;
}

//*******************************************************************************

// TODO: straighten out matrix-vector math
// Matrices can be added/subtracted from matrices and scalars
// Matrices can be negated
// Matrices can be multiplied by matrices, vectors, and scalars.
// Vector-matrix multiplication is not commutative. v*m != m*v
// Matrices can be divided by scalars
template<typename T>
static VALUE Matrix44_add(VALUE self, VALUE rhs) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(ToMatrix44(rhs, rhsval))
        return Matrix44_new(*selfval + rhsval);
    else rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
}

template<typename T>
static VALUE Matrix44_sub(VALUE self, VALUE rhs) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(ToMatrix44(rhs, rhsval))
        return Matrix44_new(*selfval - rhsval);
    else rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
}

template<typename T>
static VALUE Matrix44_mul(VALUE self, VALUE rhs) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(ToMatrix44(rhs, rhsval))
        return Matrix44_new(*selfval * rhsval);
    else rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
}

template<typename T>
static VALUE Matrix44_div(VALUE self, VALUE rhs) {
    // Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    // if(ToMatrix44(rhs, rhsval))
    //     return Matrix44_new(*selfval / rhsval);
    // else rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
    rb_raise(rb_eArgError, "Feature not yet implemented");
}

//*******************************************************************************

template<typename T>
VALUE Matrix44_mult_vec_matrix(VALUE self, VALUE rbvec)
{
    Matrix44<T> * selfval = GetMatrix44<T>(self);
    Imath::Vec3<T> * vec = GetVec3<T>(rbvec);
    Imath::Vec3<T> result;
    VALUE arr = rb_ary_new();
    selfval->multVecMatrix(*vec, result);
    return Vec3_new(result);
}

template<typename T>
VALUE Matrix44_mult_dir_matrix(VALUE self, VALUE rbvec)
{
    Matrix44<T> * selfval = GetMatrix44<T>(self);
    Imath::Vec3<T> * vec = GetVec3<T>(rbvec);
    Imath::Vec3<T> result;
    VALUE arr = rb_ary_new();
    selfval->multDirMatrix(*vec, result);
    return Vec3_new(result);
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_eq(VALUE self, VALUE rhs) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(!ToMatrix44(rhs, rhsval))
        rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
    return (*selfval == rhsval)? Qtrue : Qfalse;
}

template<typename T>
static VALUE Matrix44_neq(VALUE self, VALUE rhs) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(!ToMatrix44(rhs, rhsval))
        rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
    return (*selfval != rhsval)? Qtrue : Qfalse;
}

template<typename T>
static VALUE Matrix44_eq_w_abs_err(VALUE self, VALUE rhs, VALUE err) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(!ToMatrix44(rhs, rhsval))
        rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
    return (selfval->equalWithAbsError(rhsval, rbpp_num_to<T>(err)))? Qtrue : Qfalse;
}

template<typename T>
static VALUE Matrix44_eq_w_rel_err(VALUE self, VALUE rhs, VALUE err) {
    Matrix44<T> * selfval = GetMatrix44<T>(self), rhsval;
    if(!ToMatrix44(rhs, rhsval))
        rb_raise(rb_eArgError, "Expected a Matrix44, Array, or numeric type");
    return (selfval->equalWithRelError(rhsval, rbpp_num_to<T>(err)))? Qtrue : Qfalse;
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_make_identity(VALUE self) {
    GetMatrix44<T>(self)->makeIdentity();
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_transpose(VALUE self) {
    GetMatrix44<T>(self)->transpose();
    return self;
}
template<typename T>
static VALUE Matrix44_transposed(VALUE self) {return Matrix44_new(GetMatrix44<T>(self)->transposed());}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_invert(VALUE self) {
    GetMatrix44<T>(self)->invert();
    return self;
}
template<typename T>
static VALUE Matrix44_inverse(VALUE self) {return Matrix44_new(GetMatrix44<T>(self)->inverse());}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_gj_invert(VALUE self) {
    GetMatrix44<T>(self)->gjInvert();
    return self;
}
template<typename T>
static VALUE Matrix44_gj_inverse(VALUE self) {return Matrix44_new(GetMatrix44<T>(self)->gjInverse());}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_idx(VALUE self, VALUE ridx, VALUE cidx) {
    int r = rbpp_num_to<int>(ridx);
    int c = rbpp_num_to<int>(cidx);
    return rbpp_new((*GetMatrix44<T>(self))[r][c]);
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_idxeq(VALUE self, VALUE ridx, VALUE cidx, VALUE rhs) {
    int r = rbpp_num_to<int>(ridx);
    int c = rbpp_num_to<int>(cidx);
    (*GetMatrix44<T>(self))[r][c] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Matrix44_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetMatrix44<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Matrix44_rows(VALUE self, VALUE rhs)
{
    Matrix44<T> * selfval = GetMatrix44<T>(self);
    VALUE arr = rb_ary_new();
    for(int i = 0; i < 4; ++i) {
        Imath::V4d vval((*selfval)[i][0], (*selfval)[i][1], (*selfval)[i][2], (*selfval)[i][3]);
        rb_ary_push(arr, Vec4_new(vval));
    }
    return arr;
}

template<typename T>
VALUE Matrix44_columns(VALUE self, VALUE rhs)
{
    Matrix44<T> * selfval = GetMatrix44<T>(self);
    VALUE arr = rb_ary_new();
    for(int i = 0; i < 4; ++i) {
        Imath::V4d vval((*selfval)[0][i], (*selfval)[1][i], (*selfval)[2][i], (*selfval)[3][i]);
        rb_ary_push(arr, Vec4_new(vval));
    }
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Matrix44_Class(VALUE m44)
{
    rb_define_alloc_func(m44, Matrix44_allocate<T>);
//    rb_define_singleton_method(m44, "allocate", (VALUE (*)(...))M44d_allocate, 0);
    DEF_MTHD(m44, "initialize", Matrix44_init<T>, -1);
    
    DEF_MTHD(m44, "+", Matrix44_add<T>, 1);
    DEF_MTHD(m44, "-", Matrix44_sub<T>, 1);
    DEF_MTHD(m44, "*", Matrix44_mul<T>, 1);
    DEF_MTHD(m44, "/", Matrix44_div<T>, 1);
    
    DEF_MTHD(m44, "==", Matrix44_eq<T>, 1);
    DEF_MTHD(m44, "!=", Matrix44_neq<T>, 1);
    
    DEF_MTHD(m44, "[]", Matrix44_idx<T>, 1);
    DEF_MTHD(m44, "[]=", Matrix44_idxeq<T>, 2);
    
    DEF_MTHD(m44, "equal_with_abs_error", Matrix44_eq_w_abs_err<T>, 2);
    DEF_MTHD(m44, "equal_with_rel_error", Matrix44_eq_w_rel_err<T>, 2);
    
    DEF_MTHD(m44, "mult_vec_matrix", Matrix44_mult_vec_matrix<T>, 1);
    DEF_MTHD(m44, "mult_dir_matrix", Matrix44_mult_dir_matrix<T>, 1);
    
    DEF_MTHD(m44, "make_identity", Matrix44_make_identity<T>, 0);
    
    DEF_MTHD(m44, "transpose", Matrix44_transpose<T>, 0);
    DEF_MTHD(m44, "transposed", Matrix44_transposed<T>, 0);
    
    DEF_MTHD(m44, "invert", Matrix44_invert<T>, 0);
    DEF_MTHD(m44, "inverse", Matrix44_inverse<T>, 0);
    
    DEF_MTHD(m44, "gj_invert", Matrix44_gj_invert<T>, 0);
    DEF_MTHD(m44, "gj_inverse", Matrix44_gj_inverse<T>, 0);
    
    // const Matrix44 &    setEulerAngles (const Vec3<S>& r);
    // const Matrix44 &    setAxisAngle (const Vec3<S>& ax, S ang);
    // const Matrix44 &    rotate (const Vec3<S> &r);
    // const Matrix44 &    setScale (T s);
    // const Matrix44 &    setScale (const Vec3<S> &s);
    // const Matrix44 &    scale (const Vec3<S> &s);
    // const Matrix44 &    setTranslation (const Vec3<S> &t);
    // const Vec3<T>       translation () const;
    // const Matrix44 &    translate (const Vec3<S> &t);
    // const Matrix44 &    setShear (const Vec3<S> &h);
    // const Matrix44 &    setShear (const Shear6<S> &h);
    // const Matrix44 &    shear (const Vec3<S> &h);
    // const Matrix44 &    shear (const Shear6<S> &h);
    // static T            baseTypeMin()           {return limits<T>::min();}
    // static T            baseTypeMax()           {return limits<T>::max();}
    // static T            baseTypeSmallest()      {return limits<T>::smallest();}
    // static T            baseTypeEpsilon()       {return limits<T>::epsilon();}
    
    DEF_MTHD(m44, "to_s", Matrix44_to_s<T>, 0);
    
    DEF_MTHD(m44, "rows", Matrix44_rows<T>, 0);
    DEF_MTHD(m44, "columns", Matrix44_columns<T>, 0);
    
    DEF_MTHD(m44, "to_a", Matrix44_rows<T>, 0);
    DEF_MTHD(m44, "to_ary", Matrix44_rows<T>, 0);
}

void Init_Matrix44()
{
    class_M44d = rb_define_class_under(module_Imath, "M44d", rb_cObject);
    Init_Matrix44_Class<double>(class_M44d);
}

//*******************************************************************************
