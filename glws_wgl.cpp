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

#include "glimports.hpp"
#include "glws.hpp"


namespace glws {


static LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MINMAXINFO *pMMI;
    switch (uMsg) {
    case WM_GETMINMAXINFO:
        // Allow to create a window bigger than the desktop
        pMMI = (MINMAXINFO *)lParam;
        pMMI->ptMaxSize.x = 60000;
        pMMI->ptMaxSize.y = 60000;
        pMMI->ptMaxTrackSize.x = 60000;
        pMMI->ptMaxTrackSize.y = 60000;
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


class WglDrawable : public Drawable
{
public:
    DWORD dwExStyle;
    DWORD dwStyle;
    HWND hWnd;
    HDC hDC;
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormat;

    WglDrawable(const Visual *vis, int width, int height) :
        Drawable(vis, width, height)
    {
        static bool first = TRUE;
        RECT rect;

        if (first) {
            WNDCLASS wc;
            memset(&wc, 0, sizeof wc);
            wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.lpfnWndProc = WndProc;
            wc.lpszClassName = "glretrace";
            wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            RegisterClass(&wc);
            first = FALSE;
        }

        dwExStyle = 0;
        dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;

        int x = 0, y = 0;

        rect.left = x;
        rect.top = y;
        rect.right = rect.left + width;
        rect.bottom = rect.top + height;

        AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

        hWnd = CreateWindowEx(dwExStyle,
                              "glretrace", /* wc.lpszClassName */
                              NULL,
                              dwStyle,
                              0, /* x */
                              0, /* y */
                              rect.right - rect.left, /* width */
                              rect.bottom - rect.top, /* height */
                              NULL,
                              NULL,
                              NULL,
                              NULL);
        hDC = GetDC(hWnd);
   
        memset(&pfd, 0, sizeof pfd);
        pfd.cColorBits = 4;
        pfd.cRedBits = 1;
        pfd.cGreenBits = 1;
        pfd.cBlueBits = 1;
        pfd.cAlphaBits = 1;
        pfd.cDepthBits = 1;
        pfd.cStencilBits = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;

        if (visual->doubleBuffer) {
           pfd.dwFlags |= PFD_DOUBLEBUFFER;
        }

        iPixelFormat = ChoosePixelFormat(hDC, &pfd);

        SetPixelFormat(hDC, iPixelFormat, &pfd);
    }

    ~WglDrawable() {
        ReleaseDC(hWnd, hDC);
        DestroyWindow(hWnd);
    }
    
    void
    resize(int w, int h) {
        Drawable::resize(w, h);
        RECT rClient, rWindow;
        GetClientRect(hWnd, &rClient);
        GetWindowRect(hWnd, &rWindow);
        w += (rWindow.right  - rWindow.left) - rClient.right;
        h += (rWindow.bottom - rWindow.top)  - rClient.bottom;
        SetWindowPos(hWnd, NULL, rWindow.left, rWindow.top, w, h, SWP_NOMOVE);
    }

    void show(void) {
        if (!visible) {
            ShowWindow(hWnd, SW_SHOW);

            Drawable::show();
        }
    }

    void swapBuffers(void) {
        SwapBuffers(hDC);
    }
};


class WglContext : public Context
{
public:
    HGLRC hglrc;
    WglContext *shareContext;

    WglContext(const Visual *vis, WglContext *share) :
        Context(vis),
        hglrc(0),
        shareContext(share)
    {}

    ~WglContext() {
        if (hglrc) {
            wglDeleteContext(hglrc);
        }
    }
};


class WglWindowSystem : public WindowSystem
{
public:
    Visual *
    createVisual(bool doubleBuffer) {
        Visual *visual = new Visual();

        visual->doubleBuffer = doubleBuffer;

        return visual;
    }
    
    Drawable *
    createDrawable(const Visual *visual, int width, int height)
    {
        return new WglDrawable(visual, width, height);
    }

    Context *
    createContext(const Visual *visual, Context *shareContext)
    {
        return new WglContext(visual, dynamic_cast<WglContext *>(shareContext));
    }

    bool
    makeCurrent(Drawable *drawable, Context *context)
    {
        if (!drawable || !context) {
            return wglMakeCurrent(NULL, NULL);
        } else {
            WglDrawable *wglDrawable = dynamic_cast<WglDrawable *>(drawable);
            WglContext *wglContext = dynamic_cast<WglContext *>(context);

            if (!wglContext->hglrc) {
                wglContext->hglrc = wglCreateContext(wglDrawable->hDC);
                if (!wglContext->hglrc) {
                    return false;
                }
                if (wglContext->shareContext) {
                    wglShareLists(wglContext->shareContext->hglrc,
                                  wglContext->hglrc);
                }
            }

            return wglMakeCurrent(wglDrawable->hDC, wglContext->hglrc);
        }
    }

    bool
    processEvents(void) {
        // TODO
        return true;
    }
};


WindowSystem *createNativeWindowSystem(void) {
    return new WglWindowSystem();
}


} /* namespace glretrace */
