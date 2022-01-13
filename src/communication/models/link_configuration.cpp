#include "link_configuration.h"

using namespace md::domain;

//LinkConfiguration::LinkConfiguration(QString type, const QString& name, QObject* parent) :
//    Named(name, utils::generateId(), parent),
//    m_type(std::move(type)),
//    m_connected(false),
//    m_bytesReceived(0),
//    m_bytesSent(0)
//{
//    this->fillInParameters();
//}

LinkConfiguration::LinkConfiguration(QVariantMap parameters, const QString& name, QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_type(parameters.take(link_parameters::type).toString()),
    m_parameters(parameters),
    m_connected(false),
    m_bytesReceived(0),
    m_bytesSent(0)
{
    // TODO: Bring back after refactoring is finished
    //    Q_ASSERT(m_type != "");
    this->fillInParameters();
}

using namespace md::domain;

QString LinkConfiguration::type() const
{
    return m_type;
}

QVariant LinkConfiguration::parameter(const QString& parameter) const
{
    return m_parameters.value(parameter);
}

QVariantMap LinkConfiguration::parameters() const
{
    return m_parameters;
}

bool LinkConfiguration::isConnected() const
{
    return m_connected;
}

int LinkConfiguration::bytesReceived() const
{
    return m_bytesReceived;
}

int LinkConfiguration::bytesSent() const
{
    return m_bytesSent;
}

void LinkConfiguration::Connect()
{
    if (m_connected == true)
        return;

    m_connected = true;
    emit connectedChanged(m_connected);
}

void LinkConfiguration::Disconnect()
{
    if (m_connected == false)
        return;

    m_connected = false;
    emit connectedChanged(m_connected);
}

void LinkConfiguration::fillInParameters()
{
    if (!m_type.isNull())
    {
        for (const QString& parameter : link_type::parameters.values(m_type))
        {
            if (!m_parameters.contains(parameter))
            {
                m_parameters.insert(parameter, link_type::defaultValues.value(parameter));
            }
        }
    }
}