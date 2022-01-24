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

    query.exec("CREATE TABLE schema_version (version STRING NOT NULL UNIQUE)");

    // Vehicles
    query.exec("CREATE TABLE vehicles ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "type STRING);");

    // Routes
    query.exec("CREATE TABLE routes ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING);");

    query.exec("CREATE TABLE route_items ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "position TEXT, "
               "calcData TEXT, "
               "type STRING, "
               "route UUID, "
               "FOREIGN KEY(route) REFERENCES routes(id) ON DELETE CASCADE);");

    // Missions
    query.exec("CREATE TABLE missions ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING, "
               "route UUID, "
               "vehicle UUID, "
               "FOREIGN KEY(route) REFERENCES routes(id) ON DELETE CASCADE, "
               "FOREIGN KEY(vehicle) REFERENCES vehicles(id) ON DELETE CASCADE);");

    query.exec("CREATE TABLE home_items ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "position TEXT, "
               "type STRING, "
               "mission UUID, "
               "FOREIGN KEY(mission) REFERENCES missions(id) ON DELETE CASCADE);");

    query.exec("CREATE TABLE communication_descriptions ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "type STRING, "
               "state BOOL, "
               "protocol STRING);");

    query.exec("INSERT INTO schema_version (version) VALUES (\'17.14.00_09.11.2021\')");
}
