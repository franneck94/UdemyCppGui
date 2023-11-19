#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto main_window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto main_window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto main_window_pos = ImVec2(0.0F, 0.0F);
    constexpr static auto upper_section_height = 50.0F;
    constexpr static auto lower_section_height = 100.0F;
    constexpr static auto content_height =
        main_window_size.y - lower_section_height - upper_section_height;

    ImGui::SetNextWindowSize(main_window_size);
    ImGui::SetNextWindowPos(main_window_pos);

    ImGui::Begin(label.data(), nullptr, main_window_flags);

    DrawMenu();
    ImGui::Separator();

    ImGui::SetNextWindowSize(ImVec2(main_window_size.x, content_height));
    ImGui::SetNextWindowPos(ImVec2(main_window_pos.x, upper_section_height));
    ImGui::Begin(fmt::format("##innerContent{}", currentPath.string()).c_str(),
                 nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    DrawContent();
    ImGui::End();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height);
    ImGui::Separator();
    DrawActions();
    ImGui::Separator();
    DrawFilter();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    if (ImGui::Button("Go Up"))
    {
        if (currentPath.has_parent_path())
        {
            currentPath = currentPath.parent_path();
        }
    }

    ImGui::SameLine();

    ImGui::Text("Current directory: %s", currentPath.string().c_str());
}

void WindowClass::DrawContent()
{
    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        const auto is_selected = entry.path() == selectedEntry;
        const auto is_directory = entry.is_directory();
        const auto is_file = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_directory)
            entry_name = "[D]" + entry_name;
        else if (is_file)
            entry_name = "[F]" + entry_name;

        if (ImGui::Selectable(entry_name.c_str(), is_selected))
        {
            if (is_directory)
                currentPath /= entry.path().filename();

            selectedEntry = entry.path();
        }
    }
}

void WindowClass::DrawActions()
{
    if (fs::is_directory(selectedEntry))
        ImGui::Text("Selected dir: %s", selectedEntry.string().c_str());
    else if (fs::is_regular_file(selectedEntry))
        ImGui::Text("Selected file: %s", selectedEntry.string().c_str());
    else
    {
        ImGui::Text("Selected file: n/a");
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                            ImGui::GetStyle().Alpha * 0.0f);
        ImGui::Button("Non-clickable button");
        ImGui::PopStyleVar();
        return;
    }

    if (fs::is_regular_file(selectedEntry) && ImGui::Button("Open"))
        openFileWithDefaultEditor();

    ImGui::SameLine();

    if (ImGui::Button("Rename"))
    {
        renameDialogOpen = true;
        ImGui::OpenPopup("Rename File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        deleteDialogOpen = true;
        ImGui::OpenPopup("Delete File");
    }

    renameFilePopup();
    deleteFilePopup();
}

void WindowClass::DrawFilter()
{
    static char extension_filter[16] = {"\0"};

    ImGui::Text("Filter by extension");
    ImGui::SameLine();
    ImGui::InputText("###inFilter", extension_filter, sizeof(extension_filter));

    if (std::strlen(extension_filter) == 0)
        return;

    auto filtered_file_count = std::size_t{0};
    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        if (!fs::is_regular_file(entry))
            continue;

        if (entry.path().extension().string() == extension_filter)
            ++filtered_file_count;
    }

    ImGui::Text("Number of files: %u", filtered_file_count);
}

void WindowClass::openFileWithDefaultEditor()
{
#ifdef _WIN32
    const auto command = "start \"\" \"" + selectedEntry.string() + "\"";
#elif __APPLE__
    const auto command = "open \"" + selectedEntry.string() + "\"";
#else
    const auto command = "xdg-open \"" + selectedEntry.string() + "\"";
#endif

    std::system(command.c_str());
}

void WindowClass::renameFilePopup()
{
    if (ImGui::BeginPopupModal("Rename File", &renameDialogOpen))
    {
        static char buffer_name[512] = {'\0'};

        ImGui::Text("New name: ");
        ImGui::InputText("###newName", buffer_name, sizeof(buffer_name));

        if (ImGui::Button("Rename"))
        {
            auto new_path = selectedEntry.parent_path() / buffer_name;
            if (renameFile(selectedEntry, new_path))
            {
                renameDialogOpen = false;
                selectedEntry = new_path;
                std::memset(buffer_name, 0, sizeof(buffer_name));
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            renameDialogOpen = false;

        ImGui::EndPopup();
    }
}

void WindowClass::deleteFilePopup()
{
    if (ImGui::BeginPopupModal("Delete File", &deleteDialogOpen))
    {
        ImGui::Text("Are you sure you want to delete %s?",
                    selectedEntry.filename().string().c_str());

        if (ImGui::Button("Yes"))
        {
            if (deleteFile(selectedEntry))
                selectedEntry.clear();
            deleteDialogOpen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("No"))
            deleteDialogOpen = false;

        ImGui::EndPopup();
    }
}

bool WindowClass::renameFile(const fs::path &old_path, const fs::path &new_path)
{
    try
    {
        fs::rename(old_path, new_path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool WindowClass::deleteFile(const fs::path &path)
{
    try
    {
        fs::remove(path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("File Explorer Tool");
}
