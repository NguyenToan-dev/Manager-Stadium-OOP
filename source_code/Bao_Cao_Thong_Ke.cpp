#include "Bao_Cao_Thong_Ke.h"

#include <QCoreApplication>
#include <QApplication>
#include <QToolTip>
#include <QCursor>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QLineSeries>
#include <QScatterSeries>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QGuiApplication>
#include <QScreen>
#include <QLocale>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QBrush>
#include <QLinearGradient>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <thread>
#include <chrono>

// Helper utilities (local, file-scoped)
static QString formatCurrency(qint64 v) {
    return QLocale(QLocale::Vietnamese).toString(v);
}

// Constructor implementation
Bao_Cao_Thong_Ke::Bao_Cao_Thong_Ke(QWidget* parent)
    : QWidget(parent),
    currentSport("Bóng đá"),
    currentChartType(0),
    leftWidget(nullptr),
    rightWrapper(nullptr),
    serviceTable(nullptr),
    filterCombo(nullptr),
    totalLabel(nullptr),
    titleLabel(nullptr)
{
    this->setObjectName("root");


    // Load records from file to determine min/max date before creating widgets
    loadData(); // sets minDate and maxDate and populates records

    // date editors
    startDateEdit = new QDateEdit(minDate, this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");

    endDateEdit = new QDateEdit(maxDate, this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");

    connect(startDateEdit, &QDateEdit::dateChanged, this, &Bao_Cao_Thong_Ke::onDateRangeChanged);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &Bao_Cao_Thong_Ke::onDateRangeChanged);

    // Build UI
    setupUI();

    // Styling - keep compact but functional
    this->setStyleSheet(R"(
        QWidget#root { background-color: #3e4a59; font-family: "Segoe UI", "Arial"; color: #e0e0e0; font-size: 13px; }
        QWidget#leftPanel { background-color: transparent; border-right: 1px solid #4a637d; }
        QWidget#rightPanel { background-color: #34495e; border: 1px solid #4a637d; border-radius: 8px; }
        QLabel { color: #bdc3c7; padding: 2px; background-color: transparent; }
        QLabel#titleLabel { font: bold 18px; color: #ffffff; padding: 10px; }
        QChartView, QAbstractAxis, QLegend { color: #e0e0e0; background-color: transparent; }
        QLegendLabel { color: #e0e0e0; }
        QPushButton { background-color: #34495e; color: #e0e0e0; border: 1px solid #4a637d; border-radius: 5px; padding: 8px 16px; font-weight: bold; text-align: left; }
        QPushButton:hover { background-color: #4a637d; }
        QPushButton:checked { background-color: #0d6efd; color: #ffffff; border-color: #0d6efd; }
        QPushButton#exportBtn { background-color: #0d6efd; color: #ffffff; text-align: center; }
        QPushButton#exportBtn:hover { background-color: #0b5ed7; }
        QComboBox { background-color: #2c3e50; color: #e0e0e0; border: 1px solid #4a637d; padding: 5px; border-radius: 4px; }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView { background-color: #2c3e50; selection-background-color: #0d6efd; color: #e0e0e0; border: 1px solid #4a637d; }
        QDateEdit { background-color: #2c3e50; color: #e0e0e0; border: 1px solid #4a637d; border-radius: 4px; padding: 5px; }
        QTableWidget { background-color: #34495e; color: #e0e0e0; gridline-color: #4a637d; border: 1px solid #4a637d; border-radius: 5px; alternate-background-color: #3e4a59; }
        QHeaderView::section { background-color: #2c3e50; color: #ffffff; padding: 5px; border: 1px solid #4a637d; font-weight: bold; }
        QTableCornerButton::section { background-color: #2c3e50; }
        QScrollBar:vertical { border: none; background: #34495e; width: 10px; }
        QScrollBar::handle:vertical { background: #4a637d; min-height: 20px; border-radius: 5px; }
        QScrollBar::handle:vertical:hover { background: #5a738d; }
    )");

    // Build an initial chart/table
    updateChart();

    // Center window to a reasonable size
    if (QGuiApplication::primaryScreen()) {
        QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        int w = screenGeometry.width() * 2 / 3;
        int h = screenGeometry.height() * 2 / 3;
        resize(w, h);
        move((screenGeometry.width() - w) / 2, (screenGeometry.height() - h) / 2);
    }
}

// -----------------------------------------------------------------------------
// setupUI: build the left control panel and the right content container
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::setupUI()
{
    titleLabel = new QLabel(tr("📊 BÁO CÁO & THỐNG KÊ"));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    btnFootball = new QPushButton(tr("⚽ Bóng đá"), this);
    btnBasketball = new QPushButton(tr("🏀 Bóng rổ"), this);
    btnVolleyball = new QPushButton(tr("🏐 Bóng chuyền"), this);

    btnFootball->setCheckable(true);
    btnBasketball->setCheckable(true);
    btnVolleyball->setCheckable(true);
    btnFootball->setChecked(true);

    connect(btnFootball, &QPushButton::clicked, this, [=]() {
        currentSport = "Bóng đá";
        btnBasketball->setChecked(false);
        btnVolleyball->setChecked(false);
        updateChart();
    });
    connect(btnBasketball, &QPushButton::clicked, this, [=]() {
        currentSport = "Bóng rổ";
        btnFootball->setChecked(false);
        btnVolleyball->setChecked(false);
        updateChart();
    });
    connect(btnVolleyball, &QPushButton::clicked, this, [=]() {
        currentSport = "Bóng chuyền";
        btnFootball->setChecked(false);
        btnBasketball->setChecked(false);
        updateChart();
    });

    chartTypeCombo = new QComboBox(this);
    chartTypeCombo->addItems({
        tr("Biểu đồ Thống kê giờ sử dụng theo sân"),
        tr("Biểu đồ Tần suất đặt sân theo khung giờ"),
        tr("Biểu đồ Doanh thu"),
        tr("Bảng Thống kê dịch vụ đi kèm")
    });
    connect(chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Bao_Cao_Thong_Ke::onChartTypeChanged);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto* leftLay = new QVBoxLayout;
    leftLay->setContentsMargins(10, 10, 10, 10);
    leftLay->setSpacing(6);
    leftLay->addWidget(new QLabel(tr("Chọn môn:"), this));
    leftLay->addWidget(btnFootball);
    leftLay->addWidget(btnBasketball);
    leftLay->addWidget(btnVolleyball);
    leftLay->addSpacing(10);
    leftLay->addWidget(new QLabel(tr("Chọn loại thống kê:"), this));
    leftLay->addWidget(chartTypeCombo);
    leftLay->addSpacing(10);
    leftLay->addWidget(new QLabel(tr("Khoảng thời gian:"), this));
    leftLay->addWidget(new QLabel(tr("Từ:"), this));
    leftLay->addWidget(startDateEdit);
    leftLay->addWidget(new QLabel(tr("Đến:"), this));
    leftLay->addWidget(endDateEdit);
    leftLay->addStretch();

    auto* exportBtn = new QPushButton(tr("📁 Xuất CSV"), this);
    exportBtn->setObjectName("exportBtn");
    connect(exportBtn, &QPushButton::clicked, this, &Bao_Cao_Thong_Ke::exportToCSV);
    leftLay->addWidget(exportBtn);

    auto* mainLay = new QHBoxLayout(this);
    mainLay->setContentsMargins(0, 0, 0, 0);
    mainLay->setSpacing(0);

    leftWidget = new QWidget(this);
    leftWidget->setObjectName("leftPanel");
    leftWidget->setLayout(leftLay);
    leftWidget->setFixedWidth(220);
    mainLay->addWidget(leftWidget);

    rightWrapper = new QWidget(this);
    rightWrapper->setObjectName("rightPanel");
    auto* rightLay = new QVBoxLayout(rightWrapper);
    rightLay->setContentsMargins(10, 10, 10, 10);
    rightLay->setSpacing(6);
    rightLay->addWidget(titleLabel);
    rightLay->addWidget(chartView, 1);

    // Service filter UI for table - simplified to 5 main categories
    filterCombo = new QComboBox(rightWrapper);
    filterCombo->addItem(tr("Tất cả dịch vụ"));
    filterCombo->addItem(tr("Dịch vụ"));
    filterCombo->addItem(tr("Giày thi đấu"));
    filterCombo->addItem(tr("Đồ uống"));
    filterCombo->addItem(tr("Y tế"));
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Bao_Cao_Thong_Ke::updateChart);

    totalLabel = new QLabel(rightWrapper);
    totalLabel->setAlignment(Qt::AlignRight);
    totalLabel->setStyleSheet("font-weight:bold; color:#0d6efd;");

    rightLay->addWidget(filterCombo);
    rightLay->addWidget(totalLabel);

    mainLay->addWidget(rightWrapper, 1);

    setLayout(mainLay);
}

// -----------------------------------------------------------------------------
// updateChart: central function that either shows a table or a chart depending
// on currentChartType. It is defensive (checks pointers) and separates
// responsibilities into helper blocks for readability.
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::updateChart()
{
    // decide whether to show the service table or a chart
    bool showTable = (currentChartType == 3);

    if (chartView)
        chartView->setVisible(!showTable);

    // Hide/show filter controls based on chart type
    if (filterCombo)
        filterCombo->setVisible(showTable);
    if (totalLabel)
        totalLabel->setVisible(showTable);

    // Ensure rightWrapper exists
    QVBoxLayout* rightLay = qobject_cast<QVBoxLayout*>(rightWrapper ? rightWrapper->layout() : nullptr);

    // If we need to show the table, create it lazily and populate
    if (showTable) {
        if (!serviceTable) {
            serviceTable = new QTableWidget(rightWrapper);
            serviceTable->setColumnCount(4); // Loại, Tên, Số lượt, Doanh thu (removed mini chart)
            QStringList headers;
            headers << tr("Loại dịch vụ") << tr("Tên dịch vụ") << tr("Số lượt") << tr("Doanh thu (VND)");
            serviceTable->setHorizontalHeaderLabels(headers);
            serviceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            serviceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            serviceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
            serviceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
            serviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
            serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
            serviceTable->setAlternatingRowColors(true);
            if (rightLay) rightLay->addWidget(serviceTable, 1);
        }
        if (serviceTable) serviceTable->setVisible(true);

        // Read stats from the file. This is tolerant if file is missing or malformed.
        QMap<QString, QPair<int, qint64>> stats = readServiceStatsFromFile();

        // Compute summary values
        int totalCount = 0;
        qint64 totalRevenue = 0;
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            totalCount += it.value().first;
            totalRevenue += it.value().second;
        }

        // Fill table based on danhMucDichVu, applying filter
        serviceTable->setRowCount(0);
        int filterIdx = filterCombo ? filterCombo->currentIndex() : 0;

        // Determine which category to show based on filter
        QStringList categoriesToShow;
        switch (filterIdx) {
        case 0: // Tất cả dịch vụ
            categoriesToShow << "Dịch vụ" << "Giày thi đấu" << "Đồ uống" << "Y tế";
            break;
        case 1: // Dịch vụ
            categoriesToShow << "Dịch vụ";
            break;
        case 2: // Giày thi đấu
            categoriesToShow << "Giày thi đấu";
            break;
        case 3: // Đồ uống
            categoriesToShow << "Đồ uống";
            break;
        case 4: // Y tế
            categoriesToShow << "Y tế";
            break;
        }

        // Filtered summary values
        int filteredTotalCount = 0;
        qint64 filteredTotalRevenue = 0;

        for (const auto& group : danhMucDichVu) {
            const QString& loai = group.first;

            // Skip if this category is not in our filter
            if (!categoriesToShow.contains(loai)) continue;

            for (const auto& dv : group.second) {
                const QString& ten = dv.first;
                QString key = loai + "," + ten;
                int cnt = stats.contains(key) ? stats[key].first : 0;
                qint64 rev = stats.contains(key) ? stats[key].second : 0;

                // Add to filtered totals
                filteredTotalCount += cnt;
                filteredTotalRevenue += rev;

                int r = serviceTable->rowCount();
                serviceTable->insertRow(r);

                // create items first
                auto* itemLoai = new QTableWidgetItem(loai);
                auto* itemTen = new QTableWidgetItem(ten);
                auto* itemCnt = new QTableWidgetItem(QString::number(cnt));
                auto* itemRev = new QTableWidgetItem(formatCurrency(rev));
                itemCnt->setTextAlignment(Qt::AlignCenter);
                itemRev->setTextAlignment(Qt::AlignRight);

                // set items into table
                serviceTable->setItem(r, 0, itemLoai);
                serviceTable->setItem(r, 1, itemTen);
                serviceTable->setItem(r, 2, itemCnt);
                serviceTable->setItem(r, 3, itemRev);

                // No highlighting since we removed the "top performer" feature
            }
        }

        if (totalLabel) {
            // Show filtered totals when a specific category is selected
            if (filterIdx == 0) {
                totalLabel->setText(QString("Tổng lượt: <b>%1</b> | Tổng doanh thu: <b>%2</b>")
                                        .arg(totalCount)
                                        .arg(formatCurrency(totalRevenue)));
            }
            else {
                QString categoryName = filterCombo->currentText();
                totalLabel->setText(QString("%1 - Tổng lượt: <b>%2</b> | Tổng doanh thu: <b>%3</b>")
                                        .arg(categoryName)
                                        .arg(filteredTotalCount)
                                        .arg(formatCurrency(filteredTotalRevenue)));
            }
        }

        return; // done with table branch
    }

    // hide the service table when not in table mode
    if (serviceTable && !showTable) {
        serviceTable->setVisible(false);
    }

    // Continue with drawing charts (bar/line/pie)
    if (chartView == nullptr) return; // nothing to draw onto

    QChart* chart = new QChart;
    chart->setBackgroundBrush(QBrush(QColor("#34495e")));
    chart->setTitleBrush(QBrush(QColor("#ffffff")));
    chart->setTitleFont(QFont("Segoe UI", 15, QFont::Bold));
    chart->setTitle(chartTypeCombo ? chartTypeCombo->currentText() : QString());
    chart->legend()->setLabelColor(QColor("#e0e0e0"));

    if (currentChartType == 0) {
        // 1️⃣ Bar chart - total hours per field with separated individual bars
        QMap<QString, double> hours;
        for (const auto& r : records) {
            if (r.sport != currentSport) continue;
            if (r.date < startDateEdit->date() || r.date > endDateEdit->date()) continue;
            double h = r.timeStart.secsTo(r.timeEnd) / 3600.0;
            hours[r.fieldCode] += h;
        }

        if (hours.isEmpty()) {
            chart->addSeries(new QLineSeries); // placeholder
            chartView->setChart(chart);
            chartView->setToolTip(tr("Không có dữ liệu cho khoảng thời gian và môn đã chọn."));
            return;
        }

        // Find max value for highlighting
        double maxHour = 0.0;
        QString maxField;
        for (auto it = hours.begin(); it != hours.end(); ++it) {
            if (it.value() > maxHour) {
                maxHour = it.value();
                maxField = it.key();
            }
        }

        // Create ONE BarSet with ALL values for separated bars
        QBarSet* set = new QBarSet(tr("Giờ sử dụng"));
        QStringList categories;

        for (auto it = hours.begin(); it != hours.end(); ++it) {
            categories << it.key();
            *set << it.value();
        }

        // Apply gradient color to the whole set
        QLinearGradient grad(0, 0, 1, 1);
        grad.setCoordinateMode(QGradient::ObjectBoundingMode);
        grad.setColorAt(0.0, QColor("#0d6efd"));
        grad.setColorAt(1.0, QColor("#4a637d"));
        set->setBrush(QBrush(grad));

        // Create series and add the single set
        QBarSeries* series = new QBarSeries;
        series->append(set);
        chart->addSeries(series);

        // Tooltip on hover for the entire series
        QObject::connect(set, &QBarSet::hovered, this, [chart, hours, categories](bool status, int index) {
            if (status && index >= 0 && index < categories.size()) {
                QString fieldCode = categories[index];
                double value = hours[fieldCode];
                chart->setToolTip(QString("%1: %2 giờ").arg(fieldCode).arg(value, 0, 'f', 1));
            }
            else {
                chart->setToolTip("");
            }
        });

        // X Axis
        QBarCategoryAxis* axisX = new QBarCategoryAxis;
        axisX->append(categories);
        axisX->setLabelsColor(QColor("#e0e0e0"));
        if (categories.size() > 6) axisX->setLabelsAngle(-30);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Y Axis
        QValueAxis* axisY = new QValueAxis;
        axisY->setTitleText(tr("Tổng giờ sử dụng"));
        axisY->setLabelsColor(QColor("#e0e0e0"));
        axisY->setTitleBrush(QBrush(QColor("#e0e0e0")));
        axisY->setRange(0, maxHour > 0 ? maxHour * 1.2 : 1);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chartView->setChart(chart);

        // Add value labels on top of bars after chart is rendered (with delay)
        std::thread([this, chart, set, categories, maxField]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // delay 10ms

            // Về GUI thread
            QMetaObject::invokeMethod(this, [this, chart, set, categories, maxField]() {
                if (!chart || !chart->scene()) return;

                // Clear old labels first
                auto items = chart->scene()->items();
                for (auto* item : items) {
                    if (auto* textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item)) {
                        chart->scene()->removeItem(textItem);
                        delete textItem;
                    }
                }

                QValueAxis* axisY = nullptr;
                for (auto* ax : chart->axes(Qt::Vertical)) {
                    axisY = qobject_cast<QValueAxis*>(ax);
                    if (axisY) break;
                }
                if (!axisY) return;

                for (int i = 0; i < categories.size(); ++i) {
                    double value = set->at(i);
                    QString fieldCode = categories[i];

                    QGraphicsSimpleTextItem* label = new QGraphicsSimpleTextItem(QString::number(value, 'f', 1));
                    label->setBrush(fieldCode == maxField ? QBrush(Qt::yellow) : QBrush(Qt::white));
                    label->setFont(QFont("Segoe UI", 10, QFont::Bold));
                    chart->scene()->addItem(label);

                    // Position label correctly on top of bar
                    QRectF plotArea = chart->plotArea();
                    double barWidth = plotArea.width() / categories.size();
                    double x = plotArea.left() + i * barWidth + barWidth / 2 - label->boundingRect().width() / 2;
                    double yRatio = value / axisY->max();
                    double y = plotArea.bottom() - (plotArea.height() * yRatio) - label->boundingRect().height() - 5;
                    label->setPos(x, y);
                }
            });
        }).detach();
    }
    else if (currentChartType == 1) {
        // 2️⃣ Line chart - booking frequency per hour slot with markers and peak highlighting
        QMap<int, int> hourCount;
        for (const auto& r : records) {
            if (r.sport != currentSport) continue;
            if (r.date < startDateEdit->date() || r.date > endDateEdit->date()) continue;
            QTime t = r.timeStart;
            while (t < r.timeEnd) {
                int h = t.hour();
                if (h >= 6 && h < 22) hourCount[h]++;
                t = t.addSecs(3600);
            }
        }

        QLineSeries* lineSeries = new QLineSeries;
        QScatterSeries* scatterSeries = new QScatterSeries;

        // Line series configuration
        lineSeries->setColor(QColor("#0d6efd"));
        lineSeries->setName(tr("Tần suất đặt sân"));
        QPen linePen(QColor("#0d6efd"), 3);
        lineSeries->setPen(linePen);

        // Scatter series for markers
        scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        scatterSeries->setMarkerSize(12.0); // Slightly bigger for easier clicking
        scatterSeries->setColor(QColor("#ff9800"));
        scatterSeries->setBorderColor(QColor("#f57c00"));
        scatterSeries->setName(tr("Điểm dữ liệu"));

        int maxCnt = 0;
        int peakHour = 6; // Track the hour with highest booking count
        for (int h = 6; h < 22; ++h) {
            int cnt = hourCount.value(h, 0);
            lineSeries->append(h, cnt);
            scatterSeries->append(h, cnt);
            if (cnt > maxCnt) {
                maxCnt = cnt;
                peakHour = h;
            }
        }

        chart->addSeries(lineSeries);
        chart->addSeries(scatterSeries);

        // Add click functionality for scatter series (more reliable than hover)
        connect(scatterSeries, &QScatterSeries::clicked, this, [chart, hourCount](const QPointF& point) {
            int hour = static_cast<int>(qRound(point.x()));
            int count = hourCount.value(hour, 0);
            QString tooltip = QString("🎯 Khung giờ %1:00\nSố lượt đặt: %2 lượt\n(Click điểm khác để xem thêm)")
                                  .arg(hour, 2, 10, QChar('0'))
                                  .arg(count);
            chart->setToolTip(tooltip);

            // Show tooltip persistently
            QToolTip::showText(QCursor::pos(), tooltip);
        });

        // Add hover functionality as backup (may work on some systems)
        connect(scatterSeries, &QScatterSeries::hovered, this, [chart, hourCount](const QPointF& point, bool state) {
            if (state) {
                int hour = static_cast<int>(qRound(point.x()));
                int count = hourCount.value(hour, 0);
                QString tooltip = QString("📍 %1:00 - %2 lượt")
                                      .arg(hour, 2, 10, QChar('0'))
                                      .arg(count);
                chart->setToolTip(tooltip);
            } else {
                chart->setToolTip("");
            }
        });

        // X Axis
        QValueAxis* axisX = new QValueAxis;
        axisX->setTitleText(tr("Khung giờ"));
        axisX->setLabelsColor(QColor("#e0e0e0"));
        axisX->setTitleBrush(QBrush(QColor("#e0e0e0")));
        axisX->setRange(6, 21);
        axisX->setTickCount(16);
        axisX->setLabelFormat("%d:00");
        chart->addAxis(axisX, Qt::AlignBottom);
        lineSeries->attachAxis(axisX);
        scatterSeries->attachAxis(axisX);

        // Y Axis - Improved to show integer values properly
        QValueAxis* axisY = new QValueAxis;
        axisY->setTitleText(tr("Số lượt đặt"));
        axisY->setLabelsColor(QColor("#e0e0e0"));
        axisY->setTitleBrush(QBrush(QColor("#e0e0e0")));

        // Set proper integer formatting and tick intervals
        axisY->setLabelFormat("%.0f");

        // Calculate appropriate range and tick count
        int yMax = maxCnt > 0 ? maxCnt + 1 : 4; // Add 1 for better visualization, minimum 4
        axisY->setRange(0, yMax);

        // Set tick count to ensure integer steps
        if (yMax <= 5) {
            axisY->setTickCount(yMax + 1); // This ensures we get 0, 1, 2, 3, 4, 5
        } else {
            axisY->setTickCount(6); // Maximum 6 ticks for readability
        }

        // Force integer intervals by setting minor tick count
        axisY->setMinorTickCount(0);

        chart->addAxis(axisY, Qt::AlignLeft);
        lineSeries->attachAxis(axisY);
        scatterSeries->attachAxis(axisY);

        chartView->setChart(chart);

        // Add peak value label at the highest point (only if maxCnt > 0)
        if (maxCnt > 0) {
            // Force chart to layout and render properly before adding labels
            chartView->repaint();
            QApplication::processEvents();

            // Create label for peak value
            QString peakText = QString("Đỉnh: %1 lượt\n%2:00")
                                   .arg(maxCnt)
                                   .arg(peakHour);

            QGraphicsSimpleTextItem* peakLabel = new QGraphicsSimpleTextItem(peakText);
            peakLabel->setBrush(QBrush(QColor("#ff9800"))); // Orange color to match scatter points
            peakLabel->setFont(QFont("Segoe UI", 9, QFont::Bold));

            // Add subtle background for better readability
            QRectF textBounds = peakLabel->boundingRect();
            QGraphicsRectItem* backgroundRect = new QGraphicsRectItem(
                textBounds.adjusted(-3, -2, 3, 2)
                );
            backgroundRect->setBrush(QBrush(QColor(0, 0, 0, 150)));
            backgroundRect->setPen(QPen(QColor("#ff9800"), 1));
            backgroundRect->setParentItem(peakLabel);

            chart->scene()->addItem(peakLabel);

            // Position label above the peak point
            QRectF plotArea = chart->plotArea();

            // Calculate X position based on peak hour
            double xRatio = (double)(peakHour - 6) / (21 - 6); // Normalize to 0-1 range
            double x = plotArea.left() + (plotArea.width() * xRatio) - textBounds.width() / 2;

            // Calculate Y position based on peak count
            double yRatio = (double)maxCnt / yMax;
            double y = plotArea.bottom() - (plotArea.height() * yRatio) - textBounds.height() - 10;

            // Ensure label stays within plot area
            x = qMax(x, plotArea.left() + 5);
            x = qMin(x, plotArea.right() - textBounds.width() - 5);
            y = qMax(y, plotArea.top() + 5);

            peakLabel->setPos(x, y);
        }

        // Background highlight for peak hours (18-20)
        int peakCount = hourCount.value(18, 0) + hourCount.value(19, 0) + hourCount.value(20, 0);
        if (peakCount > 0) {
            chart->setPlotAreaBackgroundVisible(true);
            chart->setPlotAreaBackgroundBrush(QBrush(QColor(255, 152, 0, 30)));
        }
    }
    else if (currentChartType == 2) {
        // 3️⃣ Pie chart - revenue per field with exploded max slice and clean legend
        QMap<QString, double> rev;
        for (const auto& r : records) {
            if (r.sport != currentSport) continue;
            if (r.date < startDateEdit->date() || r.date > endDateEdit->date()) continue;
            rev[r.fieldCode] += r.money;
        }

        if (rev.isEmpty()) {
            chart->addSeries(new QPieSeries);
            chartView->setChart(chart);
            if (chartView) {
                chartView->setToolTip(tr("Không có doanh thu cho lựa chọn hiện tại."));
            }
            return;
        }

        QPieSeries* pieSeries = new QPieSeries;
        double total = 0.0;
        double maxVal = 0.0;
        int maxIdx = -1;
        int idx = 0;

        for (auto it = rev.begin(); it != rev.end(); ++it, ++idx) {
            pieSeries->append(it.key(), it.value());
            total += it.value();
            if (it.value() > maxVal) {
                maxVal = it.value();
                maxIdx = idx;
            }
        }

        // Color scheme with contrasting colors
        QVector<QColor> colors = {
            QColor("#0d6efd"), QColor("#ff9800"), QColor("#4a637d"),
            QColor("#e91e63"), QColor("#43a047"), QColor("#f44336"),
            QColor("#00bcd4"), QColor("#8e24aa"), QColor("#795548")
        };

        int idx2 = 0;
        for (int i = 0; i < pieSeries->count(); ++i, ++idx2) {
            QPieSlice* slice = pieSeries->slices().at(i);
            if (!slice) continue;

            slice->setBrush(QBrush(colors[i % colors.size()]));

            // Lấy tên sân (fieldCode)
            QString fieldName = rev.keys().at(i);

            // Tính phần trăm
            double percentage = (slice->value() / total) * 100.0;

            // Format label: "Tên sân: XX% (XXXXXX vnđ)"
            QString sliceLabel = QString("%1: %2% (%3)")
                                     .arg(fieldName)
                                     .arg(QString::number(percentage, 'f', 1))
                                     .arg(formatCurrency(static_cast<qint64>(slice->value())));

            slice->setLabel(sliceLabel);
            slice->setLabelVisible(true);

            if (percentage >= 5.0) {
                slice->setLabelPosition(QPieSlice::LabelOutside);
                slice->setLabelBrush(QBrush(QColor("#ffffff")));
                slice->setLabelFont(QFont("Segoe UI", 8, QFont::Bold));
            }
            else {
                slice->setLabelPosition(QPieSlice::LabelInsideNormal);
                slice->setLabelBrush(QBrush(QColor("#000000")));
                slice->setLabelFont(QFont("Segoe UI", 7, QFont::Bold));
            }

            // Hiệu ứng cho lát lớn nhất
            if (i == maxIdx) {
                slice->setExploded(true);
                slice->setExplodeDistanceFactor(0.1);
                slice->setPen(QPen(QColor("#ff9800"), 3));
                slice->setLabelBrush(QBrush(QColor("#ff9800")));
            }
        }

        // Set label arm length to reduce overlapping
        pieSeries->setLabelsPosition(QPieSlice::LabelOutside);

        // Use horizontal label position for better spacing
        pieSeries->setHorizontalPosition(0.5);
        pieSeries->setVerticalPosition(0.45); // Slightly higher to make room for total label

        chart->addSeries(pieSeries);

        if (chartView) {
            chartView->setChart(chart);
        }

        // Display total revenue in bottom right corner with smaller font
        std::thread([this, chart, total]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // delay 10ms

            // Vì đang ở thread khác, cần về thread GUI:
            QMetaObject::invokeMethod(this, [this, chart, total]() {
                if (!chart || !chart->scene()) return;

                // Clear old total revenue labels
                auto items = chart->scene()->items();
                for (auto* item : items) {
                    if (auto* textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item)) {
                        if (textItem->text().contains("Tổng:")) {
                            chart->scene()->removeItem(textItem);
                            delete textItem;
                        }
                    }
                }

                QString totalText = QString("Tổng: %1")
                                        .arg(formatCurrency(static_cast<qint64>(total)));

                QGraphicsSimpleTextItem* totalLabelItem = new QGraphicsSimpleTextItem(totalText);
                totalLabelItem->setBrush(QBrush(QColor("#ffffff")));
                totalLabelItem->setFont(QFont("Segoe UI", 8, QFont::Normal));

                QRectF textBounds = totalLabelItem->boundingRect();
                QGraphicsRectItem* backgroundRect = new QGraphicsRectItem(
                    textBounds.adjusted(-4, -2, 4, 2)
                    );
                if (backgroundRect) {
                    backgroundRect->setBrush(QBrush(QColor(0, 0, 0, 120)));
                    backgroundRect->setPen(QPen(QColor("#4a637d"), 1));
                    backgroundRect->setParentItem(totalLabelItem);
                }

                chart->scene()->addItem(totalLabelItem);

                QRectF plotArea = chart->plotArea();
                double rightMargin = 15;
                double bottomMargin = 15;

                double x = plotArea.right() - textBounds.width() - rightMargin;
                double y = plotArea.bottom() - textBounds.height() - bottomMargin;

                x = qMax(x, plotArea.left() + 10);
                y = qMax(y, plotArea.top() + 10);

                totalLabelItem->setPos(x, y);
            });
        }).detach();
    }

    chartView->setChart(chart);
}
// -----------------------------------------------------------------------------
// readServiceStatsFromFile: robust parser for the service statistics file. The
// expected format per line is: Loai,Tên,Số lượt,Doanh thu
// return map keyed by "Loai,Tên" -> (count, revenue)
// -----------------------------------------------------------------------------
QMap<QString, QPair<int, qint64>> Bao_Cao_Thong_Ke::readServiceStatsFromFile()
{
    QMap<QString, QPair<int, qint64>> stats;
    QString path = QCoreApplication::applicationDirPath() + "/data/DichVu_ThongKe.txt";
    QFile f(path);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Warning: could not open" << path;
        return stats;
    }

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto cols = line.split(',', Qt::KeepEmptyParts);
        if (cols.size() < 4) continue;
        QString loai = cols[0].trimmed();
        QString ten = cols[1].trimmed();
        int count = cols[2].trimmed().toInt();
        QString num = cols[3].trimmed();
        num.remove('.'); num.remove(','); num.remove('\n'); num.remove('\r');
        qint64 revenue = num.toLongLong();
        QString key = loai + "," + ten;
        stats[key] = qMakePair(count, revenue);
    }
    f.close();
    return stats;
}

// -----------------------------------------------------------------------------
// onChartTypeChanged - slot
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::onChartTypeChanged(int index)
{
    currentChartType = index;
    updateChart();
}

// -----------------------------------------------------------------------------
// sportToName - helper to convert English sport names (if present) into
// localized Vietnamese names used in the data.
// -----------------------------------------------------------------------------
QString Bao_Cao_Thong_Ke::sportToName(const QString& sport) const
{
    if (sport == "Football")   return "Bóng đá";
    if (sport == "Basketball") return "Bóng rổ";
    if (sport == "Volleyball") return "Bóng chuyền";
    return sport;
}

// -----------------------------------------------------------------------------
// parseLine - parse a booking record from a single line in data.txt. Expected
// format: Sport-FieldCode-dd/MM/yyyy-HH:mm-HH:mm-Customer-Phone(-Money)
// If parse fails, the returned BookingRecord will have empty sport and should be
// ignored by callers.
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::parseLine(const QString& line, BookingRecord& rec)
{
    auto parts = line.split('-', Qt::SkipEmptyParts);
    if (parts.size() < 7) {
        rec.sport.clear();
        return;
    }

    rec.sport = sportToName(parts[0].trimmed());
    rec.fieldCode = parts[1].trimmed();
    rec.date = QDate::fromString(parts[2].trimmed(), "dd/MM/yyyy");
    rec.timeStart = QTime::fromString(parts[3].trimmed(), "HH:mm");
    rec.timeEnd = QTime::fromString(parts[4].trimmed(), "HH:mm");
    rec.customerName = parts[5].trimmed();
    rec.phone = parts[6].trimmed();
    if (parts.size() >= 8) {
        // money may contain thousand separators
        QString m = parts[7].trimmed();
        m.remove('.'); m.remove(',');
        rec.money = m.toDouble();
    }
    else {
        rec.money = 0.0;
    }
}

// -----------------------------------------------------------------------------
// onDateRangeChanged - slot used by both date edits
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::onDateRangeChanged()
{
    if (startDateEdit->date() > endDateEdit->date())
        endDateEdit->setDate(startDateEdit->date());
    updateChart();
}

// -----------------------------------------------------------------------------
// loadData - reads booking records from data/data.txt (or data.txt fallback).
// Fills `records` vector and computes minDate, maxDate.
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::loadData()
{
    records.clear();
    QString path = QCoreApplication::applicationDirPath() + "/data/data.txt";
    QFile f(path);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDate today = QDate::currentDate();
        minDate = today.addDays(-7);
        maxDate = today;
        qDebug() << "Warning: could not open booking data file" << f.fileName();
        return;
    }

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        BookingRecord rec;
        parseLine(line, rec);
        if (!rec.sport.isEmpty()) records.append(rec);
    }
    f.close();

    if (records.isEmpty()) {
        QDate today = QDate::currentDate();
        minDate = today.addDays(-7);
        maxDate = today;
    }
    else {
        minDate = records.first().date;
        maxDate = records.first().date;
        for (const auto& r : records) {
            minDate = qMin(minDate, r.date);
            maxDate = qMax(maxDate, r.date);
        }
    }
}

// -----------------------------------------------------------------------------
// exportToCSV - export the current view's data into a CSV file. It enforces a
// minimum date range of 7 days (keeps the original behavior you had).
// -----------------------------------------------------------------------------
void Bao_Cao_Thong_Ke::exportToCSV()
{
    int days = startDateEdit->date().daysTo(endDateEdit->date());
    if (days < 7) {
        QMessageBox::warning(this,
                             tr("Không thể xuất CSV"),
                             tr("Vui lòng chọn ít nhất 7 ngày (hiện tại %1 ngày).").arg(days));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Xuất báo cáo"), "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Lỗi"), tr("Không thể mở file để ghi."));
        return;
    }

    file.write("\xEF\xBB\xBF"); // BOM for UTF-8
    QTextStream out(&file);

    QString from = startDateEdit->date().toString("dd/MM/yyyy");
    QString to = endDateEdit->date().toString("dd/MM/yyyy");

    if (currentChartType == 0) {
        out << QString("--- BẢNG 1: GIỜ SỬ DỤNG THEO SÂN TỪ NGÀY %1 - %2 ---\n").arg(from, to);
        out << "Môn,Mã sân,Tổng giờ\n";
        QMap<QString, double> hours;
        for (auto& r : records) {
            if (r.sport != currentSport)        continue;
            if (r.date < startDateEdit->date())  continue;
            if (r.date > endDateEdit->date())    continue;
            hours[r.fieldCode] += r.timeStart.secsTo(r.timeEnd) / 3600.0;
        }
        for (auto it = hours.begin(); it != hours.end(); ++it)
            out << currentSport << "," << it.key() << "," << QString::number(it.value(), 'f', 1) << "\n";
    }
    else if (currentChartType == 1) {
        out << QString("--- BẢNG 2: TẦN SUẤT THEO GIỜ TỪ NGÀY %1 - %2 ---\n").arg(from, to);
        out << "Môn,Khung giờ,Số lượt đặt\n";
        QMap<int, int> hourCount;
        for (auto& r : records) {
            if (r.sport != currentSport)        continue;
            if (r.date < startDateEdit->date())  continue;
            if (r.date > endDateEdit->date())    continue;
            QTime t = r.timeStart;
            while (t < r.timeEnd) {
                int h = t.hour();
                if (h >= 6 && h < 22) hourCount[h]++;
                t = t.addSecs(3600);
            }
        }
        for (int h = 6; h < 22; ++h) {
            QString hourLabel = QString("%1:00").arg(h, 2, 10, QChar('0'));
            int cnt = hourCount.value(h, 0);
            out << currentSport << ","
                << hourLabel << ","
                << cnt << "\n";
        }
    }
    else if (currentChartType == 2) {
        out << QString("--- BẢNG 3: DOANH THU THEO SÂN TỪ NGÀY %1 - %2 ---\n").arg(from, to);
        out << "Môn,Mã sân,Doanh thu (nghìn VND)\n";
        QMap<QString, double> rev;
        for (auto& r : records) {
            if (r.sport != currentSport)        continue;
            if (r.date < startDateEdit->date())  continue;
            if (r.date > endDateEdit->date())    continue;
            rev[r.fieldCode] += r.money;
        }
        for (auto it = rev.begin(); it != rev.end(); ++it)
            out << currentSport << "," << it.key() << "," << QString::number(it.value(), 'f', 0) << "\n";
    }
    else {
        out << QString("--- BẢNG 4: THỐNG KÊ DỊCH VỤ ĐI KÈM TỪ NGÀY %1 - %2 ---\n")
                   .arg(from, to);
        out << "Loại dịch vụ,Tên dịch vụ,Số lượt sử dụng,Doanh thu (nghìn VND)\n";
        QMap<QString, QPair<int, qint64>> stats = readServiceStatsFromFile();
        for (const auto& group : danhMucDichVu) {
            const QString& loai = group.first;
            for (const auto& dv : group.second) {
                const QString& ten = dv.first;
                QString key = loai + "," + ten;
                int cnt = stats.contains(key) ? stats[key].first : 0;
                qint64 rev = stats.contains(key) ? stats[key].second : 0;
                out << loai << "," << ten << "," << cnt << "," << formatCurrency(rev) << "\n";
            }
        }
    }

    file.close();
    QMessageBox::information(this, tr("Hoàn tất"), tr("Xuất báo cáo thành công."));
}
