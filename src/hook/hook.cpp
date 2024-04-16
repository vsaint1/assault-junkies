#include "hook.h"

typedef BOOL(__stdcall *WglSwapBuffers)(HDC hdc);

WglSwapBuffers o_swap_buffers = nullptr;

BOOL hk_swap_buffers(HDC hdc) {

  if (render->get_hdc() != hdc)
    render->build_font();

  render->ortho();

  render->draw_text(200, 250, WHITE, "%s %d, threads: %d", "abcdefghijk", 1234, _Thrd_hardware_concurrency());

  render->restore();

  return o_swap_buffers(hdc);
}

bool hook::setup() {

  MH_Initialize();

  if (MH_CreateHookApi(L"OpenGL32.dll", "wglSwapBuffers", &hk_swap_buffers, reinterpret_cast<void **>(&o_swap_buffers)) != MH_OK)
    return false;

  if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    return false;

  spdlog::info("Hooks done");

  return true;
}

bool hook::shutdown() {
  MH_DisableHook(MH_ALL_HOOKS);

  MH_Uninitialize();

  return true;
}
