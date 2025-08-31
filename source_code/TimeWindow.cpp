#include "TimeWindow.h"
#include "BookingInfoWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QListView>
#include <QTime>
#include <QFile>
#include <QStandardItemModel>
#include <fstream>
#include <string>
#include <QCoreApplication>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

using namespace std;

TimeWindow::TimeWindow(const QString& sport, const QString& field, const QDate& date, QWidget* parent)
    : QWidget(parent), selectedDate(date), selectedField(field), selectedSport(sport) {

    setWindowTitle("Chọn giờ đặt sân");
    setFixedSize(300, 250);

    QLabel* noteLabel = new QLabel("Lưu ý:\n- Các khung giờ mờ đã được đặt.\n- Bạn chỉ có thể chọn những khung giờ còn trống.\n- Sân phải được đặt ít nhất 1 tiếng và sớm nhất 2 tiếng.");
    noteLabel->setStyleSheet("color: #d32f2f; font-style: italic; font-size: 12px;");
    noteLabel->setWordWrap(true);

    startCombo = new QComboBox;
    endCombo = new QComboBox;
    confirmButton = new QPushButton("Xác nhận");
    confirmButton->setObjectName("confirmButton");
    backButton = new QPushButton("← Quay lại");
    backButton->setObjectName("backButton");

    loadBookedSlots();

    updateStartCombo();

    startCombo->setView(new QListView());
    startCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    endCombo->setView(new QListView());
    endCombo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto* startLayout = new QHBoxLayout;
    startLayout->addWidget(new QLabel("Bắt đầu:"));
    startLayout->addWidget(startCombo);

    auto* endLayout = new QHBoxLayout;
    endLayout->addWidget(new QLabel("Kết thúc:"));
    endLayout->addWidget(endCombo);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(noteLabel);
    mainLayout->addLayout(startLayout);
    mainLayout->addLayout(endLayout);
    auto* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(backButton, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton, 0, Qt::AlignRight);

    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        /* === Main Widget Background === */
        QWidget {
            background-color: #3e4a59;
        }

        /* === Labels === */
        QLabel {
            background-color: transparent;
            color: #bdc3c7;
            font-size: 13px;
            font-weight: 500;
        }

        /* === Note Label Specific Styling (Override) === */
        QLabel[styleSheet~="color: #d32f2f"] {
            color: #e74c3c; /* Adjusted to fit dark theme */
            font-style: italic;
            font-size: 12px;
        }

        /* === Combo Boxes === */
        QComboBox {
            background-color: #3e4a59;
            border: 1px solid #4a637d;
            border-radius: 4px;
            padding: 0 12px;
            font-size: 14px;
            color: #e0e0e0;
            min-height: 40px;
        }
        QComboBox:focus {
            border-color: #0d6efd;
        }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView {
            border: 1px solid #4a637d;
            background-color: #3e4a59;
            selection-background-color: #0d6efd;
            color: #e0e0e0;
            padding: 4px;
        }

        /* === Button Base Style === */
        QPushButton {
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
        }

        /* === Primary Action Button (Confirm) === */
        #confirmButton {
            background-color: #0d6efd;
            color: white;
        }
        #confirmButton:hover {
            background-color: #0b5ed7;
        }

        /* === Secondary Button (Back) === */
        #backButton {
            background-color: #34495e;
            color: #e0e0e0;
            border: 1px solid #4a637d;
        }
        #backButton:hover {
            background-color: #4a637d;
        }
    )");

    connect(startCombo, &QComboBox::currentTextChanged, this, &TimeWindow::updateEndCombo);
    connect(confirmButton, &QPushButton::clicked, this, &TimeWindow::onConfirm);
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToDateSelection();
        this->close();
    });
}

QStringList TimeWindow::generateTimeList(const QTime& minTime, const QTime& maxTime) {
    QStringList times;
    for (int h = 5; h <= 22; h++) {
        for (int m = 0; m <= 30; m += 30) {
            QTime t(h, m);
            if (t >= minTime && t <= maxTime) {
                times << t.toString("hh:mm");
            }
        }
    }
    return times;
}

