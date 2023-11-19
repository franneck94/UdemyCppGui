#pragma once

#include <cstdint>
#include <string_view>
#include <tuple>
#include <vector>

#include <imgui.h>

#include "WindowBase.hpp"

class Paint : public WindowBase
{
public:
    using PointData = std::tuple<ImVec2, ImColor, float>;
    static constexpr auto popUpFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popUpSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popUpPos = ImVec2(1280.0F / 2.0F - popUpSize.x / 2.0F,
                                            720.0F / 2.0F - popUpSize.y / 2.0F);

public:
    Paint()
        : points({}), canvasPos({}), currentDrawColor(ImColor(255, 255, 255)),
          pointDrawSize(2.0F), filenameBuffer("test.bin"){};
    virtual ~Paint(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawMenu();
    void DrawCanvas();
    void DrawColorButtons();
    void DrawSizeSettings();

    void DrawSavePopup();
    void DrawLoadPopup();

    void SaveToImageFile(std::string_view filename);
    void LoadFromImageFile(std::string_view filename);
    void ClearCanvas();

private:
    std::uint32_t numRows = 800;
    std::uint32_t numCols = 600;
    std::uint32_t numChannels = 3;
    ImVec2 canvasSize =
        ImVec2(static_cast<float>(numRows), static_cast<float>(numCols));

    std::vector<PointData> points;
    ImVec2 canvasPos;

    ImColor currentDrawColor;
    float pointDrawSize;

    char filenameBuffer[256];
};
