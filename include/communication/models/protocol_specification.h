#ifndef PROTOCOL_SPECIFICATION_H
#define PROTOCOL_SPECIFICATION_H

#include <QVariant>

namespace md::domain
{
class ProtocolSpecification
{
public:
    ProtocolSpecification() = default;
    ProtocolSpecification(const QString& name);

    QString name() const;

private:
    QString m_name;
};
} // namespace md::domain

#endif //PROTOCOL_SPECIFICATION_H
