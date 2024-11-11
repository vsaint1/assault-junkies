#ifndef OPENGL_HOOK_H
#define OPENGL_HOOK_H

#include "../sdk/math.h"
#include "../utils.h"
#include <gl/GL.h>

const GLubyte WHITE[3] = {255, 255, 255};

class Render {

  public:
    Render(unsigned int fontSize, const char *fontName) : fontSize(fontSize), fontName(fontName){};

    bool BuildFont();

    HDC GetHdc() { return this->hdc; }

    void Text(Vector2 pos, const unsigned char color[3], const char *fmt, ...);

    void DrawLine(Vector2 v1, Vector2 v2, const unsigned char color[3]);

    void DrawOutlineRect(Vector2 screen, float lineWidth,const unsigned char color[3]);

    void SetupOrtho();

    void Restore();

    float GetWidth() { return this->viewport[2]; }
    float GetHeight() { return this->viewport[3]; }

  private:
    unsigned int fontSize;
    const char *fontName;
    GLint viewport[4];
    HDC hdc;
    GLuint base;
};

inline auto render = std::make_unique<Render>(16, "Calibri");
#endif // OPENGL_HOOK_H
