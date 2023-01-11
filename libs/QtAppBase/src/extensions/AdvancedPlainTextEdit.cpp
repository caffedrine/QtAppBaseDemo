#include "AdvancedPlainTextEdit.h"

AdvancedPlainTextEdit::AdvancedPlainTextEdit(QWidget *parent): QTextEdit(parent)
{
    this->AdvancedTextEditInit();
}

AdvancedPlainTextEdit::AdvancedPlainTextEdit(const QString &text, QWidget *parent): QTextEdit(text, parent)
{
    this->AdvancedTextEditInit();
}

void AdvancedPlainTextEdit::AdvancedTextEditInit()
{
    this->setAcceptDrops(true);
}

void AdvancedPlainTextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void AdvancedPlainTextEdit::dropEvent(QDropEvent *e)
{
    for (const QUrl &url: e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        emit OnDraggedFile(fileName);
    }
}
