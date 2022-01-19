#ifndef COMMUNICATION_TRAITS_H
#define COMMUNICATION_TRAITS_H

#include <QMap>
#include <QSharedPointer>
#include <QString>
#include <QVariantMap>

namespace md::domain
{
namespace link_parameters
{
const QString links = "links";

const QString id = "id";
const QString name = "name";
const QString type = "type";

const QString address = ("address");
const QString port = ("port");
const QString endpoint = ("endpoint");
const QString endpoints = ("endpoints");
const QString autoResponse = ("autoResponse");
const QString device = ("device");
const QString baudRate = ("baudRate");
const QString dataBits = ("dataBits");
const QString parity = ("parity");
const QString flowControl = ("flowControl");
const QString stopBits = ("stopBits");
} // namespace link_parameters

namespace link_type
{
const QString tcp = ("tcp");
const QString udp = ("udp");
const QString serial = ("serial");

const QMultiMap<QString, QString> parameters = {
    { tcp, link_parameters::address },        { tcp, link_parameters::port },
    { tcp, link_parameters::endpoint },       { udp, link_parameters::port },
    { udp, link_parameters::endpoints },      { udp, link_parameters::autoResponse },
    { serial, link_parameters::device },      { serial, link_parameters::baudRate },
    { serial, link_parameters::dataBits },    { serial, link_parameters::parity },
    { serial, link_parameters::flowControl }, { serial, link_parameters::stopBits }
};

const QVariantMap defaultValues{ { link_parameters::address, QString() },
                                 { link_parameters::port, 14550 },
                                 { link_parameters::endpoint, QVariantMap() },
                                 { link_parameters::endpoints, QVariantList() },
                                 { link_parameters::autoResponse, false },
                                 { link_parameters::device, QString() },
                                 { link_parameters::baudRate, 9600 },
                                 { link_parameters::dataBits, 8 },
                                 { link_parameters::parity, QString(("NoParity")) },
                                 { link_parameters::flowControl, QString(("NoFlowControl")) },
                                 { link_parameters::stopBits, 1 } };

} // namespace link_type
} // namespace md::domain

#endif //COMMUNICATION_TRAITS_H
