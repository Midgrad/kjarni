#include "json_source_file.h"

#include <QFile>

using namespace md::data_source;

JsonSourceFile::JsonSourceFile(const QString& fileName) : m_fileName(fileName)
{
}

QString JsonSourceFile::errorString() const
{
    return m_errorString;
}

QJsonDocument JsonSourceFile::read()
{
    QFile file(m_fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    m_errorString = file.errorString();
    file.close();

    return document;
}

void JsonSourceFile::save(const QJsonDocument& document)
{
    QFile file(m_fileName);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    file.write(document.toJson());
    file.close();
}
