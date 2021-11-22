#ifndef I_GUI_LAYOUT_H
#define I_GUI_LAYOUT_H

#include "i_service.h"

#include <QString>

namespace md::presentation
{
class IGuiLayout : public md::domain::IService
{
public:
    IGuiLayout() = default;

    virtual void addItem(const QString& place, const QString& url) = 0;
};
} // namespace md::presentation

#endif // I_GUI_LAYOUT_H
