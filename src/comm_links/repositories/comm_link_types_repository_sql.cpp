#include "comm_link_types_repository_sql.h"

#include <QDebug>

namespace
{
constexpr char comm_link_types[] = "comm_link_types";
constexpr char comm_link_params[] = "comm_link_params";
constexpr char comm_link_type_params[] = "comm_link_type_params";
} // namespace

using namespace md::data_source;

CommLinkTypesRepositorySql::CommLinkTypesRepositorySql(QSqlDatabase* database) :
    ICommLinkTypesRepository(),
    m_typesTable(database, ::comm_link_types),
    m_paramsTable(database, ::comm_link_params),
    m_paramTypesTable(database, ::comm_link_type_params)
{
}

QList<const md::domain::CommLinkType*> CommLinkTypesRepositorySql::selectCommLinkTypes()
{
    QList<const md::domain::CommLinkType*> list;

    for (const QVariant& id : m_typesTable.selectIds())
    {
        QVariantMap map = m_typesTable.selectById(id);
        if (map.isEmpty())
            continue;

        const QString idString = id.toString();
        auto parameters = this->selectParametersForType(idString);
        auto type = new domain::CommLinkType(idString, map.value(domain::props::name).toString(),
                                             parameters);
        list.append(type);
    }
    return list;
}

QVector<const md::domain::ParameterType*> CommLinkTypesRepositorySql::selectParametersForType(
    const QString& id)
{
    QVector<const md::domain::ParameterType*> vec;
    for (const QVariantMap& map : m_paramTypesTable.select({ { domain::props::type, id } }))
    {
        QString paramId = map.value(domain::props::param).toString();
        if (paramId.isEmpty())
            continue;

        QVariantMap paramMap = m_paramsTable.selectById(paramId);
        paramMap.insert(domain::props::defaultValue, map.value(domain::props::defaultValue));

        vec.append(new domain::ParameterType(paramMap));
    }
    return vec;
}
