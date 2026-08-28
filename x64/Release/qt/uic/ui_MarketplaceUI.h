/********************************************************************************
** Form generated from reading UI file 'MarketplaceUI.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKETPLACEUI_H
#define UI_MARKETPLACEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarketplaceUIClass
{
public:
    QWidget *chartWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *idInput;
    QPushButton *parseButton;
    QPushButton *exportButton;
    QLabel *resultLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MarketplaceUIClass)
    {
        if (MarketplaceUIClass->objectName().isEmpty())
            MarketplaceUIClass->setObjectName("MarketplaceUIClass");
        MarketplaceUIClass->resize(950, 655);
        chartWidget = new QWidget(MarketplaceUIClass);
        chartWidget->setObjectName("chartWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(chartWidget->sizePolicy().hasHeightForWidth());
        chartWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(chartWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        idInput = new QLineEdit(chartWidget);
        idInput->setObjectName("idInput");

        verticalLayout->addWidget(idInput);

        parseButton = new QPushButton(chartWidget);
        parseButton->setObjectName("parseButton");

        verticalLayout->addWidget(parseButton);

        exportButton = new QPushButton(chartWidget);
        exportButton->setObjectName("exportButton");

        verticalLayout->addWidget(exportButton);

        resultLabel = new QLabel(chartWidget);
        resultLabel->setObjectName("resultLabel");

        verticalLayout->addWidget(resultLabel);

        MarketplaceUIClass->setCentralWidget(chartWidget);
        menuBar = new QMenuBar(MarketplaceUIClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 950, 21));
        MarketplaceUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MarketplaceUIClass);
        mainToolBar->setObjectName("mainToolBar");
        MarketplaceUIClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);

        retranslateUi(MarketplaceUIClass);

        QMetaObject::connectSlotsByName(MarketplaceUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *MarketplaceUIClass)
    {
        MarketplaceUIClass->setWindowTitle(QCoreApplication::translate("MarketplaceUIClass", "MarketplaceUI", nullptr));
        parseButton->setText(QCoreApplication::translate("MarketplaceUIClass", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        exportButton->setText(QCoreApplication::translate("MarketplaceUIClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262 Excel ", nullptr));
        resultLabel->setText(QCoreApplication::translate("MarketplaceUIClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MarketplaceUIClass: public Ui_MarketplaceUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKETPLACEUI_H
