#ifndef PROTOCOL_SPECIFICATION_H
#define PROTOCOL_SPECIFICATION_H

#include <QObject>
#include <QVariant>

namespace md::domain
{
class ProtocolSpecification : public QObject
{
    Q_OBJECT

public:
    ProtocolSpecification() = default;
    ProtocolSpecification(const QString& name, QObject* parent = nullptr);
    ProtocolSpecification(const ProtocolSpecification& another, QObject* parent = nullptr);

    ProtocolSpecification& operator=(const ProtocolSpecification& another);

    QString name() const;

private:
    QString m_name;
};
} // namespace md::domain

#endif //PROTOCOL_SPECIFICATION_H
