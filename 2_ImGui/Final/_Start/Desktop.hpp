#pragma once

#include <string_view>
#include <vector>

#include <imgui.h>

#include <fmt/format.h>
#include <implot.h>

#include "AdvCalc.hpp"
#include "Calender.hpp"
#include "Clock.hpp"
#include "CsvEditor.hpp"
#include "Diff.hpp"
#include "FileExplorer.hpp"
#include "OtherTopics.hpp"
#include "Paint.hpp"
#include "TextEditor.hpp"
#include "WindowBase.hpp"

class Desktop : public WindowBase
{
public:
    struct Icon
    {
        Icon(std::string_view label_, WindowBase *base_)
            : label(label_), base(base_), position(ImVec2{}){};
        void Draw();

        std::string label;
        ImVec2 position;
        bool popupOpen = false;
        std::uint32_t clickCount = 0;
        WindowBase *base = nullptr;
    };

public:
    Desktop()
        : icons({}), adv_calc(), calendar(), diff_viewer(), file_explorer(),
          paint(), text_editor(), csv_editor(), clock()
    {
        icons.reserve(7);
        icons.push_back(Icon{"AdvCalc", &adv_calc});
        icons.push_back(Icon{"Calendar", &calendar});
        icons.push_back(Icon{"DiffViewer", &diff_viewer});
        icons.push_back(Icon{"FileExplorer", &file_explorer});
        icons.push_back(Icon{"Paint", &paint});
        icons.push_back(Icon{"TextEditor", &text_editor});
        icons.push_back(Icon{"CsvEditor", &csv_editor});
        icons.push_back(Icon{"OtherTopics", &other_topics});

        LoadTheme();
    };

    void Draw(std::string_view label, bool *open = nullptr) final;
    void DrawBackground();
    void DrawDesktop();
    void DrawTaskbar();


    void ShowIconList(bool *open = nullptr);

private:
    std::vector<Icon> icons;

    AdvCalc adv_calc;
    Calender calendar;
    DiffViewer diff_viewer;
    FileExplorer file_explorer;
    Paint paint;
    TextEditor text_editor;
    CsvEditor csv_editor;
    OtherTopics other_topics;

    Clock clock;
};

void render(Desktop &window_class);
