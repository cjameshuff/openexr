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

#ifndef RB_VEC2_H
#define RB_VEC2_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathVec.h>

extern VALUE class_V2d;
extern VALUE class_V2i;

void Init_Vec2();


inline VALUE Vec2_new(const Imath::V2d & vval) {
    Imath::V2d * cval = new Imath::V2d(vval);
    VALUE val = Data_Wrap_Struct(class_V2d, NULL, CPP_DeleteFree<Imath::V2d>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
inline VALUE Vec2_new(const Imath::V2i & vval) {
    Imath::V2i * cval = new Imath::V2i(vval);
    VALUE val = Data_Wrap_Struct(class_V2i, NULL, CPP_DeleteFree<Imath::V2i>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}


template<typename T>
inline Imath::Vec2<T> * GetVec2(VALUE value) {
    Imath::Vec2<T> * val; Data_Get_Struct(value, Imath::Vec2<T>, val);
    return val;
}


template<typename T>
static bool ToVec2(VALUE val, Imath::Vec2<T> & vval)
{
    // TODO: truncate Vec3s and Vec4s
    if(CLASS_OF(val) == class_V2d) {
        vval = *GetVec2<double>(val);
        return true;
    }
    else if(CLASS_OF(val) == class_V2i) {
        vval = *GetVec2<int>(val);
        return true;
    }
    else if(IsNumType(val))
    {
        vval = Imath::Vec2<T>(rbpp_num_to<T>(val));
        return true;
    }
    else if(IsType(val, T_ARRAY))
    {
        long len = RARRAY_LEN(val);
        if(len == 0) {
            vval.x = 0;
            vval.y = 0;
        }
        else if(len == 1) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = 0;
        }
        else if(len == 2) {
            vval.x = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.y = rbpp_num_to<T>(rb_ary_entry(val, 1));
        }
        return true;
    }
    return false;
}


#endif // RB_VEC2_H
