#ifndef INFOFILE_H
#define INFOFILE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>

class InfoFile : public QObject
{
    Q_OBJECT
public:
    InfoFile(QString filename, QObject *parent = 0);

    bool open();

    void close();

    void write(QString text);

    void beginText();

    void endText();

    InfoFile& operator<<(QString text);

private:
    QString m_filename;
    QFile m_infofile;
    QTextStream m_ts;
};

#endif // INFOFILE_H
