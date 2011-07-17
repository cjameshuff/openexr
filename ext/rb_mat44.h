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

#ifndef RB_MAT4_H
#define RB_MAT4_H

#include "ruby.h"
#include "rubyhelpers.h"
#include <ImathMatrix.h>

extern VALUE class_M44d;

void Init_Matrix44();


inline VALUE Matrix44_new(const Imath::M44d & vval) {
    Imath::M44d * cval = new Imath::M44d(vval);
    VALUE val = Data_Wrap_Struct(class_M44d, NULL, CPP_DeleteFree<Imath::M44d>, (void *)cval);
    rb_obj_call_init(val, 0, 0);
    return val;
}


template<typename T>
inline Imath::Matrix44<T> * GetMatrix44(VALUE value) {
    Imath::Matrix44<T> * val; Data_Get_Struct(value, Imath::Matrix44<T>, val);
    return val;
}


template<typename T>
static bool ToMatrix44(VALUE val, Imath::Matrix44<T> & vval)
{
    if(CLASS_OF(val) == class_M44d)
    {
        vval = *GetMatrix44<double>(val);
        return true;
    }
    return false;
}


#endif // RB_MAT4_H
