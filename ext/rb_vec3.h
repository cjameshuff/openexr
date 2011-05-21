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

#ifndef RB_VEC3_H
#define RB_VEC3_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathVec.h>

extern VALUE class_V3d;
extern VALUE class_V3i;

void Init_Vec3();


inline VALUE Vec3_new(const Imath::V3d & vval) {
    Imath::V3d * cval = new Imath::V3d(vval);
    VALUE val = Data_Wrap_Struct(class_V3d, NULL, CPP_DeleteFree<Imath::V3d>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
inline VALUE Vec3_new(const Imath::V3i & vval) {
    Imath::V3i * cval = new Imath::V3i(vval);
    VALUE val = Data_Wrap_Struct(class_V3i, NULL, CPP_DeleteFree<Imath::V3i>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}


template<typename T>
inline Imath::Vec3<T> * GetVec3(VALUE value) {
    Imath::Vec3<T> * val; Data_Get_Struct(value, Imath::Vec3<T>, val);
    return val;
}
// #define GetV3d  GetVec3<double>
// #define GetV3i  GetVec3<int>


template<typename T>
static bool ToVec3(VALUE val, Imath::Vec3<T> & vval)
{
    // TODO: truncate Vec4s and extend Vec2s
    if(CLASS_OF(val) == class_V3d)
    {
        vval = *GetVec3<double>(val);
        return true;
    }
    else if(CLASS_OF(val) == class_V3i)
    {
        vval = *GetVec3<int>(val);
        return true;
    }
    else if(IsNumType(val))
    {
        vval = Imath::Vec3<T>(rbpp_num_to<T>(val));
        return true;
    }
    else if(IsType(val, T_ARRAY))
    {
        long len = RARRAY_LEN(val);
        if(len == 0) {
            vval.x = 0;
            vval.y = 0;
            vval.z = 0;
        }
        else if(len == 1) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = 0;
            vval.z = 0;
        }
        else if(len == 2) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.z = 0;
        }
        else {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.z = rbpp_num_to<T>(rb_ary_entry(val, 2));
        }
        return true;
    }
    return false;
}


#endif // RB_VEC3_H
