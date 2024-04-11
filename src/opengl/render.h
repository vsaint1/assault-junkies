#pragma once
#include "../utils/macros.h"
#include <memory>
#include <vadefs.h>

// @todo: refactor
struct Vector {
    float x, y, z;
};

const GLubyte WHITE[3] = {255, 255, 255};

class OpenglHook {

    GLuint base{0};
    HDC hdc{0};
    int m_height;
    const char *m_font;
    GLint viewport[4];

  public:
    OpenglHook(int height, const char *font_name) : m_height(height), m_font(font_name){};

    bool build_font();

    void draw_line(Vector lvalue, Vector rvalue, const unsigned char color[3]);

    void draw_text(int x, int y, const unsigned char color[3], const char *fmt, ...);

    void draw_outlined_rect(int x, int y, int width, int height, int stroke, const unsigned char color[3]);

    void draw_filled_rect(int x, int y, int width, int height, const unsigned char color[3]);

    void ortho();

    void restore();

    GLint get_viewport(int idx) { return this->viewport[idx]; }

    GLuint get_base() { return this->base; }

    HDC get_hdc() { return this->hdc; }
};

inline auto render = std::make_unique<OpenglHook>(16, "Calibri");
