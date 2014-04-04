#ifndef INFOFILE_H
#define INFOFILE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>

class InfoFile
{
public:
    static InfoFile* getInstance();

    bool open();

    void close();

    void write(QString text);

    void beginText();

    void endText();

    InfoFile& operator <<(QString text);

    InfoFile& operator <<(int num);

private:
    InfoFile(QString filename);

private:
    QString m_filename;
    QFile m_infofile;
    QTextStream m_ts;
};

#endif // INFOFILE_H
