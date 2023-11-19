#pragma once

#include <filesystem>
#include <string_view>

#include <imgui.h>

#include "WindowBase.hpp"

namespace fs = std::filesystem;

class FileExplorer : public WindowBase
{
public:
    FileExplorer()
        : currentPath(fs::current_path()), selectedEntry(fs::path{}){};
    virtual ~FileExplorer(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawMenu();
    void DrawContent();
    void DrawActions();
    void DrawFilter();

    void openFileWithDefaultEditor();
    void renameFilePopup();
    void deleteFilePopup();
    bool renameFile(const fs::path &old_path, const fs::path &new_path);
    bool deleteFile(const fs::path &path);

private:
    fs::path currentPath;
    fs::path selectedEntry;

    bool renameDialogOpen = false;
    bool deleteDialogOpen = false;
};
