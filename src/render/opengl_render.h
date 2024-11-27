#ifndef OPENGL_RENDER_H
#define OPENGL_RENDER_H

#include "../sdk/math.h"
#include "../utils.h"
#include <gl/GL.h>

const GLubyte WHITE[3] = {255, 255, 255};
const GLubyte RED[3] = {255, 0, 0};
const GLubyte BLUE[3] = {0, 0, 255};
const GLubyte GREEN[3] = {0, 255, 0};

class Render {

  public:
    Render(unsigned int fontSize, const char *fontName) : fontSize(fontSize), fontName(fontName) {};

    bool BuildFont();

    HDC GetHdc() { return this->hdc; }

    void Text(Vector2 pos, const unsigned char color[3], const char *fmt, ...);

    void DrawLine(Vector2 v1, Vector2 v2, const unsigned char color[3] = WHITE, float lineWidth = 1.0f);

    void DrawRect(Vector2 screenPlayerPos, Vector2 screenHeadPos, const unsigned char color[3] = WHITE,float lineWidth = 1.0f);

    void DrawCircle(Vector2 pos, float radius, const unsigned char color[3] = WHITE,int segments = 360, float lineWidth = 1.0f);

    void SetupOrtho();

    void Restore();

    float GetWidth() const { return this->viewport[2]; }

    float GetHeight() const { return this->viewport[3]; }

  private:
    unsigned int fontSize;
    const char *fontName;
    GLint viewport[4];
    HDC hdc;
    GLuint base;
};

inline auto render = std::make_unique<Render>(10, "Calibri");

#endif // OPENGL_RENDER_H
