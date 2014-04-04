#include "infofile.h"


InfoFile::InfoFile(QString filename, QObject *parent)
    :QObject(parent)
{
    m_filename = filename;
    m_infofile.setFileName(m_filename);
}

bool InfoFile::open()
{
    if (!m_infofile.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        return false;
    m_ts.setDevice(&m_infofile);
    return true;
}

void InfoFile::close()
{
    m_infofile.close();
}

void InfoFile::write(QString text)
{
    open();
    *this << text;
    close();
}

InfoFile &InfoFile::operator<<(QString text)
{
    m_ts << text;
    return *this;
}
