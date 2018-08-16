#include "../inc/MainForm.hpp"



MainForm::MainForm():
    nana::form{nana::API::make_center(300, 200), nana::form::appear::decorate<>{}}
{
    m_text.create(*this, nana::rectangle{10, 10, 100, 100});
    m_text.caption("Hello, world!");
}

MainForm::~MainForm()
{}
