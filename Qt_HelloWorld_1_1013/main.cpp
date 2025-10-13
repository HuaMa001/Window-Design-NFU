#include <QCoreApplication>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std :: cout<<"HelloWorld"<<std::endl;
    system("pause");
    return a.exec();
}
