#include <exception>
#include <iostream>
#include <string_view>
#include <tuple>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "OtherTopics.hpp"
#include "UiHelpers.hpp"

void OtherTopics::Draw(std::string_view label, bool *open)
{
    ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);

    ImGui::Begin(label.data(), open, mainWindowFlags);

    if (ImGui::TreeNode("Tabbing"))
    {
        static char buf[32] = "hello";
        ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
        ImGui::TreePop();
    }

    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("1");
        ImGui::Text("2");
        ImGui::Text("3");
    }

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Sizes1"))
        {
            ImGui::SeparatorText("Main11");
            ImGui::Text("1");
            ImGui::SeparatorText("Main21");
            ImGui::Text("2");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sizes2"))
        {
            ImGui::SeparatorText("Main12");
            ImGui::Text("1");
            ImGui::SeparatorText("Main22");
            ImGui::Text("2");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
