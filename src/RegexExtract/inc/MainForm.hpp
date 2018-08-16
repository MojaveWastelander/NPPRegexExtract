#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

class MainForm:
    public nana::form
{
public:
    MainForm();
    ~MainForm();

private:
    nana::label m_text;
};

