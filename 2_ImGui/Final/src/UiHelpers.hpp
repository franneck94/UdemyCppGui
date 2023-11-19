#pragma once

#include <cstdint>
#include <string_view>
#include <tuple>

#include <GLFW/glfw3.h>

std::tuple<GLuint, std::uint32_t, std::uint32_t> loadTexture(
    const char *filename);

void LoadAndDisplayImage(std::string_view image_filepath);
