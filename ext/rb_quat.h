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

#ifndef RB_QUAT_H
#define RB_QUAT_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathQuat.h>

extern VALUE class_Quatd;

void Init_Quat();


inline VALUE Quat_new(const Imath::Quatd & qval) {
    Imath::Quatd * cval = new Imath::Quatd(qval);
    VALUE val = Data_Wrap_Struct(class_Quatd, NULL, CPP_DeleteFree<Imath::Quatd>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
inline VALUE rbpp_new(const Imath::Quatd & qval) {return Quat_new(qval);}


template<typename T>
inline Imath::Quat<T> * GetQuat(VALUE value) {
    Imath::Quat<T> * val; Data_Get_Struct(value, Imath::Quat<T>, val);
    return val;
}


template<typename T>
static bool ToQuat(VALUE val, Imath::Quat<T> & qval)
{
    if(CLASS_OF(val) == class_Quatd) {
        qval = *GetQuat<double>(val);
        return true;
    }
    else if(IsType(val, T_ARRAY) && RARRAY_LEN(val) == 4)
    {
        qval.r = rbpp_num_to<T>(rb_ary_entry(val, 0));
        qval.v.x = rbpp_num_to<T>(rb_ary_entry(val, 1));
        qval.v.y = rbpp_num_to<T>(rb_ary_entry(val, 2));
        qval.v.z = rbpp_num_to<T>(rb_ary_entry(val, 3));
        return true;
    }
    return false;
}

template<typename T>
static Imath::Quat<T> RequireQuat(VALUE rb_val)
{
    Imath::Quat<T> vval;
    if(ToQuat(rb_val, vval))
        return vval;
    else rb_raise(rb_eArgError, "Expected a Quat");
}


#endif // RB_QUAT_H
