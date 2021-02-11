#include "exclass.h"

ExClass::ExClass(QObject *parent)
    : QObject(parent)
    , m_string("init value")
    , stringProperty("init value") // for Qt 6+
{

}

void ExClass::setStringText(QString str)
{
    if(str != m_string){
        m_string = str;
        emit stringChanged(m_string);
    }

}

QString ExClass::getString()
{
 return m_string;
}
