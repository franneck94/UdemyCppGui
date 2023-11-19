#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <imgui.h>

#include "WindowBase.hpp"

class DiffViewer : public WindowBase
{
public:
    using FileContent = std::vector<std::string>;

public:
    DiffViewer()
        : filePath1("text1.txt"), filePath2("text2.txt"), fileContent1({}),
          fileContent2({}), diffResult1({}), diffResult2({}){};
    virtual ~DiffViewer(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawSelection();
    void DrawDiffView();
    void DrawStats();

    FileContent LoadFileContent(std::string_view file_path);
    void SaveFileContent(std::string_view file_path, FileContent &file_content);
    void CreateDiff();

private:
    std::string filePath1;
    std::string filePath2;

    FileContent fileContent1;
    FileContent fileContent2;

    FileContent diffResult1;
    FileContent diffResult2;
};