void TimeWindow::loadBookedSlots() {
    string filePath = QCoreApplication::applicationDirPath().toStdString() + "/data/booking.txt";
    ifstream file(filePath);
    string line;

    while (getline(file, line)) {
        QStringList parts = QString::fromStdString(line).split('-');
        if (parts.size() < 5) continue;

        QString sportName = parts[0].trimmed();
        QString fieldName = parts[1].trimmed();

        if (sportName != selectedSport || fieldName != selectedField) continue;

        QDate d = QDate::fromString(parts[2], "dd/MM/yyyy");
        QTime s = QTime::fromString(parts[3], "hh:mm");
        QTime e = QTime::fromString(parts[4], "hh:mm");

        if (d == selectedDate && s.isValid() && e.isValid()) {
            bookedSlots.append({s, e});
        }
    }

    file.close();
}

bool TimeWindow::isBooked(const QTime& t) const {
    for (const auto& slot : bookedSlots) {
        if (t >= slot.first && t < slot.second) return true;
    }
    return false;
}

void TimeWindow::updateStartCombo() {
    QTime minStartTime = QTime(5, 0);
    if (selectedDate == QDate::currentDate()) {
        minStartTime = QTime::currentTime().addSecs(7200);
    }

    startCombo->clear();
    auto* model = new QStandardItemModel(this);

    for (int h = 5; h <= 21; ++h) {
        for (int m = 0; m <= 30; m += 30) {
            if (h == 21 && m == 30) continue;
            QTime t(h, m);
            QString label = t.toString("hh:mm");
            auto* item = new QStandardItem(label);

            if (t < minStartTime || isBooked(t) || isBooked(t.addSecs(1800))) {
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
                item->setForeground(QBrush(Qt::gray));
            }

            model->appendRow(item);
        }
    }

    startCombo->setModel(model);

    // Tự chọn dòng đầu tiên hợp lệ
    for (int i = 0; i < model->rowCount(); i++) {
        if (model->item(i)->isEnabled()) {
            startCombo->setCurrentIndex(i);
            updateEndCombo(startCombo->currentText());
            break;
        }
    }
}


void TimeWindow::updateEndCombo(const QString& startTimeStr) {
    QTime startTime = QTime::fromString(startTimeStr, "hh:mm");
    QTime minEndTime = startTime.addSecs(3600);

    endCombo->clear();
    QStringList validTimes;

    for (int h = 5; h <= 22; h++) {
        for (int m = 0; m <= 30; m += 30) {
            QTime t(h, m);
            if (t >= minEndTime && t <= QTime(22, 0)) {
                bool conflict = false;

                for (const auto& slot : bookedSlots) {
                    if (t > slot.first && startTime < slot.first) {
                        conflict = true;
                        break;
                    }
                }

                if (!conflict) {
                    validTimes << t.toString("hh:mm");
                }
            }
        }
    }

    endCombo->addItems(validTimes);
}

tuple<int, int, int, int, int> extractDateTime(const string& line) {
    // Tìm vị trí phần ngày sau trường thứ hai (San1)
    size_t pos1 = line.find('-', 0); // Sau tên môn thể thao
    if (pos1 == string::npos) return {0, 0, 0, 0, 0};

    size_t pos2 = line.find('-', pos1 + 1); // Sau tên sân
    if (pos2 == string::npos) return {0, 0, 0, 0, 0};

    // Ngày bắt đầu sau pos2
    size_t posDate = pos2 + 1;
    if (posDate + 10 > line.size()) return {0, 0, 0, 0, 0}; // ngày: dd/MM/yyyy

    int d = stoi(line.substr(posDate, 2));
    int m = stoi(line.substr(posDate + 3, 2));
    int y = stoi(line.substr(posDate + 6, 4));

    // Giờ bắt đầu là sau phần ngày + 11 (có dấu '-')
    size_t posTime = posDate + 11;
    if (posTime + 5 > line.size()) return {0, 0, 0, 0, 0}; // giờ: hh:mm

    int h = stoi(line.substr(posTime, 2));
    int min = stoi(line.substr(posTime + 3, 2));

    return {y, m, d, h, min};
}

