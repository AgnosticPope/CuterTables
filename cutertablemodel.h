#ifndef CUTERTABLEMODEL_H
#define CUTERTABLEMODEL_H

#include <QAbstractTableModel>

class CuterTableModel : public QAbstractTableModel
{
public:
    CuterTableModel(QStringList colNames={});

    // Virtual overrides
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

    // Convenience functions
    int rowCount() const;
    int columnCount() const;

    // JSON I/O
    bool saveAsJson(QString filename) const;
    QJsonDocument toJsonDoc() const;
    QJsonObject toJsonObject(int row, bool* hasError=nullptr) const;

    static CuterTableModel *fromJson(QString filename);

    QStringList  columnNames() const;
    void setColumnNames(const QStringList &columnNames);
    bool addRow(int rowNum, QStringList stuff=QStringList());
    bool prependRow(QStringList stuff);
signals:

public slots:
protected:

private:
    QStringList m_ColNames;
    QList<QList<QVariant> > m_data;
};

#endif // CUTERTABLEMODEL_H
