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

#ifndef RB_VEC4_H
#define RB_VEC4_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathVec.h>

extern VALUE class_V4d;
extern VALUE class_V4i;

void Init_Vec4();


inline VALUE Vec4_new(const Imath::V4d & vval) {
    Imath::V4d * cval = new Imath::V4d(vval);
    VALUE val = Data_Wrap_Struct(class_V4d, NULL, CPP_DeleteFree<Imath::V4d>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
inline VALUE Vec4_new(const Imath::V4i & vval) {
    Imath::V4i * cval = new Imath::V4i(vval);
    VALUE val = Data_Wrap_Struct(class_V4i, NULL, CPP_DeleteFree<Imath::V4i>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
inline VALUE rbpp_new(const Imath::V4d & val) {return Vec4_new(val);}
inline VALUE rbpp_new(const Imath::V4i & val) {return Vec4_new(val);}


template<typename T>
inline Imath::Vec4<T> * GetVec4(VALUE value) {
    Imath::Vec4<T> * val; Data_Get_Struct(value, Imath::Vec4<T>, val);
    return val;
}


template<typename T>
static bool ToVec4(VALUE val, Imath::Vec4<T> & vval)
{
    // TODO: extend Vec2s and Vec3s
    if(CLASS_OF(val) == class_V4d)
    {
        vval = *GetVec4<double>(val);
        return true;
    }
    else if(CLASS_OF(val) == class_V4i)
    {
        vval = *GetVec4<int>(val);
        return true;
    }
    else if(IsNumType(val))
    {
        vval = Imath::Vec4<T>(rbpp_num_to<T>(val));
        return true;
    }
    else if(IsType(val, T_ARRAY))
    {
        long len = RARRAY_LEN(val);
        if(len == 0) {
            vval.x = 0;
            vval.y = 0;
            vval.z = 0;
            vval.w = 0;
        }
        else if(len == 1) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = 0;
            vval.z = 0;
            vval.w = 0;
        }
        else if(len == 2) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.z = 0;
            vval.w = 0;
        }
        else if(len == 3) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.z = rbpp_num_to<T>(rb_ary_entry(val, 2));
            vval.w = 0;
        }
        else {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.z = rbpp_num_to<T>(rb_ary_entry(val, 2));
            vval.w = rbpp_num_to<T>(rb_ary_entry(val, 3));
        }
        return true;
    }
    return false;
}

template<typename T>
static Imath::Vec4<T> RequireVec4(VALUE rb_val)
{
    Imath::Vec4<T> vval;
    if(ToVec4(rb_val, vval))
        return vval;
    else rb_raise(rb_eArgError, "Can not be converted to V4.");
}


#endif // RB_VEC4_H
