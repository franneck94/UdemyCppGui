#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "clock.hpp"

class WindowClass
{
public:
    constexpr static auto numIcons = std::uint32_t{10};

    struct Icon
    {
        Icon(std::string_view label_)
            : label(label_), position(ImVec2{}), popupOpen(false),
              clickCount(0){};
        void Draw();

        std::string label;
        ImVec2 position;
        bool popupOpen;
        std::uint32_t clickCount;
    };

public:
    WindowClass() : icons(), clock({})
    {
        icons.reserve(numIcons);
        for (std::uint32_t i = 0; i < numIcons; ++i)
        {
            icons.push_back(Icon{fmt::format("Icon{}", i)});
        }
    };

    void Draw(std::string_view label);

private:
    void DrawDesktop();
    void DrawTaskbar();

    void ShowIconList(bool *open = nullptr);

private:
    std::vector<Icon> icons;

    Clock clock;
};

void render(WindowClass &window_class);
