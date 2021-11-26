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

    query.exec("CREATE TABLE vehicles ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "type STRING);");

    query.exec("CREATE TABLE route_items ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "params TEXT, "
               "type STRING, "
               "parent UUID);"); // parent can be other item, route or mission

    query.exec("CREATE TABLE routes ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING);");

    // TODO: specialised mission route
    query.exec("CREATE TABLE missions ("
               "id UUID PRIMARY KEY NOT NULL, "
               "name STRING, "
               "type STRING, "
               "route UUID, "
               "vehicle UUID, "
               "FOREIGN KEY(route) REFERENCES routes(id) ON DELETE CASCADE, "
               "FOREIGN KEY(vehicle) REFERENCES vehicles(id) ON DELETE CASCADE);");

    query.exec("INSERT INTO schema_version (version) VALUES (\'17.14.00_09.11.2021\')");
}
