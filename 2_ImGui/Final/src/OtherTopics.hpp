#pragma once

#include <string_view>

#include "WindowBase.hpp"

class OtherTopics : public WindowBase
{
public:
    OtherTopics(){};
    ~OtherTopics(){};

    void Draw(std::string_view label, bool *open = nullptr) final;
};
