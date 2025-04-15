// HistoryDialog.cpp
#include "HistoryDialog.h"
#include <QPushButton>

HistoryDialog::HistoryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Event History");
    setFixedSize(400, 300);

    setStyleSheet(R"(
        QDialog {
            background-color: #f9f9f9;
            border-radius: 15px;
        }
        QLabel {
            font-size: 18px;
            font-weight: bold;
            color: #3e5879;
            padding-bottom: 10px;
        }
        QListWidget {
            background-color: #ffffff;
            border: 1px solid #ccc;
            border-radius: 8px;
            font-size: 14px;
            padding: 8px;
            color: #333;
        }
        QPushButton {
            background-color: #3e5879;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #5c759a;
        }
    )");

<<<<<<< HEAD
    layout = new QVBoxLayout(this);
=======
    layout = new QVBoxLayout(this);//layout vertival
>>>>>>> 8a1c2a3 (final)
    titleLabel = new QLabel("Event History Details:", this);

    historyListWidget = new QListWidget(this);

    layout->addWidget(titleLabel);
    layout->addWidget(historyListWidget);

    QPushButton *closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &HistoryDialog::accept);
}

void HistoryDialog::setHistory(const QStringList &history)
{
    historyListWidget->clear();
    historyListWidget->addItems(history);
}
