#ifndef LINK_PTR_H
#define LINK_PTR_H

#include "link_async.h"

#include <QMap>
#include <QString>

namespace md::data_source
{
using LinkPtr = QSharedPointer<loodsman::LinkAsync>;
using LinkPtrMap = QMap<QString, LinkPtr>;
} // namespace md::data_source

#endif //LINK_PTR_H
