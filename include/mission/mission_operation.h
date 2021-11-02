#ifndef MISSION_OPERATION_H
#define MISSION_OPERATION_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class MissionOperation : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        Idle,
        Downloading,
        Uploading
    };

    MissionOperation(QObject* parent);

    Type type() const;
    int progress() const;
    int total() const;

    bool isComplete() const;
    QVariantMap toVariantMap() const;

public slots:
    void startDownload(int total);
    void startUpload(int total);
    void stop();
    void setProgress(int progress);

signals:
    void upload();   // To the vehicle
    void download(); // From the vehicle
    void cancel();   // Downloading or uploading
    void clear();    // Clear mission onborad and in storage

    void changed();

private:
    Type m_type = Idle;
    int m_progress = 0;
    int m_total = 0;

    Q_ENUM(Type);
};

} // namespace md::domain

#endif // MISSION_OPERATION_H
