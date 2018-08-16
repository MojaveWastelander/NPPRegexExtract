#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

class MainForm:
    public nana::form
{
public:
    MainForm();
    ~MainForm();
    void events_processing();

private:
    nana::label   m_find_lbl{*this, "Find"};
    nana::button  m_find_btn{*this, "&Search"};
    nana::textbox m_find_edit{*this};
    nana::label   m_replace_lbl{*this, "Replace"};
    nana::button  m_replace_btn{*this, "&Replace"};
    nana::textbox m_replace_edit{*this};
    nana::button  m_button2{*this, ""};
    nana::button  m_button3{*this, ""};
    nana::button  m_button4{*this, ""};
    nana::place   m_place;
};

