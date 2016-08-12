#include "mainwindow.h"
#include <QApplication>

#include "cutertablemodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    CuterTableModel m({"Author","Book"});
//    QStringList book{"Neil Gaiman","American Gods"};
//    QStringList book2{"Sarah Vowell","Lafayette in the Somewhat United States"};
//    QStringList book3{"Viet Nygen","The Sympathizer"};
//    QStringList book4{"Ta Na Hasi Coates","Between the World and Me"};
//    m.prependRow(book);
//    m.prependRow(book2);
//    m.prependRow(book3);
//    m.prependRow(book4);
//    m.saveAsJson("books.json");
    auto model=CuterTableModel::fromJson("books.json");
    w.setModel(model);

    return a.exec();
}
