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
#include "rb_quat.h"
#include "rb_vec3.h"

using namespace std;
using namespace Imath;

VALUE class_Quatd;

//rb_gc_mark(VALUE...)

// No references, do nothing
// Might make native Ruby value binding...
//void * QuatMark(void * st) {}

template<typename T>
static VALUE Quat_allocate(VALUE klass) {
    Quat<T> * cval = new Quat<T>();
    return Data_Wrap_Struct(klass, NULL, CPP_DeleteFree<Quat<T> >, (void *)cval);
}


//*******************************************************************************

template<typename T>
static VALUE Quat_init(int argc, VALUE * argv, VALUE self)
{
    // Possible argument combinations:
    // initialize()
    // initialize(quat)
    // initialize(Array vect)
    // initialize(numeric s, numeric x, numeric y, numeric z)
    // initialize(numeric s, V3d dir)
    // initialize(numeric s, Array vect)
    if(argc == 0)
        return self;
    
    VALUE a, b, c, d;
    rb_scan_args(argc, argv, "04", &a, &b, &c, &d);
    
    Quat<T> * selfval = GetQuat<T>(self);
    if(argc == 1) {
        *selfval = RequireQuat<T>(a);
    }
    else if(argc == 2) {
        selfval->r = rbpp_num_to<T>(a);
        selfval->v = RequireVec3<T>(b);
    }
    else if(argc == 4)
    {
        selfval->r = rbpp_num_to<T>(a);
        selfval->v.x = rbpp_num_to<T>(b);
        selfval->v.y = rbpp_num_to<T>(c);
        selfval->v.z = rbpp_num_to<T>(d);
    }
    else {
        rb_raise(rb_eArgError, "Expected 1, 2, or 4 arguments");
    }
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Quat_add(VALUE self, VALUE rb_rhs) {
    return Quat_new(*GetQuat<T>(self) + RequireQuat<T>(rb_rhs));
}

template<typename T>
static VALUE Quat_sub(VALUE self, VALUE rb_rhs) {
    return Quat_new(*GetQuat<T>(self) - RequireQuat<T>(rb_rhs));
}

// Matrix33<T>      operator * (const Quat<T> &q, const Matrix33<T> &M);
// Quat<T>          operator * (const Quat<T> &q1, const Quat<T> &q2);
// Quat<T>          operator * (const Quat<T> &q, T t);
template<typename T>
static VALUE Quat_mul(VALUE self, VALUE rb_rhs) {
    VALUE rb_rhs_f = rb_check_to_float(rb_rhs);
    if(rb_rhs_f != Qnil)
        rbpp_new(*GetQuat<T>(self)*rbpp_num_to<T>(rb_rhs_f));
    else
    // if(CLASS_OF(val) == class_Mat33d)
    //     Quat_new(*GetQuat<T>(self)*RequireMat33<T>(rb_rhs));
    // else
    return rbpp_new(*GetQuat<T>(self)*RequireQuat<T>(rb_rhs));
}

template<typename T>
static VALUE Quat_vec3_mul(VALUE self, VALUE rb_lhs) {
    return rbpp_new(RequireVec3<T>(rb_lhs)*(*GetQuat<T>(self)));
}
// TODO: Matrix33
// template<typename T>
// static VALUE Quat_mat33_mul(VALUE self, VALUE rb_lhs) {
//     return Quat_new(RequireMat33<T>(rb_lhs)*(*GetQuat<T>(self)));
// }
template<typename T>
static VALUE Quat_scalar_mul(VALUE self, VALUE rb_lhs) {
    return rbpp_new(rbpp_num_to<T>(rb_lhs)*(*GetQuat<T>(self)));
}

// Quat<T>          operator / (const Quat<T> &q1, const Quat<T> &q2);
// Quat<T>          operator / (const Quat<T> &q, T t);
template<typename T>
static VALUE Quat_div(VALUE self, VALUE rb_rhs) {
    Quat<T> * selfval = GetQuat<T>(self), rhsval = RequireQuat<T>(rb_rhs);
    return rbpp_new(*selfval + rhsval);
}


// Quat<T>          operator ~ (const Quat<T> &q);
// Quat<T>          operator - (const Quat<T> &q);
template<typename T>
static VALUE Quat_neg(VALUE self) {
    return rbpp_new(-(*GetQuat<T>(self)));
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

// Quat<T>          slerp (const Quat<T> &q1, const Quat<T> &q2, T t);
// Quat<T>          slerpShortestArc(const Quat<T> &q1, const Quat<T> &q2, T t);

// Quat<T>          squad (const Quat<T> &q1, const Quat<T> &q2, const Quat<T> &qa, const Quat<T> &qb, T t);

// void         intermediate (const Quat<T> &q0, const Quat<T> &q1, 
//                    const Quat<T> &q2, const Quat<T> &q3,
//                    Quat<T> &qa, Quat<T> &qb);

//*******************************************************************************

template<typename T>
static VALUE Quat_req(VALUE self, VALUE rhs) {
    GetQuat<T>(self)->r = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Quat_veq(VALUE self, VALUE rhs) {
    GetQuat<T>(self)->v = RequireVec3<T>(rhs);
    return self;
}

template<typename T>
static VALUE Quat_xeq(VALUE self, VALUE rhs) {
    GetQuat<T>(self)->v.x = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Quat_yeq(VALUE self, VALUE rhs) {
    GetQuat<T>(self)->v.y = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Quat_zeq(VALUE self, VALUE rhs) {
    GetQuat<T>(self)->v.z = rbpp_num_to<T>(rhs);
    return self;
}

template<typename T>
static VALUE Quat_idxeq(VALUE self, VALUE idx, VALUE rhs) {
    (*GetQuat<T>(self))[rbpp_num_to<int>(idx)] = rbpp_num_to<T>(rhs);
    return self;
}

//*******************************************************************************

template<typename T>
static VALUE Quat_r(VALUE self) {return rbpp_new(GetQuat<T>(self)->r);}
template<typename T>
static VALUE Quat_v(VALUE self) {return Vec3_new(GetQuat<T>(self)->v);}
template<typename T>
static VALUE Quat_x(VALUE self) {return rbpp_new(GetQuat<T>(self)->v.x);}
template<typename T>
static VALUE Quat_y(VALUE self) {return rbpp_new(GetQuat<T>(self)->v.y);}
template<typename T>
static VALUE Quat_z(VALUE self) {return rbpp_new(GetQuat<T>(self)->v.z);}

template<typename T>
static VALUE Quat_idx(VALUE self, VALUE idx) {
    return rbpp_new((*GetQuat<T>(self))[rbpp_num_to<int>(idx)]);
}

//*******************************************************************************

template<typename T>
static VALUE Quat_invert(VALUE self) {
    GetQuat<T>(self)->invert();
    return self;
}
template<typename T>
static VALUE Quat_inverse(VALUE self) {
    return Quat_new(GetQuat<T>(self)->inverse());
}

template<typename T>
static VALUE Quat_normalize(VALUE self) {
    GetQuat<T>(self)->normalize();
    return self;
}
template<typename T>
static VALUE Quat_normalized(VALUE self) {
    return Quat_new(GetQuat<T>(self)->normalized());
}

template<typename T>
static VALUE Quat_length(VALUE self) {return rbpp_new(GetQuat<T>(self)->length());}

template<typename T>
static VALUE Quat_angle(VALUE self) {return rbpp_new(GetQuat<T>(self)->angle());}
template<typename T>
static VALUE Quat_axis(VALUE self) {return Vec3_new(GetQuat<T>(self)->axis());}

template<typename T>
static VALUE Quat_identity(VALUE self) {return Quat_new(Quat<T>::identity());}

//*******************************************************************************

template<typename T>
static VALUE Quat_rotate_vector(VALUE self, VALUE rb_v) {
    return Vec3_new(GetQuat<T>(self)->rotateVector(RequireVec3<T>(rb_v)));
}
template<typename T>
static VALUE Quat_euclidian_inner_product(VALUE self, VALUE rb_v) {
    return rbpp_new(GetQuat<T>(self)->euclideanInnerProduct(RequireQuat<T>(rb_v)));
}

// const Quat<T> &  operator =  (const Quat<T> &q);
// const Quat<T> &  operator *= (const Quat<T> &q);
// const Quat<T> &  operator *= (T t);
// const Quat<T> &  operator /= (const Quat<T> &q);
// const Quat<T> &  operator /= (T t);
// const Quat<T> &  operator += (const Quat<T> &q);
// const Quat<T> &  operator -= (const Quat<T> &q);
// 
// template <class S> bool operator == (const Quat<S> &q) const;
// template <class S> bool operator != (const Quat<S> &q) const;
// 
// Quat<T> &        setAxisAngle (const Vec3<T> &axis, T radians);
// Quat<T> &        setRotation (const Vec3<T> &fromDirection, const Vec3<T> &toDirection);
// 
// Matrix33<T>      toMatrix33 () const;
// Matrix44<T>      toMatrix44 () const;

template<typename T>
static VALUE Quat_log(VALUE self) {return rbpp_new(GetQuat<T>(self)->log());}

template<typename T>
static VALUE Quat_exp(VALUE self) {return rbpp_new(GetQuat<T>(self)->exp());}

//*******************************************************************************

template<typename T>
static VALUE Quat_to_s(VALUE self, VALUE rhs) {
    stringstream os;
    os << *GetQuat<T>(self);
    return rb_str_new2(os.str().c_str());
}

template<typename T>
VALUE Quat_to_a(VALUE self, VALUE rhs)
{
    Quat<T> * selfval = GetQuat<T>(self);
    VALUE arr = rb_ary_new();
    rb_ary_push(arr, rbpp_new(selfval->r));
    rb_ary_push(arr, rbpp_new(selfval->v.x));
    rb_ary_push(arr, rbpp_new(selfval->v.y));
    rb_ary_push(arr, rbpp_new(selfval->v.z));
    return arr;
}

//*******************************************************************************


template<typename T>
void Init_Quat_Class(VALUE quat)
{
    rb_define_alloc_func(quat, Quat_allocate<T>);
//    rb_define_singleton_method(quat, "allocate", (VALUE (*)(...))V3d_allocate, 0);
    
    DEF_SING_MTHD(quat, "identity", Quat_identity<T>, 0);
    
    DEF_MTHD(quat, "initialize", Quat_init<T>, -1);
    
    DEF_MTHD(quat, "+", Quat_add<T>, 1);
    DEF_MTHD(quat, "-", Quat_sub<T>, 1);
    
    DEF_MTHD(quat, "*", Quat_mul<T>, 1);
    DEF_MTHD(quat, "*", Quat_vec3_mul<T>, 1);
    // DEF_MTHD(quat, "*", Quat_mat33_mul<T>, 1);
    DEF_MTHD(quat, "*", Quat_scalar_mul<T>, 1);
    
    DEF_MTHD(quat, "/", Quat_div<T>, 1);
    
    DEF_MTHD(quat, "-@", Quat_neg<T>, 0);
    
    // DEF_MTHD(quat, "coerce", Quat_coerce<T>, 1);// Implemented in Ruby
    
    DEF_MTHD(quat, "normalize", Quat_normalize<T>, 0);
    DEF_MTHD(quat, "normalized", Quat_normalized<T>, 0);
    DEF_MTHD(quat, "invert", Quat_invert<T>, 0);
    DEF_MTHD(quat, "inverse", Quat_inverse<T>, 0);
    
    DEF_MTHD(quat, "r", Quat_r<T>, 0);
    DEF_MTHD(quat, "v", Quat_v<T>, 0);
    DEF_MTHD(quat, "x", Quat_x<T>, 0);
    DEF_MTHD(quat, "y", Quat_y<T>, 0);
    DEF_MTHD(quat, "z", Quat_z<T>, 0);
    DEF_MTHD(quat, "[]", Quat_idx<T>, 1);
    
    DEF_MTHD(quat, "r=", Quat_req<T>, 1);
    DEF_MTHD(quat, "v=", Quat_veq<T>, 1);
    DEF_MTHD(quat, "x=", Quat_xeq<T>, 1);
    DEF_MTHD(quat, "y=", Quat_yeq<T>, 1);
    DEF_MTHD(quat, "z=", Quat_zeq<T>, 1);
    DEF_MTHD(quat, "[]=", Quat_idxeq<T>, 2);
    
    DEF_MTHD(quat, "length", Quat_length<T>, 0);
    DEF_MTHD(quat, "angle", Quat_angle<T>, 0);
    DEF_MTHD(quat, "axis", Quat_axis<T>, 0);
    DEF_MTHD(quat, "rotate_vector", Quat_rotate_vector<T>, 1);
    DEF_MTHD(quat, "euclidian_inner_product", Quat_euclidian_inner_product<T>, 1);
    
    DEF_MTHD(quat, "to_s", Quat_to_s<T>, 0);
    
    DEF_MTHD(quat, "to_a", Quat_to_a<T>, 0);
    DEF_MTHD(quat, "to_ary", Quat_to_a<T>, 0);
}

void Init_Quat()
{
    class_Quatd = rb_define_class_under(module_Imath, "Quatd", rb_cObject);
    Init_Quat_Class<double>(class_Quatd);
}

//*******************************************************************************
