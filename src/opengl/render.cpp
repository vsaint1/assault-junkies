#include "render.h"



void OpenglHook::ortho() {


    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
}

void OpenglHook::restore() {

    glPopMatrix();

    glPopAttrib();
}

bool OpenglHook::build_font() {

    this->hdc = wglGetCurrentDC();
    this->base = glGenLists(96);
    HFONT font = CreateFontA(-this->m_height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, this->m_font);
    HFONT old_font = (HFONT)SelectObject(hdc, font);
    wglUseFontBitmapsW(hdc, 32, 96, base);
    SelectObject(hdc, old_font);
    DeleteObject(font);
    return true;
}

void OpenglHook::draw_line(Vector3 lvalue, Vector3 rvalue, const unsigned char color[3]) {

    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_LINES);
    glVertex2f(lvalue.x, lvalue.y);
    glVertex2f(rvalue.x, rvalue.y);
    glEnd();
}

void OpenglHook::draw_text(int x, int y, const unsigned char color[3], const char *fmt, ...) {
    glColor3ub(color[0], color[1], color[2]);
    glRasterPos2i(x, y);
    char text[128];
    char *args;

    __crt_va_start(args, fmt);
    vsprintf_s(text, 128, fmt, args);
    __crt_va_end(args);

    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

void OpenglHook::draw_outlined_rect(int x, int y, int width, int height, int stroke, const unsigned char color[3]) {
    glLineWidth(stroke);
    glBegin(GL_LINE_STRIP);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2f(x - 0.5f, y - 0.5f);
    glVertex2f(x + width + 0.5f, y - 0.5f);
    glVertex2f(x + width + 0.5f, y + height + 0.5f);
    glVertex2f(x - 0.5f, y + height + 0.5f);
    glVertex2f(x - 0.5f, y - 0.5f);
    glEnd();
}

void OpenglHook::draw_filled_rect(int x, int y, int width, int height, const unsigned char color[3]) {
    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