void sortBookingFile(const string& filePath) {
    ifstream fin(filePath);
    if (!fin.is_open()) return;

    vector<string> lines;
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    fin.close();

    sort(lines.begin(), lines.end(), [](const string& a, const string& b) {
        return extractDateTime(a) < extractDateTime(b);
    });

    ofstream fout(filePath, ios::trunc);
    for (const auto& l : lines) {
        fout << l << '\n';
    }
    fout.close();
}

int calculateTotalPriceFromRateFile(const QString& sport, const QString& field, const QTime& startTime, const QTime& endTime) {
    QString filePath = QCoreApplication::applicationDirPath() + "/data/price/" + sport + "/" + TimeWindow::removeAccents(field).replace(" ", "_") + ".txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    QTextStream in(&file);
    int total = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split('-');
        if (parts.size() != 3) continue;

        QTime slotStart = QTime::fromString(parts[0].trimmed(), "hh:mm");
        QTime slotEnd   = QTime::fromString(parts[1].trimmed(), "hh:mm");
        int pricePerHour = parts[2].trimmed().toInt();

        if (!slotStart.isValid() || !slotEnd.isValid()) continue;
        if (slotEnd <= startTime || slotStart >= endTime) continue;

        // Tính phần giao nhau giữa thời gian đặt và slot giá
        QTime overlapStart = (slotStart > startTime) ? slotStart : startTime;
        QTime overlapEnd   = (slotEnd < endTime)   ? slotEnd   : endTime;

        int overlapMinutes = overlapStart.secsTo(overlapEnd) / 60;
        if (overlapMinutes <= 0) continue;

        double hours = static_cast<double>(overlapMinutes) / 60.0;
        total += static_cast<int>(hours * pricePerHour);
    }

    file.close();
    return total;
}

void TimeWindow::onConfirm() {
    QString start = startCombo->currentText();
    QString end = endCombo->currentText();

    if (start.isEmpty() || end.isEmpty())
        return;

    QTime startTime = QTime::fromString(start, "hh:mm");
    QTime endTime = QTime::fromString(end, "hh:mm");

    int totalPrice = calculateTotalPriceFromRateFile(selectedSport, selectedField, startTime, endTime);
    if (totalPrice < 0) {
        QMessageBox::warning(this, "Lỗi", "Không thể tính giá tiền.");
        return;
    }

    BookingInfoWindow* infoWindow = new BookingInfoWindow;
    infoWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(infoWindow, &BookingInfoWindow::infoConfirmed, this, [=](const QString& name, const QString& phone) {
        QString dateStr = selectedDate.toString("dd/MM/yyyy");

        QString entry = QString("%1-%2-%3-%4-%5-%6-%7-%8")
                            .arg(selectedSport)
                            .arg(selectedField)
                            .arg(dateStr)
                            .arg(start)
                            .arg(end)
                            .arg(name)
                            .arg(phone)
                            .arg(totalPrice);

        string filePath = QCoreApplication::applicationDirPath().toStdString() + "/data/booking.txt";
        ofstream file(filePath, ios::app);
        if (file.is_open()) {
            file << entry.toUtf8().constData() << endl;
            file.close();
        }

        sortBookingFile(filePath);

        QMessageBox::information(this, "Xác nhận đặt sân", "Đặt sân thành công!");

        emit timeConfirmed();

        this->close();
    });

    connect(infoWindow, &BookingInfoWindow::backToTimeSelection, this, [this]() {
        this->show();
    });

    this->hide();
    infoWindow->show();
}

