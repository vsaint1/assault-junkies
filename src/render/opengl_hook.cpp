#include "opengl_hook.h"

bool Render::BuildFont() {
    const auto RANGE = 96;
    this->hdc = wglGetCurrentDC();
    this->base = glGenLists(RANGE);

    auto hFont = CreateFont(-fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);
    auto oldFont = (HFONT)SelectObject(hdc, hFont);
    wglUseFontBitmapsW(hdc, RANGE / 3, RANGE, base);
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    return true;
}

void Render::Text(Vector2 pos, const unsigned char color[3], const char *fmt, ...) {
    glColor3ub(color[0], color[1], color[2]);

    glRasterPos2i(pos.x, pos.y);
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

void Render::DrawLine(Vector2 v1, Vector2 v2, const unsigned char color[3]) {
    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_LINES);
    glVertex2f(v1.x, v2.y);
    glVertex2f(v1.x, v2.y);
    glEnd();
}

void Render::DrawOutlineRect(Vector2 screen, float lineWidth, const unsigned char color[3]) {

    glLineWidth(lineWidth);
    glBegin(GL_LINE_STRIP);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2f(screen.x - 0.5f, screen.y - 0.5f);
    glVertex2f(screen.x + this->GetWidth() + 0.5f, screen.y - 0.5f);
    glVertex2f(screen.x + this->GetWidth() + 0.5f, screen.y + this->GetHeight() + 0.5f);
    glVertex2f(screen.x - 0.5f, screen.y + this->GetHeight() + 0.5f);
    glVertex2f(screen.x - 0.5f, screen.y + this->GetHeight() - 0.5f);
    glEnd();
}

void Render::SetupOrtho() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
}

void Render::Restore() {
    glPopMatrix();
    glPopAttrib();
}