#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <imgui.h>

#include "WindowBase.hpp"

class CsvEditor : public WindowBase
{
public:
    static constexpr auto popUpFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popUpSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popUpPos = ImVec2(1280.0F / 2.0F - popUpSize.x / 2.0F,
                                            720.0F / 2.0F - popUpSize.y / 2.0F);
    static constexpr auto maxNumRows = 30;
    static constexpr auto maxNumCols = 8;

public:
    CsvEditor()
        : numCols(0), numRows(0), data({}), filenameBuffer("test.csv"){};
    virtual ~CsvEditor(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawSizeButtons();
    void DrawIoButtons();
    void DrawTable();

    void DrawSavePopup();
    void DrawLoadPopup();
    void DrawValuePopup(const int row, const int col);

    void SaveToCsvFile(std::string_view filename);
    void LoadFromCsvFile(std::string_view filename);

    template <typename T>
    void PlotCellValue(std::string_view formatter, const T value);
    void SetPopupLayout();

private:
    std::int32_t numCols;
    std::int32_t numRows;
    std::vector<std::vector<float>> data;
    char filenameBuffer[256];
};
