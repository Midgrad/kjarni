#include "sqlite_schema.h"

#include <QSqlError>
#include <QSqlQuery>

namespace
{
constexpr char connectionType[] = "QSQLITE";
} // namespace

using namespace md::data_source;

SqliteSchema::SqliteSchema(const QString& databaseName) :
    m_db(QSqlDatabase::addDatabase(::connectionType))
{
    m_db.setDatabaseName(databaseName);
}

QSqlDatabase* SqliteSchema::db()
{
    return &m_db;
}

void SqliteSchema::setup()
{
    if (!m_db.open())
    {
        qCritical("Can't open database");
    }

    // TODO: replace with some migrations framework
    QSqlQuery query;
    query.exec("SELECT version FROM schema_version ORDER BY version DESC LIMIT 1");
    if (query.next())
    {
        // Already have schema
        return;
    }

    query.exec("PRAGMA FOREIGN_KEYS=ON;");

    // Version
    query.exec("CREATE TABLE schema_version (version STRING NOT NULL UNIQUE)");

    // Comm Links
    query.exec("CREATE TABLE comm_link_types ("
               "id STRING NOT NULL UNIQUE, "
               "name STRING);");

    query.exec("CREATE TABLE comm_link_params ("
               "id STRING NOT NULL UNIQUE, "
               "name STRING, "
               "minValue STRING, "
               "maxValue STRING, "
               "step STRING, "
               "options TEXT, "
               "type STRING );");

    query.exec("CREATE TABLE comm_link_type_params ("
               "type STRING, "
               "param STRING, "
               "defaultValue STRING, "
               "FOREIGN KEY(type) REFERENCES comm_link_types(id) ON DELETE CASCADE, "
               "FOREIGN KEY(param) REFERENCES comm_link_params(id) ON DELETE CASCADE);");

    query.exec("CREATE TABLE comm_links ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "protocol STRING, "
               "autoconnect BOOL, "
               "type STRING, "
               "FOREIGN KEY(type) REFERENCES comm_link_types(id) ON DELETE CASCADE);");

    query.exec("INSERT INTO comm_link_types (id, name) VALUES (\'udp\', \'UDP\')");
    query.exec("INSERT INTO comm_link_types (id, name) VALUES (\'tcp\', \'TCP\')");
    query.exec("INSERT INTO comm_link_types (id, name) VALUES (\'uart\', \'UART\')");

    query.exec("INSERT INTO comm_link_params (id, name, type, minValue, maxValue, step) "
               "VALUES (\'port\', \'Port\', \'Int\', 1024, 65535, 1)");
    query.exec("INSERT INTO comm_link_params (id, name, type) "
               "VALUES (\'address\', \'Address\', \'Text\')");
    query.exec("INSERT INTO comm_link_params (id, name, type) "
               "VALUES (\'device\', \'Device\', \'Combo\')");
    query.exec("INSERT INTO comm_link_params (id, name, type, options) "
               "VALUES (\'baud_rate\', \'Baud rate\', \'Combo\', "
               "\'2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200\')");

    query.exec("INSERT INTO comm_link_type_params (type, param, defaultValue) "
               "VALUES (\'udp\', \'port\', 14550)");
    query.exec("INSERT INTO comm_link_type_params (type, param, defaultValue) "
               "VALUES (\'tcp\', \'port\', 5760)");
    query.exec("INSERT INTO comm_link_type_params (type, param, defaultValue) "
               "VALUES (\'tcp\', \'address\', \'127.0.0.1\')");
    query.exec("INSERT INTO comm_link_type_params (type, param) "
               "VALUES (\'uart\', \'device\')");
    query.exec("INSERT INTO comm_link_type_params (type, param, defaultValue) "
               "VALUES (\'uart\', \'baud_rate\', 57600)");

    // Vehicles
    query.exec("CREATE TABLE vehicles ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "type STRING);");

    // Missions
    query.exec("CREATE TABLE missions ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING, "
               "vehicle UUID NOT NULL, "
               "visible BOOL, "
               "FOREIGN KEY(vehicle) REFERENCES vehicles(id) ON DELETE CASCADE);");

    query.exec("CREATE TABLE mission_items ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "position TEXT, "
               "type STRING, "
               "mission UUID NOT NULL, "
               "FOREIGN KEY(mission) REFERENCES missions(id) ON DELETE CASCADE);");

    query.exec("INSERT INTO schema_version (version) VALUES (\'17.14.00_09.11.2021\')");
}
