#include "PriceWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimeEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCoreApplication>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QMessageBox>

QString PriceWindow::removeAccents(const QString& input) {
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

PriceWindow::PriceWindow(const QString& sport, const QString& field, QWidget* parent)
    : QWidget(parent), sportName(sport), fieldName(field) {

    setWindowTitle("Tùy chỉnh giá: " + field);
    setFixedSize(400, 400);
    QVBoxLayout* layout = new QVBoxLayout(this);

    setStyleSheet(R"(
        PriceWindow {
            background-color: #3e4a59;
            font-family: "Segoe UI", sans-serif;
            font-size: 14px;
        }

        QLabel {
            color: #ffffff;
            background: transparent;
        }

        /* Nút remove (❌) theo style backButton */
        QPushButton[text="❌"] {
            background-color: #34495e;
            color: #ffffff;
            border: 1px solid #4a637d;
            border-radius: 4px;
        }
        QPushButton[text="❌"]:hover {
            background-color: #4a637d;
        }

        /* Nút thêm + lưu: dùng style nút chính trong theme app */
        QPushButton#addButton, QPushButton#saveButton {
            background-color: #34495e;
            color: white;
            border-radius: 6px;
            font-weight: bold;
            padding: 8px 14px;
        }
        QPushButton#addButton:hover, QPushButton#saveButton:hover {
            background-color: #4a637d;
        }
    )");

    entryLayout = new QVBoxLayout;
    layout->addLayout(entryLayout);

    QPushButton* addButton = new QPushButton("➕ Thêm khung giờ");
    addButton->setObjectName("addButton");
    QPushButton* saveButton = new QPushButton("💾 Lưu");
    saveButton->setObjectName("saveButton");

    layout->addWidget(addButton);
    layout->addWidget(saveButton);
    layout->addStretch();

    connect(addButton, &QPushButton::clicked, this, [=]() {
        addTimeEntry();
    });

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        saveToFile();
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Lưu thành công");
        msgBox.setText("Đã lưu giá tùy chỉnh.");
        msgBox.setIcon(QMessageBox::Information);

        msgBox.setStyleSheet(R"(
            QMessageBox {
                background-color: #3e4a59;
                color: #e0e0e0;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton {
                background-color: #0d6efd;
                color: white;
                border-radius: 4px;
                padding: 6px 12px;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #0b5ed7;
            }
        )");

        msgBox.exec();
        close();
    });

    loadExisting();
}

void PriceWindow::addTimeEntry(const QTime& from, const QTime& to, int price) {
    QHBoxLayout* row = new QHBoxLayout;

    QTimeEdit* fromEdit = new QTimeEdit(from.isValid() ? from : QTime(5, 0));
    QTimeEdit* toEdit = new QTimeEdit(to.isValid() ? to : QTime(6, 0));

    fromEdit->setDisplayFormat("hh:mm");
    toEdit->setDisplayFormat("hh:mm");

    // ⏰ Giới hạn từ 05:00 đến 22:00
    fromEdit->setMinimumTime(QTime(5, 0));
    fromEdit->setMaximumTime(QTime(22, 0));
    toEdit->setMinimumTime(QTime(5, 0));
    toEdit->setMaximumTime(QTime(22, 0));

    toEdit->setMinimumTime(fromEdit->time());
    connect(fromEdit, &QTimeEdit::timeChanged, this, [=]() {
        toEdit->setMinimumTime(fromEdit->time());
    });

    QSpinBox* priceBox = new QSpinBox;
    priceBox->setRange(0, 10000000);
    priceBox->setValue(price);
    priceBox->setSuffix(" đ");

    QPushButton* removeButton = new QPushButton("❌");
    removeButton->setFixedWidth(30);

    row->addWidget(new QLabel("Từ:"));
    row->addWidget(fromEdit);
    row->addWidget(new QLabel("  →"));
    row->addWidget(toEdit);
    row->addWidget(new QLabel("Giá:"));
    row->addWidget(priceBox);
    row->addWidget(removeButton);

    QWidget* rowWidget = new QWidget;
    rowWidget->setLayout(row);
    entryLayout->addWidget(rowWidget);

    connect(removeButton, &QPushButton::clicked, this, [=]() {
        entryLayout->removeWidget(rowWidget);
        rowWidget->deleteLater();
    });
}

void PriceWindow::loadExisting() {
    QString dirPath = QCoreApplication::applicationDirPath() + "/data/price/" + sportName;
    QString filePath = dirPath + "/" + removeAccents(fieldName).replace(" ", "_") + ".txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split('-');
        if (parts.size() < 3) continue;

        QTime from = QTime::fromString(parts[0], "hh:mm");
        QTime to = QTime::fromString(parts[1], "hh:mm");
        int price = parts[2].toInt();
        if (from.isValid() && to.isValid() && price > 0) {
            addTimeEntry(from, to, price);
        }
    }
}

struct TimeRange {
    QTime from;
    QTime to;
    int price;
};

bool isOverlap(const QTime& f1, const QTime& t1, const QTime& f2, const QTime& t2) {
    return f1 < t2 && f2 < t1;
}

void PriceWindow::saveToFile() {
    QString dirPath = QCoreApplication::applicationDirPath() + "/data/price/" + sportName;
    QDir().mkpath(dirPath);
    QString filePath = dirPath + "/" + removeAccents(fieldName).replace(" ", "_") + ".txt";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate)) return;

    QList<TimeRange> result;

    for (int i = 0; i < entryLayout->count(); ++i) {
        QWidget* rowWidget = entryLayout->itemAt(i)->widget();
        if (!rowWidget) continue;

        QList<QTimeEdit*> edits = rowWidget->findChildren<QTimeEdit*>();
        QList<QSpinBox*> boxes = rowWidget->findChildren<QSpinBox*>();
        if (edits.size() == 2 && !boxes.isEmpty()) {
            QTime from = edits[0]->time();
            QTime to = edits[1]->time();
            int price = boxes[0]->value();

            if (from < QTime(5, 0) || to > QTime(22, 0) || from >= to) continue;

            QList<TimeRange> updated;

            for (const TimeRange& existing : result) {
                if (!isOverlap(existing.from, existing.to, from, to)) {
                    updated.append(existing);
                } else {
                    if (existing.from < from) {
                        updated.append({existing.from, from, existing.price});
                    }
                    if (existing.to > to) {
                        updated.append({to, existing.to, existing.price});
                    }
                }
            }

            // Thêm khung mới
            updated.append({from, to, price});
            result = updated;
        }
    }

    std::sort(result.begin(), result.end(), [](const TimeRange& a, const TimeRange& b) {
        return a.from < b.from;
    });

    QTextStream out(&file);
    for (const auto& r : result) {
        out << r.from.toString("hh:mm") << "-" << r.to.toString("hh:mm") << "-" << r.price << "\n";
    }

    file.close();
}
