#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawDesktop();
    DrawTaskbar();

    ImGui::End();
}

void WindowClass::DrawDesktop()
{
    for (auto &icon : icons)
        icon.Draw();
}

void WindowClass::DrawTaskbar()
{
    constexpr static auto taskbar_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoTitleBar;
    constexpr static auto taskbar_size = ImVec2(1280.0F, 40.0F);
    constexpr static auto taskbar_pos = ImVec2(0.0F, 680.0F);

    static auto open_taskbar = false;

    ImGui::SetNextWindowSize(taskbar_size);
    ImGui::SetNextWindowPos(taskbar_pos);

    ImGui::Begin("Taskbar", nullptr, taskbar_flags);

    if (ImGui::Button("All Icons", ImVec2(100, 30)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
    }

    if (open_taskbar)
        ShowIconList(&open_taskbar);

    ImGui::SameLine();

    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x);

    static auto clock_open = false;
    clock.GetTime();
    const auto time = fmt::format("{}:{}", clock.hrs, clock.mins);
    if (ImGui::Button(time.data(), ImVec2(100.0F, 30.0F)) || clock_open)
    {
        clock.Draw("clockWindow");
        clock_open = true;
    }
    if (clock_open && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        clock_open = false;

    ImGui::End();
}

void WindowClass::ShowIconList(bool *open)
{
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * numIcons + 40.0F;

    ImGui::SetNextWindowSize(ImVec2(100.0F, popup_height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F - popup_height),
                            ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs", open, ImGuiWindowFlags_NoResize))
    {
        for (auto &icon : icons)
        {
            if (ImGui::Selectable(icon.label.data()))
            {
                icon.popupOpen = true;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}

void WindowClass::Icon::Draw()
{
    constexpr static auto icon_window_flags = ImGuiWindowFlags_NoResize |
                                              ImGuiWindowFlags_NoCollapse |
                                              ImGuiWindowFlags_NoScrollbar;
    constexpr static auto button_size = ImVec2(100.0F, 50.0F);

    const auto label_icon_window = fmt::format("IconWindow##{}", label);
    const auto label_icon_popup = fmt::format("IconPopup##{}", label);

    ImGui::SetNextWindowSize(
        ImVec2(button_size.x + 35.0F, button_size.y + 35.0F),
        ImGuiCond_Always);
    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);

    ImGui::Begin(label_icon_window.data(), nullptr, icon_window_flags);

    if (ImGui::Button(label.data(), button_size))
    {
        ++clickCount;
    }

    if (clickCount >= 1 || popupOpen)
    {
        ImGui::OpenPopup(label_icon_popup.data());
        clickCount = 0;
        popupOpen = true;
    }

    if (ImGui::BeginPopupModal(label_icon_popup.data(), &popupOpen))
    {
        ImGui::Text("Hi");

        if (ImGui::Button("Close"))
        {
            popupOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Desktop");
}
