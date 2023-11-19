#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "TextEditor.hpp"

namespace fs = std::filesystem;

void TextEditor::Draw(std::string_view label, bool *open)
{
    ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);

    ImGui::Begin(label.data(),
                 open,
                 mainWindowFlags | ImGuiWindowFlags_MenuBar);

    SettingsMenuBar();
    DrawMenu();
    DrawContent();
    DrawInfo();

    ImGui::End();
}

void TextEditor::DrawMenu()
{
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto s_pressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    const auto l_pressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    if (ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    DrawSavePopup();
    DrawLoadPopup();
}

void TextEditor::DrawSavePopup()
{
    static char saveFilenameBuffer[256] = "text.txt";
    const auto esc_pressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(popUpPos);
    if (ImGui::BeginPopupModal("Save File", nullptr, popUpFlags))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", popUpButtonSize))
        {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditor::DrawLoadPopup()
{
    static char loadFilenameBuffer[256] = "text.txt";
    const auto esc_pressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(popUpPos);
    if (ImGui::BeginPopupModal("Load File", nullptr, popUpFlags))
    {
        ImGui::InputText("Filename",
                         loadFilenameBuffer,
                         sizeof(loadFilenameBuffer));

        if (ImGui::Button("Load", popUpButtonSize))
        {
            LoadFromFile(loadFilenameBuffer);
            currentFilename = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popUpButtonSize) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditor::DrawContent()
{
    static constexpr auto inputTextSize = ImVec2(1200.0F, 625.0F);
    static constexpr auto lineNumberSize = ImVec2(30.0F, inputTextSize.y);
    static constexpr auto inputTextFlags =
        ImGuiInputTextFlags_AllowTabInput |
        ImGuiInputTextFlags_NoHorizontalScroll;

    ImGui::BeginChild("LineNumbers", lineNumberSize);

    const auto line_count =
        std::count(textBuffer, textBuffer + bufferSize, '\n') + 1;

    for (auto i = 1; i <= line_count; ++i)
        ImGui::Text("%d", i);

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::InputTextMultiline("###inputField",
                              textBuffer,
                              bufferSize,
                              inputTextSize,
                              inputTextFlags);
}

void TextEditor::DrawInfo()
{
    if (currentFilename.size() == 0)
    {
        ImGui::Text("No File Opened!");
        return;
    }

    const auto file_extension = GetFileExtension(currentFilename);
    ImGui::Text("Opened file %s | File extension %s",
                currentFilename.data(),
                file_extension.data());
}

void TextEditor::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream{filename.data()};

    if (out.is_open())
    {
        out << textBuffer;
        out.close();
    }
}

void TextEditor::LoadFromFile(std::string_view filename)
{
    auto in = std::ifstream{filename.data()};

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        std::memcpy(textBuffer, buffer.str().data(), bufferSize);
        in.close();
    }
}

std::string TextEditor::GetFileExtension(std::string_view filename)
{
    const auto file_path = fs::path{filename};

    return file_path.extension().string();
}
