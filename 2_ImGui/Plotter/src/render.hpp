#pragma once

#include <array>
#include <set>
#include <string_view>

#include <imgui.h>

class WindowClass
{
public:
    constexpr static auto functionNames =
        std::array<std::string_view, 3>{"unk", "sin(x)", "cos(x)"};

    enum class Function
    {
        NONE,
        SIN,
        COS,
    };

public:
    WindowClass() : selectedFunctions({}){};

    void Draw(std::string_view label);

private:
    void DrawSelection();
    void DrawPlot();

    Function functionNameMapping(std::string_view function_name);
    double evaluateFunction(const Function function, const double x);

public:
    std::set<Function> selectedFunctions;
};

void render(WindowClass &window_obj);
