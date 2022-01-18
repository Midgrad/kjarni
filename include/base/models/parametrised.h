#ifndef PARAMETRISED_H
#define PARAMETRISED_H

#include "named.h"

#include <QVector>

namespace md::domain
{
class Parameter
{
    Q_GADGET

public:
    enum Type
    {
        Bool,
        Int,
        Real,
        LatLon
    };

    Parameter(const QString& id, const QString& name, Type type, const QVariant& defaultValue,
              const QVariant& minValue, const QVariant& maxValue, const QVariant& step = 1);
    Parameter(const QString& id, const QString& name, bool defaultValue = false);

    QVariantMap toVariantMap() const;
    QVariant guard(const QVariant& value) const;

    const QString id;
    const QString name;
    const Type type;
    const QVariant defaultValue;
    const QVariant minValue;
    const QVariant maxValue;
    const QVariant step;

    Q_ENUM(Type)
};

class Parametrised : public Named
{
    Q_OBJECT

public:
    explicit Parametrised(const QVariant& id = utils::generateId(), const QString& name = QString(),
                          const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    explicit Parametrised(const QVariantMap& map, QObject* parent = nullptr);

    const QVariantMap& parameters() const;
    QVariant parameter(const QString& key) const;
    bool hasParameter(const QString& key) const;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

public slots:
    void setParameters(const QVariantMap& parameters);
    void setParameter(const QString& key, const QVariant& value);
    void setParameter(const Parameter* parameter, const QVariant& value);
    void removeParameters(const QStringList& keys);
    void removeParameter(const QString& key);

protected:
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // PARAMETRISED_H
