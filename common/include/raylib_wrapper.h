#pragma once

/**
 * \brief raylib is a namespace that wraps the raylib headers include because
 * some functions have a name conflict with functions from the windows headers
 * included in the photon library.
 * It also recreates the raylib color macros to constexpr variables.
 * This wrapper is only needed when a file have to include both photon and
 * raylib stuffs.
 */
namespace raylib {

#include <raylib.h>

void DrawRaylibText(const char* text, int posX, int posY, int fontSize,
                    Color color);

}  // namespace raylib