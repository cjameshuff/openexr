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

#include "ruby.h"
#include "rubyhelpers.h"

#include "rb_openexr.h"
#include "rb_vec2.h"
#include "rb_vec3.h"
#include "rb_vec4.h"
#include "rb_color4.h"

using namespace std;


VALUE module_Imath = Qnil;
VALUE module_Iex = Qnil;
VALUE module_Imf = Qnil;
VALUE module_Ilm = Qnil;

extern "C" void Init_openexr_native()
{
    module_Imath = rb_define_module("Imath");
    module_Iex = rb_define_module("Iex");
    module_Imf = rb_define_module("Imf");
    module_Ilm = rb_define_module("Ilm");
    
    Init_Vec2();
    Init_Vec3();
    Init_Vec4();
    Init_Color4();
}
