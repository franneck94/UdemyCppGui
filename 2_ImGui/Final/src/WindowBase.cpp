#include <filesystem>
#include <fstream>
#include <iostream>

#include <SimpleIni.h>
#include <imgui.h>
#include <implot.h>

#include "WindowBase.hpp"

namespace
{
ImU32 LoadColor(CSimpleIniA *ini,
                const char *section,
                const char *key,
                ImU32 def)
{
    const char *wc = ini->GetValue(section, key, nullptr);
    if (!wc)
        return def;

    auto value = std::strtoul(wc, nullptr, 16);
    return static_cast<ImU32>(value);
}

void SaveColor(CSimpleIniA *ini,
               const char *section,
               const char *key,
               ImU32 val)
{
    char buf[2 * 4 + 1] = {'\0'};
    snprintf(buf, sizeof(buf), "%x", val);
    ini->SetValue(section, key, buf);
}
}; // namespace

void WindowBase::SettingsMenuBar()
{
    static auto theme_menu_open = false;
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::MenuItem("Theme", nullptr, &theme_menu_open);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (theme_menu_open)
        DrawColorsSettings(&theme_menu_open);
}

void WindowBase::LoadTheme()
{
#ifdef _DEBUG
    std::cout << "Loading theme!\n";
#endif

    const char ini_filepath[] = "test.ini";
    const char section[] = "theme";

    if (!std::filesystem::exists(ini_filepath))
    {
        ImGui::GetStyle() = DefaultColorStyle();

        auto out = std::ofstream{ini_filepath};
        out.close();

        return;
    }

    ini.LoadFile(ini_filepath);

    auto &style = ImGui::GetStyle();

    style.WindowPadding.x = (float)ini.GetDoubleValue(section,
                                                      "WindowPaddingX",
                                                      style.WindowPadding.x);
    style.WindowPadding.y = (float)ini.GetDoubleValue(section,
                                                      "WindowPaddingY",
                                                      style.WindowPadding.y);
    style.WindowRounding = (float)ini.GetDoubleValue(section,
                                                     "WindowRounding",
                                                     style.WindowRounding);
    style.FramePadding.x = (float)ini.GetDoubleValue(section,
                                                     "FramePaddingX",
                                                     style.FramePadding.x);
    style.FramePadding.y = (float)ini.GetDoubleValue(section,
                                                     "FramePaddingY",
                                                     style.FramePadding.y);
    style.FrameRounding = (float)ini.GetDoubleValue(section,
                                                    "FrameRounding",
                                                    style.FrameRounding);
    style.ItemSpacing.x =
        (float)ini.GetDoubleValue(section, "ItemSpacingX", style.ItemSpacing.x);
    style.ItemSpacing.y =
        (float)ini.GetDoubleValue(section, "ItemSpacingY", style.ItemSpacing.y);
    style.ItemInnerSpacing.x =
        (float)ini.GetDoubleValue(section,
                                  "ItemInnerSpacingX",
                                  style.ItemInnerSpacing.x);
    style.ItemInnerSpacing.y =
        (float)ini.GetDoubleValue(section,
                                  "ItemInnerSpacingY",
                                  style.ItemInnerSpacing.y);
    style.IndentSpacing = (float)ini.GetDoubleValue(section,
                                                    "IndentSpacing",
                                                    style.IndentSpacing);
    style.ScrollbarSize = (float)ini.GetDoubleValue(section,
                                                    "ScrollbarSize",
                                                    style.ScrollbarSize);
    style.ScrollbarRounding =
        (float)ini.GetDoubleValue(section,
                                  "ScrollbarRounding",
                                  style.ScrollbarRounding);
    style.GrabMinSize =
        (float)ini.GetDoubleValue(section, "GrabMinSize", style.GrabMinSize);
    style.GrabRounding =
        (float)ini.GetDoubleValue(section, "GrabRounding", style.GrabRounding);
    style.WindowTitleAlign.x =
        (float)ini.GetDoubleValue(section,
                                  "WindowTitleAlignX",
                                  style.WindowTitleAlign.x);
    style.WindowTitleAlign.y =
        (float)ini.GetDoubleValue(section,
                                  "WindowTitleAlignY",
                                  style.WindowTitleAlign.y);
    style.ButtonTextAlign.x =
        (float)ini.GetDoubleValue(section,
                                  "ButtonTextAlignX",
                                  style.ButtonTextAlign.x);
    style.ButtonTextAlign.y =
        (float)ini.GetDoubleValue(section,
                                  "ButtonTextAlignY",
                                  style.ButtonTextAlign.y);

    for (int i = 0; i < ImGuiCol_COUNT; ++i)
    {
        const char *name = ImGui::GetStyleColorName(i);
        const auto default_color =
            ImGui::ColorConvertFloat4ToU32(style.Colors[i]);
        const auto color_u32 = LoadColor(&ini, section, name, default_color);
        style.Colors[i] = ImGui::ColorConvertU32ToFloat4(color_u32);
    }
}

