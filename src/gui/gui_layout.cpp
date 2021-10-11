#include "gui_layout.h"

#include "utils.h"

using namespace md::presentation;

GuiLayout::GuiLayout()
{
}

void GuiLayout::addItem(const QString& place, const QString& url)
{
    md::utils::insertInArray(m_items, place, url);
}

const QJsonObject& GuiLayout::items() const
{
    return m_items;
}
