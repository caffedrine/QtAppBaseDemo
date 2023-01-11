#include "QTabBarExtended.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

QTabBarExtended::QTabBarExtended(QTabWidget *parent): QTabBar(parent), parentTabWidget(parent), currTabEdit(nullptr), currTabEditIndex(-1)
{
    // Create + tab used to add new tabs
    this->addTab("");
    int lastTabIndex = this->count() - 1;
    this->setTabEnabled(lastTabIndex, false);

    // Create a push button
    QPushButton* newTabBtn = new QPushButton(parent);
    newTabBtn->setText("+");
    newTabBtn->setStyleSheet(QString("QPushButton {border: 0; padding: 0; padding-bottom: 1px; text-align: center; border-radius: 3px; font-weight: bold;} QPushButton:hover { background-color: #cccccc; }"));

    QObject::connect(newTabBtn, &QPushButton::clicked, [this](){
        emit NewTabRequested();
    });

    this->setTabButton(lastTabIndex, QTabBar::RightSide, newTabBtn);
    newTabBtn->resize(this->tabRect(lastTabIndex).height(), this->tabRect(lastTabIndex).height() - 3);

    // Disable passing for last tab
    this->setStyleSheet(QString("QTabBar::tab:last { padding: 0; padding-left: 5px; margin: 0px; padding-bottom: 2px; border: none;} QTabBar::tab:selected { font-weight: bold; }"));
}

void QTabBarExtended::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Get the index of the tab under the mouse cursor
    int index = tabAt(event->pos());
    if (index >= 0)
    {
        // If we are ady editing another time, cancel editing of that tab and switch to editing the one newly selected
        if(currTabEdit != nullptr )
        {
            setTabText(currTabEditIndex, currTabEditInitialText);
            currTabEdit->blockSignals(true);
            currTabEdit->removeEventFilter(nullptr);
            currTabEdit->deleteLater();
            currTabEdit = nullptr;
            currTabEditIndex = -1;
        }

        // Save the index of the edited tab
        currTabEditIndex = index;

        // Create a line edit widget
        currTabEdit = new QLineEdit(tabText(index), this);
        currTabEdit->installEventFilter(new CancelEditFilter(currTabEdit, tabText(index)));
        currTabEdit->setAttribute(Qt::WA_DeleteOnClose);
        currTabEdit->resize(tabRect(index).size());
        currTabEdit->move(tabRect(index).topLeft());
        currTabEdit->setFocus();
        currTabEdit->show();
        currTabEdit->selectAll();
        currTabEditInitialText = tabText(index);

        // Connect the line edit signals
        QObject::connect(currTabEdit, &QLineEdit::editingFinished, this, [this](){
            // Set the new tab text and delete the line edit widget
            setTabText(currTabEditIndex, currTabEdit->text());
            currTabEdit->deleteLater();
            currTabEdit = nullptr;
            currTabEditIndex = -1;
        });
        QObject::connect(currTabEdit, &QLineEdit::textEdited, this, [this](const QString &text){
            // Set the tab text as the line edit text is changed
            setTabText(currTabEditIndex, text);

            // Resize the line edit widget to fit the text
            // Get the size of the text in the line edit
            QFontMetrics fontMetrics = currTabEdit->fontMetrics();
            QSize size = fontMetrics.size(Qt::TextSingleLine, currTabEdit->text());

            // Add some extra space for the cursor and the frame of the line edit
            size.setWidth(size.width() + fontMetrics.averageCharWidth() + 5);
            size.setHeight(size.height() + 4);

            // Resize the line edit
            currTabEdit->resize(size);
        });
    }
}

void QTabBarExtended::paintEvent(QPaintEvent *event)
{
    QTabBar::paintEvent(event);
    return;

    // Calculate the position and size of the button
    const int tabCount = count();
    if (tabCount > 0)
    {
        const QRect lastTabRect = this->tabRect(tabCount - 1);

        qDebug() << "tab: " << lastTabRect;

        const int buttonSize = lastTabRect.height() / 2;
//        const QPoint buttonPos(lastTabRect.right(), lastTabRect.height() / 2);
        //const QRect buttonRect(buttonPos.x() - buttonSize / 2, buttonPos.y() - buttonSize / 2, buttonSize, buttonSize);
        const QPoint buttonPos(lastTabRect.right() + buttonSize / 4, height() / 2 - buttonSize / 2);


        qDebug() << "button pos: " <<  buttonPos;

        // Create a painter and draw the button
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw the button background
        painter.setBrush(QBrush(QColor(200, 200, 200)));
        painter.setPen(QPen(Qt::NoPen));
        painter.drawEllipse(buttonPos, buttonSize, buttonSize);

        // Draw the "+" sign
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(buttonPos.x() + buttonSize / 4, buttonPos.y() + buttonSize / 2, buttonPos.x() + 3 * buttonSize / 4, buttonPos.y() + buttonSize / 2);
        painter.drawLine(buttonPos.x() + buttonSize / 2, buttonPos.y() + buttonSize / 4, buttonPos.x() + buttonSize / 2, buttonPos.y() + 3 * buttonSize / 4);
    }
}

void QTabBarExtended::mousePressEvent(QMouseEvent *event)
{
//    // Check if the mouse press event occurred on the button
//    const int tabCount = count();
//    if (tabCount > 0)
//    {
//        const QRect lastTabRect = tabRect(tabCount - 1);
//        const QPoint buttonPos = lastTabRect.topRight();
//        const int buttonSize = lastTabRect.height();
//        const QRect buttonRect(buttonPos.x() - buttonSize / 2, buttonPos.y() - buttonSize / 2, buttonSize, buttonSize);
//        if (buttonRect.contains(event->pos()))
//        {
//            // Emit the signal to add a new tab
//            emit NewTabRequested();
//            return;
//        }
//    }

    QTabBar::mousePressEvent(event);
}