void WindowBase::SaveTheme()
{
#ifdef _DEBUG
    std::cout << "Saving theme!\n";
#endif

    const char ini_filepath[] = "test.ini";
    const char section[] = "theme";

    if (!ImGui::GetCurrentContext())
        return;

    SI_Error rc = ini.LoadFile(ini_filepath);
    if (rc < 0)
        return;

    auto &style = ImGui::GetStyle();

    ini.SetDoubleValue(section, "GlobalAlpha", style.Alpha);
    ini.SetDoubleValue(section, "WindowPaddingX", style.WindowPadding.x);
    ini.SetDoubleValue(section, "WindowPaddingY", style.WindowPadding.y);
    ini.SetDoubleValue(section, "WindowRounding", style.WindowRounding);
    ini.SetDoubleValue(section, "FramePaddingX", style.FramePadding.x);
    ini.SetDoubleValue(section, "FramePaddingY", style.FramePadding.y);
    ini.SetDoubleValue(section, "FrameRounding", style.FrameRounding);
    ini.SetDoubleValue(section, "ItemSpacingX", style.ItemSpacing.x);
    ini.SetDoubleValue(section, "ItemSpacingY", style.ItemSpacing.y);
    ini.SetDoubleValue(section, "ItemInnerSpacingX", style.ItemInnerSpacing.x);
    ini.SetDoubleValue(section, "ItemInnerSpacingY", style.ItemInnerSpacing.y);
    ini.SetDoubleValue(section, "IndentSpacing", style.IndentSpacing);
    ini.SetDoubleValue(section, "ScrollbarSize", style.ScrollbarSize);
    ini.SetDoubleValue(section, "ScrollbarRounding", style.ScrollbarRounding);
    ini.SetDoubleValue(section, "GrabMinSize", style.GrabMinSize);
    ini.SetDoubleValue(section, "GrabRounding", style.GrabRounding);
    ini.SetDoubleValue(section, "WindowTitleAlignX", style.WindowTitleAlign.x);
    ini.SetDoubleValue(section, "WindowTitleAlignY", style.WindowTitleAlign.y);
    ini.SetDoubleValue(section, "ButtonTextAlignX", style.ButtonTextAlign.x);
    ini.SetDoubleValue(section, "ButtonTextAlignY", style.ButtonTextAlign.y);

    for (int i = 0; i < ImGuiCol_COUNT; ++i)
    {
        const char *name = ImGui::GetStyleColorName(i);
        const auto color_f4 = style.Colors[i];
        const auto color_u32 = ImGui::ColorConvertFloat4ToU32(color_f4);
        SaveColor(&ini, section, name, color_u32);
    }

    rc = ini.SaveFile(ini_filepath);
    if (rc < 0)
        return;
}


