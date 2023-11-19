#pragma once

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

#include "WindowBase.hpp"

namespace fs = std::filesystem;

class TextEditor : public WindowBase
{
public:
    static constexpr auto bufferSize = std::size_t{1024};
    static constexpr auto popUpFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popUpSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popUpButtonSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popUpPos = ImVec2(1280.0F / 2.0F - popUpSize.x / 2.0F,
                                            720.0F / 2.0F - popUpSize.y / 2.0F);

public:
    TextEditor() : currentFilename({})
    {
        std::memset(textBuffer, 0, bufferSize);
    }
    virtual ~TextEditor(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawMenu();
    void DrawSavePopup();
    void DrawLoadPopup();
    void DrawContent();
    void DrawInfo();

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    char textBuffer[bufferSize];
    std::string currentFilename;
};
