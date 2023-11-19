#pragma once

#include <cstdint>
#include <string_view>

class WindowClass
{
public:
    void Draw(std::string_view label);
};

void render(WindowClass &window_obj);