void WindowBase::DrawColorsSettings(bool *open)
{
    ImGui::SetNextWindowPos(mainWindowPos);
    ImGui::SetNextWindowSize(mainWindowSize);

    ImGui::Begin("Settings", open, mainWindowFlags);

    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::SliderFloat("Global Alpha", &style.Alpha, 0.2f, 1.0f, "%.2f");
    style.Alpha = std::clamp(style.Alpha, 0.2f, 1.f);
    ImGui::SliderFloat("Global Font Scale",
                       &ImGui::GetIO().FontGlobalScale,
                       0.05f,
                       3.00f,
                       "%.1f");
    ImGui::Text("Sizes");
    ImGui::SliderFloat2("Window Padding",
                        (float *)&style.WindowPadding,
                        0.0f,
                        20.0f,
                        "%.0f");
    ImGui::SliderFloat("Window Rounding",
                       &style.WindowRounding,
                       0.0f,
                       16.0f,
                       "%.0f");
    ImGui::SliderFloat2("Frame Padding",
                        (float *)&style.FramePadding,
                        0.0f,
                        20.0f,
                        "%.0f");
    ImGui::SliderFloat("Frame Rounding",
                       &style.FrameRounding,
                       0.0f,
                       16.0f,
                       "%.0f");
    ImGui::SliderFloat2("Item Spacing",
                        (float *)&style.ItemSpacing,
                        0.0f,
                        20.0f,
                        "%.0f");
    ImGui::SliderFloat2("Item InnerSpacing",
                        (float *)&style.ItemInnerSpacing,
                        0.0f,
                        20.0f,
                        "%.0f");
    ImGui::SliderFloat("Indent Spacing",
                       &style.IndentSpacing,
                       0.0f,
                       30.0f,
                       "%.0f");
    ImGui::SliderFloat("Scrollbar Size",
                       &style.ScrollbarSize,
                       1.0f,
                       20.0f,
                       "%.0f");
    ImGui::SliderFloat("Scrollbar Rounding",
                       &style.ScrollbarRounding,
                       0.0f,
                       16.0f,
                       "%.0f");
    ImGui::SliderFloat("Grab MinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
    ImGui::SliderFloat("Grab Rounding",
                       &style.GrabRounding,
                       0.0f,
                       16.0f,
                       "%.0f");
    ImGui::Text("Alignment");
    ImGui::SliderFloat2("Window Title Align",
                        (float *)&style.WindowTitleAlign,
                        0.0f,
                        1.0f,
                        "%.2f");
    ImGui::SliderFloat2("Button Text Align",
                        (float *)&style.ButtonTextAlign,
                        0.0f,
                        1.0f,
                        "%.2f");
    ImGui::Text("Colors");
    for (int i = 0; i < ImGuiCol_COUNT; ++i)
    {
        const char *name = ImGui::GetStyleColorName(i);
        ImGui::PushID(i);
        ImGui::ColorEdit4(name, reinterpret_cast<float *>(&style.Colors[i]));
        ImGui::PopID();
    }

    ImGui::End();
}

ImGuiStyle WindowBase::DefaultColorStyle()
{
    ImGuiStyle style = ImGuiStyle();
    ImGui::StyleColorsDark(&style);

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.60f;
    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32, 32);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4, 3);
    style.FrameRounding = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.CellPadding = ImVec2(4, 2);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 0.0f;
    style.LogSliderDeadzone = 4.0f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
    style.SeparatorTextBorderSize = 3.0f;
    style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);
    style.SeparatorTextPadding = ImVec2(20.0f, 3.f);
    style.DisplayWindowPadding = ImVec2(19, 19);
    style.DisplaySafeAreaPadding = ImVec2(3, 3);
    style.MouseCursorScale = 1.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedLinesUseTex = true;
    style.AntiAliasedFill = true;
    style.CurveTessellationTol = 1.25f;
    style.CircleTessellationMaxError = 0.30f;

    style.Colors[ImGuiCol_Text] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.0F);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0F, 1.0F, 1.0F, 0.0F);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0F, 0.0F, 0.0F, 0.0F);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0F);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.0F);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0F, 0.0F, 0.0F, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0F);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0F);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] =
        ImVec4(0.41f, 0.41f, 0.41f, 1.0F);
    style.Colors[ImGuiCol_ScrollbarGrabActive] =
        ImVec4(0.51f, 0.51f, 0.51f, 1.0F);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.0F);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered] =
        ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.0F);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0F);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0F, 0.43f, 0.35f, 1.0F);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.0F, 1.0F);
    style.Colors[ImGuiCol_PlotHistogramHovered] =
        ImVec4(1.0F, 0.60f, 0.0F, 1.0F);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0F, 1.0F, 0.0F, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.0F);
    style.Colors[ImGuiCol_NavWindowingHighlight] =
        ImVec4(1.0F, 1.0F, 1.0F, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] =
        ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] =
        ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    return style;
}
