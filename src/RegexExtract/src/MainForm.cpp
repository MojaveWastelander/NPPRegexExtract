#include "../inc/MainForm.hpp"



MainForm::MainForm():
    nana::form{nana::API::make_center(800, 500), nana::form::appear::decorate<nana::form::appear::sizable>{}}
{
    m_place.bind(*this);
    m_place.div("<vertical <weight=20 arrange=[45,variable,50] find><weight=20 arrange=[45,variable,50] replace>|< vertical div2>");
    //m_find_lbl.create(*this, "Find");
    //m_find_btn.create(*this, "Search");
    //m_find_edit.create(*this, true);
    //m_replace_lbl.create(*this, "Replace");
    //m_replace_btn.create(*this, "Replace");
    //m_replace_edit.create(*this, true);

    m_button2.create(*this, "2");
    m_button3.create(*this, "3");
    m_button4.create(*this, "4");
    m_place.field("find") << m_find_lbl << m_find_edit << m_find_btn;
    m_place.field("replace") << m_replace_lbl << m_replace_edit << m_replace_btn;
    m_place.field("div2") << m_button3 << m_button4;
    m_place.collocate();
}

MainForm::~MainForm()
{}

void MainForm::events_processing()
{
    m_find_btn.events().click([&]()
    {
        m_place.field_visible("replace", !m_place.field_visible("replace"));
    });
}
