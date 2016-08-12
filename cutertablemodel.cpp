#include "cutertablemodel.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

#include <QDebug>

CuterTableModel::CuterTableModel(QStringList cols) :
    m_ColNames(cols)
{

}

QStringList CuterTableModel::columnNames() const
{
    return m_ColNames;
}

void CuterTableModel::setColumnNames(const QStringList &ColNames)
{
    m_ColNames = ColNames;
}

bool CuterTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role!=Qt::DisplayRole)
        return QAbstractTableModel::setData(index,value,role);

    if (index.column()>=m_ColNames.size()) return false;
    if (index.row()>=m_data.size()) return false;

    m_data[index.row()][index.column()]=value;

    return true;
}

QVariant CuterTableModel::data(const QModelIndex &index, int role) const
{
    if (role!=Qt::DisplayRole) return QVariant();
    if (index.column()>=m_ColNames.size()) return QVariant();
    if (index.row()>=m_data.size()) return QVariant();

    auto varList=m_data[index.row()];
    return varList.value(index.column());
}

int CuterTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int CuterTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_ColNames.size();
}

QVariant CuterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role!=Qt::DisplayRole) return QAbstractTableModel::headerData(section,orientation,role);
    if (orientation==Qt::Vertical) return QAbstractTableModel::headerData(section,orientation,role);

    return m_ColNames.at(section);
}

QJsonObject CuterTableModel::toJsonObject(int row, bool *hasErrorPtr) const
{
    QJsonObject o;
    if (m_data.size() <=row || row<0)
    {
        if (hasErrorPtr) (*hasErrorPtr)=false;
        return o;
    }
    auto list(m_data.at(row));
    for (int i=0; i < m_ColNames.size();i++)
    {
        o.insert(m_ColNames[i],list[i].toString());
    }
    //qDebug()<<"Returning " << o;
    (*hasErrorPtr)=true;
    return o;
}

CuterTableModel* CuterTableModel::fromJson(QString filename)
{
    auto t=new CuterTableModel();

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug()<<"CuterTableModel::fromJson() Couldn't open " << filename;
        return t;
    }
    auto bytes=f.readAll();
    auto jsonDoc=QJsonDocument::fromJson(bytes);

    if (!jsonDoc.isArray()) {
        qDebug()<<"CuterTableModel::fromJson() File can be opened, but not in array format " << filename;
        return t;
    }

    auto array=jsonDoc.array();
    for (auto element : array) {
        if (!element.isObject()) {
            qDebug()<<"CuterTableModel::fromJson() Element in Json array not an object";
            continue;
        }
        auto obj=element.toObject();
        if (!t->columnCount()) {
            t->setColumnNames(obj.keys());
        }
        QStringList keys=t->columnNames();
        QStringList values;
        for (auto key : keys) {
            values.append(obj[key].toString());
        }
        t->prependRow(values);
    }
    return t;
}

QJsonDocument CuterTableModel::toJsonDoc() const
{
    QJsonArray a;
    for (int i=0; i < rowCount(); i++)
    {
        qDebug()<<i << rowCount();
        bool isOK=false;
        auto doc=toJsonObject(i,&isOK);
        if (isOK)
            a.append(doc);

    }
    QJsonDocument d;
    d.setArray(a);
    return d;
}

bool CuterTableModel::saveAsJson(QString filename) const
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    QJsonDocument d(toJsonDoc());
    QTextStream s(&f);
    s << d.toJson();
    return true;
}

bool CuterTableModel::addRow(int rowNum, QStringList stuff)
{
    if (rowNum>m_data.size()+1)
        return false;
    QList<QVariant> insertMe;

    // For performance
    insertMe.reserve(m_ColNames.size());

    for (int i=0; i < qMin(stuff.size(),m_ColNames.size());i++)
        insertMe.append(stuff.at(i));
    beginInsertRows(QModelIndex(),rowNum,rowNum);
    m_data.insert(rowNum,insertMe);
    endInsertRows();
    return true;
}

bool CuterTableModel::prependRow(QStringList stuff)
{
    return addRow(0,stuff);
}

int CuterTableModel::rowCount() const
{
    return rowCount(QModelIndex());
}

int CuterTableModel::columnCount() const
{
    return columnCount(QModelIndex());
}

