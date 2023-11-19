#include <array>
#include <cmath>
#include <iostream>
#include <set>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawSelection();
    DrawPlot();

    ImGui::End();
}

void WindowClass::DrawSelection()
{
    for (const auto func_name : functionNames)
    {
        const auto curr_function = functionNameMapping(func_name);
        auto selected = selectedFunctions.contains(curr_function);

        if (ImGui::Checkbox(func_name.data(), &selected))
        {
            if (selected)
                selectedFunctions.insert(curr_function);
            else
                selectedFunctions.erase(curr_function);
        }
    }
}

void WindowClass::DrawPlot()
{
    static constexpr auto num_points = 10'000;
    static constexpr auto x_min = -100.0;
    static constexpr auto x_max = 100.0;
    static const auto x_step = (std::abs(x_max) + std::abs(x_min)) / num_points;

    static auto xs = std::array<double, num_points>{};
    static auto ys = std::array<double, num_points>{};

    if (selectedFunctions.size() == 0 ||
        (selectedFunctions.size() == 1 &&
         *selectedFunctions.begin() == Function::NONE))
    {
        ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        return;
    }

    ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);

    for (const auto &function : selectedFunctions)
    {
        auto x = x_min;
        for (std::size_t i = 0; i < num_points; ++i)
        {
            xs[i] = x;
            ys[i] = evaluateFunction(function, x);
            x += x_step;
        }

        const auto plot_label =
            fmt::format("##function{}", static_cast<int>(function));
        ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_points);
    }

    ImPlot::EndPlot();
}

WindowClass::Function WindowClass::functionNameMapping(
    std::string_view function_name)
{
    if (std::string_view{"sin(x)"} == function_name)
        return WindowClass::Function::SIN;

    if (std::string_view{"cos(x)"} == function_name)
        return WindowClass::Function::COS;

    return WindowClass::Function::NONE;
}

double WindowClass::evaluateFunction(const Function function, const double x)
{
    switch (function)
    {
    case Function::SIN:
    {
        return std::sin(x);
    }
    case Function::COS:
    {
        return std::cos(x);
    }
    case Function::NONE:
    default:
    {
        return 0.0;
    }
    }
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Calculator Tool");
}
