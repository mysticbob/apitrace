/**************************************************************************
 *
 * Copyright 2011 Jose Fonseca
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/


#include <string.h>

#include "glproc.hpp"
#include "retrace.hpp"
#include "glretrace.hpp"


using namespace glretrace;


typedef std::map<unsigned long long, glws::Drawable *> DrawableMap;
typedef std::map<unsigned long long, glws::Context *> ContextMap;
static DrawableMap drawable_map;
static ContextMap context_map;
static glws::Context *sharedContext = NULL;


static glws::Drawable *
getDrawable(unsigned long drawable_id) {
    if (drawable_id == 0) {
        return NULL;
    }

    DrawableMap::const_iterator it;
    it = drawable_map.find(drawable_id);
    if (it == drawable_map.end()) {
        return (drawable_map[drawable_id] = ws->createDrawable(visual));
    }

    return it->second;
}


static glws::Context *
getContext(unsigned long long ctx) {
    if (ctx == 0) {
        return NULL;
    }

    ContextMap::const_iterator it;
    it = context_map.find(ctx);
    if (it == context_map.end()) {
        glws::Context *context;
        context_map[ctx] = context = ws->createContext(visual, sharedContext);
        if (!sharedContext) {
            sharedContext = context;
        }
        return context;
    }

    return it->second;
}

static void retrace_CGLSetCurrentContext(Trace::Call &call) {
    unsigned long long ctx = call.arg(0).toUIntPtr();

    /*
     * XXX: Frame termination is mostly a guess, because we don't trace enough
     * of the CGL API to know that.
     */
    if (drawable && context) {
        if (double_buffer) {
            drawable->swapBuffers();
        } else {
            glFlush();
        }

        frame_complete(call.no);
    }

    glws::Drawable *new_drawable = getDrawable(ctx);
    glws::Context *new_context = getContext(ctx);

    bool result = ws->makeCurrent(new_drawable, new_context);

    if (new_drawable && new_context && result) {
        drawable = new_drawable;
        context = new_context;
    } else {
        drawable = NULL;
        context = NULL;
    }
}


void glretrace::retrace_call_cgl(Trace::Call &call) {
    const char *name = call.name().c_str();

    if (strcmp(name, "CGLSetCurrentContext") == 0) {
       retrace_CGLSetCurrentContext(call);
       return;
    }

    if (strcmp(name, "CGLGetCurrentContext") == 0) {
       return;
    }

    retrace::retrace_unknown(call);
}

