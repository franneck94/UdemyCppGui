#pragma once

#include <array>
#include <set>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "WindowBase.hpp"
class AdvCalc : public WindowBase
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
    AdvCalc() : selectedFunctions({}){};
    virtual ~AdvCalc(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawSelection();
    void DrawPlot();

    Function functionNameMapping(std::string_view function_name);
    double evaluateFunction(const Function function, const double x);

public:
    std::set<Function> selectedFunctions;
};
