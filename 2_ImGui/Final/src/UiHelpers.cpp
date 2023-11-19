#include <cstdint>
#include <exception>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>

#include "lodepng.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <imgui.h>

#include "UiHelpers.hpp"

std::tuple<GLuint, std::uint32_t, std::uint32_t> loadTexture(
    const char *filename)
{
    std::vector<unsigned char> data;
    std::uint32_t width = 0U;
    std::uint32_t height = 0U;

    const auto error = lodepng::decode(data, width, height, filename);

    if (error)
        throw "Error loading image";

    GLuint texture = 0U;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &data[0]);

    return std::make_tuple(texture, width, height);
}

void LoadAndDisplayImage(std::string_view image_filepath)
{
    try
    {
        const auto &[myImageTexture, imageWidth, imageHeight] =
            loadTexture(image_filepath.data());

        const auto imageSize = ImVec2(static_cast<float>(imageWidth),
                                      static_cast<float>(imageHeight));
        ImGui::Image(reinterpret_cast<ImTextureID>(myImageTexture), imageSize);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
}
