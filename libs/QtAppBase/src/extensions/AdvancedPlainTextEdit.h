#ifndef _ADVANCEDPLAINTEXTEDIT_H_
#define _ADVANCEDPLAINTEXTEDIT_H_

#include <QWidget>
#include <QTextEdit>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

class AdvancedPlainTextEdit: public QTextEdit
{
    Q_OBJECT
public:
    explicit AdvancedPlainTextEdit(QWidget *parent = nullptr);
    explicit AdvancedPlainTextEdit(const QString &text, QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

signals:
    void OnDraggedFile(QString filename);

private:
    void AdvancedTextEditInit();
};

#endif // _ADVANCEDPLAINTEXTEDIT_H_
