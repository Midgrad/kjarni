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

    query.exec("CREATE TABLE missions (id STRING PRIMARY KEY NOT NULL, name STRING, params TEXT, "
               "type STRING, "
               "vehicle STRING);");
    query.exec("CREATE TABLE routes (id STRING PRIMARY KEY NOT NULL, name STRING, params TEXT, "
               "type STRING);");
    query.exec("CREATE TABLE waypoints (id STRING PRIMARY KEY NOT NULL, name STRING, params TEXT, "
               "type STRING);");

    query.exec("INSERT INTO schema_version (version) VALUES (\'14.10.00_19.10.2021\')");

    if (query.lastError().type() != QSqlError::NoError)
    {
        qCritical(query.lastError().text().toLocal8Bit());
    }
}
