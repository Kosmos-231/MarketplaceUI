#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

#include "MarketplaceUI.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QProcess>
#include <QDebug>
#include <QVBoxLayout>
#include <QApplication>
#include <Qicon>

MarketplaceUI::MarketplaceUI(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // 1. Инициализируем объекты графика
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Динамика изменения цены");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(300); // Чтобы график не сжимался до нуля

    // 2. Исправленный поиск компоновщика главного окна
    // В Qt centralWidget() вызывается напрямую у QMainWindow
    if (this->centralWidget() && this->centralWidget()->layout()) {
        this->centralWidget()->layout()->addWidget(chartView);
    }

    // 3. Связываем нажатие кнопки с функцией
    connect(ui.parseButton, &QPushButton::clicked, this, &MarketplaceUI::onParseButtonClicked);
    connect(ui.exportButton, &QPushButton::clicked, this, &MarketplaceUI::onExportButtonClicked);
    this->setWindowIcon(QIcon("icopng.ico"));
}

MarketplaceUI::~MarketplaceUI()
{
    // Очищаем память
    if (chartView) delete chartView;
}

void MarketplaceUI::onParseButtonClicked()
{
    QString productId = ui.idInput->text().trimmed();

    if (productId.isEmpty()) {
        ui.resultLabel->setText("Ошибка: Введите ID товара!");
        return;
    }

    ui.resultLabel->setText("Пожалуйста, подождите...\nСобираем данные...");
    ui.parseButton->setEnabled(false);
    qApp->processEvents();

    // Запускаем Python-скрипт в фоне
    QProcess pythonProcess;
    QStringList arguments;
    arguments << "main.py" << productId;
    pythonProcess.start("python", arguments);

    if (!pythonProcess.waitForFinished(20000)) {
        ui.resultLabel->setText("Ошибка: Скрипт Python не отвечает.");
        ui.parseButton->setEnabled(true);
        return;
    }

    // Открываем БД SQLite для получения информации
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("marketplace.db");

    if (!db.open()) {
        ui.resultLabel->setText("Ошибка подключения к БД: " + db.lastError().text());
        ui.parseButton->setEnabled(true);
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name, brand, price FROM products WHERE id = :id ORDER BY timestamp DESC LIMIT 1");
    query.bindValue(":id", productId);

    if (query.exec() && query.next()) {
        QString name = query.value(0).toString();
        QString brand = query.value(1).toString();
        int price = query.value(2).toInt();

        QString resultText = QString("📦 Товар: %1\n🏢 Бренд: %2\n💰 Цена: %3 руб.")
            .arg(name).arg(brand).arg(price);
        ui.resultLabel->setText(resultText);

        // Обновляем график
        updateChart(productId);
    }
    else {
        ui.resultLabel->setText("Ошибка: Данные не появились в базе.");
    }

    db.close();
    ui.parseButton->setEnabled(true);
}

void MarketplaceUI::updateChart(const QString& productId)
{
    // Очищаем старые данные
    chart->removeAllSeries();

    for (QAbstractAxis* axis : chart->axes()) {
        chart->removeAxis(axis);
        delete axis;
    }

    series = new QLineSeries();

    QSqlQuery query;
    query.prepare("SELECT price FROM products WHERE id = :id ORDER BY timestamp ASC");
    query.bindValue(":id", productId);

    int pointIndex = 0;
    int minPrice = 999999;
    int maxPrice = 0;

    if (query.exec()) {
        while (query.next()) {
            int price = query.value(0).toInt();
            series->append(pointIndex, price);
            pointIndex++;

            if (price < minPrice) minPrice = price;
            if (price > maxPrice) maxPrice = price;
        }
    }

    if (pointIndex > 0) {
        chart->addSeries(series);
        chart->createDefaultAxes();

        QAbstractAxis* axisX = chart->axes(Qt::Horizontal).first();
        axisX->setRange(0, qMax(1, pointIndex - 1));

        QAbstractAxis* axisY = chart->axes(Qt::Vertical).first();
        if (minPrice == maxPrice) {
            axisY->setRange(minPrice - 1000, minPrice + 1000);
        }
        else {
            axisY->setRange(minPrice - 500, maxPrice + 500);
        }

        chartView->update();
    }
}
void MarketplaceUI::onExportButtonClicked()
{
    // Здесь всё верно: ui с точкой, idInput со стрелочкой
    QString productId = ui.idInput->text().trimmed();

    if (productId.isEmpty()) {
        // ИСПРАВЛЕНО: была стрелочка после ui, теперь точка!
        ui.resultLabel->setText("Ошибка: Введите ID товара для экспорта!");
        return;
    }

    // ИСПРАВЛЕНО: была стрелочка после ui, теперь точка!
    ui.resultLabel->setText("Экспортируем историю цен в Excel...");
    qApp->processEvents();

    // Запускаем Python-скрипт с флагом "--export"
    QProcess pythonProcess;
    QStringList arguments;
    arguments << "main.py" << productId << "--export"; // Передаем спец-флаг
    pythonProcess.start("python", arguments);

    if (!pythonProcess.waitForFinished(20000)) {
        ui.resultLabel->setText("Ошибка: Скрипт экспорта не отвечает.");
        return;
    }

    // Читаем, что ответил Python (для отладки)
    QString output = pythonProcess.readAllStandardOutput();
    qDebug() << output;

    if (output.contains("[Экспорт] Успешно")) {
        ui.resultLabel->setText(QString("Успех!\nИстория сохранена в файл:\nhistory_product_%1.xlsx").arg(productId));
    }
    else {
        ui.resultLabel->setText("Ошибка при экспорте.\nВозможно, по этому ID еще нет истории.");
    }
}