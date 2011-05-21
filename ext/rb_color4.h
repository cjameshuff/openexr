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

#ifndef RB_COLOR4_H
#define RB_COLOR4_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathColor.h>

extern VALUE class_Color4f;

void Init_Color4();


inline VALUE Color4_new(const Imath::Color4f & vval) {
    Imath::Color4f * cval = new Imath::Color4f(vval);
    VALUE val = Data_Wrap_Struct(class_Color4f, NULL, CPP_DeleteFree<Imath::Color4f>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}
// inline VALUE Color4_new(const Imath::V4i & vval) {
//     Imath::V4i * cval = new Imath::V4i(vval);
//     VALUE val = Data_Wrap_Struct(class_V4i, NULL, CPP_DeleteFree<Imath::V4i>, (void *)cval);
//     rb_obj_call_init(val, 0, 0);
//     return val;
// }


template<typename T>
inline Imath::Color4<T> * GetColor4(VALUE value) {
    Imath::Color4<T> * val; Data_Get_Struct(value, Imath::Color4<T>, val);
    return val;
}


template<typename T>
static bool ToColor4(VALUE val, Imath::Color4<T> & vval)
{
    // TODO: convert Vec3s and Vec4s
    if(CLASS_OF(val) == class_Color4f)
    {
        vval = *GetColor4<float>(val);
        return true;
    }
    else if(IsNumType(val))
    {
        vval = Imath::Color4<T>(rbpp_num_to<T>(val));
        return true;
    }
    else if(IsType(val, T_ARRAY))
    {
        long len = RARRAY_LEN(val);
        if(len == 0) {
            vval.r = 0;
            vval.g = 0;
            vval.b = 0;
            vval.a = 1;
        }
        else if(len == 3) {
            vval.r = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.g = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.b = rbpp_num_to<T>(rb_ary_entry(val, 2));
            vval.a = 1;
        }
        else {
            vval.r = rbpp_num_to<T>(rb_ary_entry(val, 0));
            vval.g = rbpp_num_to<T>(rb_ary_entry(val, 1));
            vval.b = rbpp_num_to<T>(rb_ary_entry(val, 2));
            vval.a = rbpp_num_to<T>(rb_ary_entry(val, 3));
        }
        return true;
    }
    return false;
}


#endif // RB_COLOR4_H
