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

    // Missions
    query.exec("CREATE TABLE missions ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING, "
               "vehicle UUID, "
               "FOREIGN KEY(vehicle) REFERENCES vehicles(id) ON DELETE CASCADE);");

    query.exec("CREATE TABLE mission_items ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "position TEXT, "
               "type STRING, "
               "mission UUID, "
               "FOREIGN KEY(mission) REFERENCES missions(id) ON DELETE CASCADE);");

    query.exec("INSERT INTO schema_version (version) VALUES (\'17.14.00_09.11.2021\')");
}
