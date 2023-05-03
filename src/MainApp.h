#ifndef _MAINAPP_H_
#define _MAINAPP_H_

#include <QObject>

class MainApp: public QObject
{
    Q_OBJECT

private:
    void exit(int retcode);

public:
    explicit MainApp(QObject *parent = nullptr);
    ~MainApp() override;
};


#endif //_MAINAPP_H_
