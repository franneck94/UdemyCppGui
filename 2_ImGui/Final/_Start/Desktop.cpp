#include <exception>
#include <iostream>
#include <string_view>
#include <tuple>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "Desktop.hpp"
#include "UiHelpers.hpp"

void Desktop::Draw(std::string_view label, bool *)
{
    ImGui::SetNextWindowSize(mainWindowSize);
    ImGui::SetNextWindowPos(mainWindowPos);

    ImGui::Begin(label.data(),
                 nullptr,
                 (mainWindowFlags | ImGuiWindowFlags_NoInputs |
                  ImGuiWindowFlags_NoTitleBar));

    DrawBackground();
    DrawDesktop();
    DrawTaskbar();

    ImGui::End();
}

void Desktop::DrawBackground()
{
    ImGui::SetCursorPos(ImVec2(0.0F, 0.0F));
    const auto image_filepath =
        fmt::format("{}{}", PROJECT_PATH, "/images/bg.png");
    LoadAndDisplayImage(image_filepath);
    ImGui::SetCursorPos(ImVec2(0.0F, 0.0F));
}

void Desktop::DrawDesktop()
{
    for (auto &icon : icons)
        icon.Draw();
}

void Desktop::DrawTaskbar()
{
    constexpr static auto button_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    static auto open_taskbar = false;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 40.0F), ImGuiCond_Always);

    ImGui::Begin("Taskbar", NULL, button_flags);

    ImGui::SetCursorPosX(0.0F);
    if (ImGui::Button("All Icons", ImVec2(100.0F, 30.0F)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
    }

    if (open_taskbar)
        ShowIconList(&open_taskbar);

    ImGui::SameLine();

    static auto theme_open = false;
    if (ImGui::Button("Theme", ImVec2(100.0F, 30.0F)) || theme_open)
    {
        theme_open = true;
        DrawColorsSettings(&theme_open);
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(mainWindowSize.x - 100.0F);

    static auto clock_open = false;
    clock.GetTime();
    const auto time = fmt::format("{}:{}", clock.hrs, clock.mins);
    if (ImGui::Button(time.data(), ImVec2(100.0F, 30.0F)) || clock_open)
    {
        clock.Draw("clockWindow");
        clock_open = true;
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        clock_open = false;

    ImGui::End();
}

void Desktop::ShowIconList(bool *open)
{
    const auto icon_count = static_cast<float>(icons.size());
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * icon_count + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F - popup_height),
                            ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(100.0F, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs", open, ImGuiWindowFlags_NoResize))
    {
        for (auto &icon : icons)
        {
            if (ImGui::Selectable(icon.label.data()))
            {
                icon.popupOpen = true;
                icon.base->Draw(icon.label, &icon.popupOpen);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}

void Desktop::Icon::Draw()
{
    constexpr static auto flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::Begin(fmt::format("###{}", label).data(), nullptr, flags);

    if (ImGui::Button(label.data(), ImVec2(100.0F, 50.0F)) || popupOpen)
    {
        popupOpen = true;
        base->Draw(label, &popupOpen);
    }

    ImGui::End();
}

void render(Desktop &window_class)
{
    window_class.Draw("Desktop");
}
