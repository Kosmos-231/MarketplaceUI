#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MarketplaceUI.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

class MarketplaceUI : public QMainWindow
{
    Q_OBJECT

public:
    MarketplaceUI(QWidget* parent = nullptr);
    ~MarketplaceUI();

private slots:
    void onParseButtonClicked();
    void onExportButtonClicked();

private:
    Ui::MarketplaceUIClass ui;

    // Переменные для хранения элементов графика
    QChart* chart = nullptr;
    QChartView* chartView = nullptr;
    QLineSeries* series = nullptr;

    // Вспомогательная функция для обновления графика
    void updateChart(const QString& productId);
};