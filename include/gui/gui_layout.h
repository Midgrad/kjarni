#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "i_gui_layout.h"

#include <QJsonObject>

namespace md::presentation
{
class GuiLayout : public IGuiLayout
{
public:
    GuiLayout();

    void addItem(const QString& place, const QString& url) override;

    const QJsonObject& items() const;

private:
    QJsonObject m_items;
};
} // namespace md::presentation

#endif // GUI_LAYOUT_H