QString TimeWindow::removeAccents(const QString& input) {
    QString output = input;
    static const QMap<QChar, QChar> accentMap = {
        // a
        {L'á', L'a'}, {L'à', L'a'}, {L'ả', L'a'}, {L'ã', L'a'}, {L'ạ', L'a'},
        {L'ă', L'a'}, {L'ắ', L'a'}, {L'ằ', L'a'}, {L'ẳ', L'a'}, {L'ẵ', L'a'}, {L'ặ', L'a'},
        {L'â', L'a'}, {L'ấ', L'a'}, {L'ầ', L'a'}, {L'ẩ', L'a'}, {L'ẫ', L'a'}, {L'ậ', L'a'},
        // e
        {L'é', L'e'}, {L'è', L'e'}, {L'ẻ', L'e'}, {L'ẽ', L'e'}, {L'ẹ', L'e'},
        {L'ê', L'e'}, {L'ế', L'e'}, {L'ề', L'e'}, {L'ể', L'e'}, {L'ễ', L'e'}, {L'ệ', L'e'},
        // i
        {L'í', L'i'}, {L'ì', L'i'}, {L'ỉ', L'i'}, {L'ĩ', L'i'}, {L'ị', L'i'},
        // o
        {L'ó', L'o'}, {L'ò', L'o'}, {L'ỏ', L'o'}, {L'õ', L'o'}, {L'ọ', L'o'},
        {L'ô', L'o'}, {L'ố', L'o'}, {L'ồ', L'o'}, {L'ổ', L'o'}, {L'ỗ', L'o'}, {L'ộ', L'o'},
        {L'ơ', L'o'}, {L'ớ', L'o'}, {L'ờ', L'o'}, {L'ở', L'o'}, {L'ỡ', L'o'}, {L'ợ', L'o'},
        // u
        {L'ú', L'u'}, {L'ù', L'u'}, {L'ủ', L'u'}, {L'ũ', L'u'}, {L'ụ', L'u'},
        {L'ư', L'u'}, {L'ứ', L'u'}, {L'ừ', L'u'}, {L'ử', L'u'}, {L'ữ', L'u'}, {L'ự', L'u'},
        // y
        {L'ý', L'y'}, {L'ỳ', L'y'}, {L'ỷ', L'y'}, {L'ỹ', L'y'}, {L'ỵ', L'y'},
        // đ
        {L'đ', L'd'},

        // A
        {L'Á', L'A'}, {L'À', L'A'}, {L'Ả', L'A'}, {L'Ã', L'A'}, {L'Ạ', L'A'},
        {L'Ă', L'A'}, {L'Ắ', L'A'}, {L'Ằ', L'A'}, {L'Ẳ', L'A'}, {L'Ẵ', L'A'}, {L'Ặ', L'A'},
        {L'Â', L'A'}, {L'Ấ', L'A'}, {L'Ầ', L'A'}, {L'Ẩ', L'A'}, {L'Ẫ', L'A'}, {L'Ậ', L'A'},
        // E
        {L'É', L'E'}, {L'È', L'E'}, {L'Ẻ', L'E'}, {L'Ẽ', L'E'}, {L'Ẹ', L'E'},
        {L'Ê', L'E'}, {L'Ế', L'E'}, {L'Ề', L'E'}, {L'Ể', L'E'}, {L'Ễ', L'E'}, {L'Ệ', L'E'},
        // I
        {L'Í', L'I'}, {L'Ì', L'I'}, {L'Ỉ', L'I'}, {L'Ĩ', L'I'}, {L'Ị', L'I'},
        // O
        {L'Ó', L'O'}, {L'Ò', L'O'}, {L'Ỏ', L'O'}, {L'Õ', L'O'}, {L'Ọ', L'O'},
        {L'Ô', L'O'}, {L'Ố', L'O'}, {L'Ồ', L'O'}, {L'Ổ', L'O'}, {L'Ỗ', L'O'}, {L'Ộ', L'O'},
        {L'Ơ', L'O'}, {L'Ớ', L'O'}, {L'Ờ', L'O'}, {L'Ở', L'O'}, {L'Ỡ', L'O'}, {L'Ợ', L'O'},
        // U
        {L'Ú', L'U'}, {L'Ù', L'U'}, {L'Ủ', L'U'}, {L'Ũ', L'U'}, {L'Ụ', L'U'},
        {L'Ư', L'U'}, {L'Ứ', L'U'}, {L'Ừ', L'U'}, {L'Ử', L'U'}, {L'Ữ', L'U'}, {L'Ự', L'U'},
        // Y
        {L'Ý', L'Y'}, {L'Ỳ', L'Y'}, {L'Ỷ', L'Y'}, {L'Ỹ', L'Y'}, {L'Ỵ', L'Y'},
        // Đ
        {L'Đ', L'D'}
    };

    for (int i = 0; i < output.length(); ++i) {
        if (accentMap.contains(output[i])) {
            output[i] = accentMap[output[i]];
        }
    }

    return output;
}
