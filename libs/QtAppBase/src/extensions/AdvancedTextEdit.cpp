#include "AdvancedTextEdit.h"

AdvancedTextEdit::AdvancedTextEdit(QWidget *parent): QTextEdit(parent)
{
    this->AdvancedTextEditInit();
}

AdvancedTextEdit::AdvancedTextEdit(const QString &text, QWidget *parent): QTextEdit(text, parent)
{
    this->AdvancedTextEditInit();
}

void AdvancedTextEdit::AdvancedTextEditInit()
{
    this->setAcceptDrops(true);
}

void AdvancedTextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void AdvancedTextEdit::dropEvent(QDropEvent *e)
{
    for (const QUrl &url: e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        emit OnDraggedFile(fileName);
    }
}
