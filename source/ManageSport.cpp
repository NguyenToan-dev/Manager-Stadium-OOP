#include "ManageSport.h"
#include "AddSportWindow.h"
#include "mainwindow.h"
#include "FieldSelectionWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QCoreApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QMenu>
#include <QPixmap>
#include <QDebug>
#include <QStackedLayout>

QString removeAccents(const QString& input) {
    QString output = input;
    static const QMap<QChar, QChar> accentMap = {
        {L'á', L'a'}, {L'à', 'a'}, {L'ả', 'a'}, {L'ã', 'a'}, {L'ạ', 'a'}, {L'ă', 'a'}, {L'ắ', 'a'}, {L'ằ', 'a'}, {L'ẳ', 'a'}, {L'ẵ', 'a'}, {L'ặ', 'a'}, {L'â', 'a'}, {L'ấ', 'a'}, {L'ầ', 'a'}, {L'ẩ', 'a'}, {L'ẫ', 'a'}, {L'ậ', 'a'},
        {L'é', 'e'}, {L'è', 'e'}, {L'ẻ', 'e'}, {L'ẽ', 'e'}, {L'ẹ', 'e'}, {L'ê', 'e'}, {L'ế', 'e'}, {L'ề', 'e'}, {L'ể', 'e'}, {L'ễ', 'e'}, {L'ệ', 'e'},
        {L'í', 'i'}, {L'ì', 'i'}, {L'ỉ', 'i'}, {L'ĩ', 'i'}, {L'ị', 'i'},
        {L'ó', 'o'}, {L'ò', 'o'}, {L'ỏ', 'o'}, {L'õ', 'o'}, {L'ọ', 'o'}, {L'ô', 'o'}, {L'ố', 'o'}, {L'ồ', 'o'}, {L'ổ', 'o'}, {L'ỗ', 'o'}, {L'ộ', 'o'}, {L'ơ', 'o'}, {L'ớ', 'o'}, {L'ờ', 'o'}, {L'ở', 'o'}, {L'ỡ', 'o'}, {L'ợ', 'o'},
        {L'ú', 'u'}, {L'ù', 'u'}, {L'ủ', 'u'}, {L'ũ', 'u'}, {L'ụ', 'u'}, {L'ư', 'u'}, {L'ứ', 'u'}, {L'ừ', 'u'}, {L'ử', 'u'}, {L'ữ', 'u'}, {L'ự', 'u'},
        {L'ý', 'y'}, {L'ỳ', 'y'}, {L'ỷ', 'y'}, {L'ỹ', 'y'}, {L'ỵ', 'y'},
        {L'đ', 'd'},
        {L'Á', 'A'}, {L'À', 'A'}, {L'Ả', 'A'}, {L'Ã', 'A'}, {L'Ạ', 'A'}, {L'Ă', 'A'}, {L'Ắ', 'A'}, {L'Ằ', 'A'}, {L'Ẳ', 'A'}, {L'Ẵ', 'A'}, {L'Ặ', 'A'}, {L'Â', 'A'}, {L'Ấ', 'A'}, {L'Ầ', 'A'}, {L'Ẩ', 'A'}, {L'Ẫ', 'A'}, {L'Ậ', 'A'},
        {L'É', 'E'}, {L'È', 'E'}, {L'Ẻ', 'E'}, {L'Ẽ', 'E'}, {L'Ẹ', 'E'}, {L'Ê', 'E'}, {L'Ế', 'E'}, {L'Ề', 'E'}, {L'Ể', 'E'}, {L'Ễ', 'E'}, {L'Ệ', 'E'},
        {L'Í', 'I'}, {L'Ì', 'I'}, {L'Ỉ', 'I'}, {L'Ĩ', 'I'}, {L'Ị', 'I'},
        {L'Ó', 'O'}, {L'Ò', 'O'}, {L'Ỏ', 'O'}, {L'Õ', 'O'}, {L'Ọ', 'O'}, {L'Ô', 'O'}, {L'Ố', 'O'}, {L'Ồ', 'O'}, {L'Ổ', 'O'}, {L'Ỗ', 'O'}, {L'Ộ', 'O'}, {L'Ơ', 'O'}, {L'Ớ', 'O'}, {L'Ờ', 'O'}, {L'Ở', 'O'}, {L'Ỡ', 'O'}, {L'Ợ', 'O'},
        {L'Ú', 'U'}, {L'Ù', 'U'}, {L'Ủ', 'U'}, {L'Ũ', 'U'}, {L'Ụ', 'U'}, {L'Ư', 'U'}, {L'Ứ', 'U'}, {L'Ừ', 'U'}, {L'Ử', 'U'}, {L'Ữ', 'U'}, {L'Ự', 'U'},
        {L'Ý', 'Y'}, {L'Ỳ', 'Y'}, {L'Ỷ', 'Y'}, {L'Ỹ', 'Y'}, {L'Ỵ', 'Y'},
        {L'Đ', 'D'}
    };
    for (int i = 0; i < output.length(); ++i) {
        if (accentMap.contains(output[i])) {
            output[i] = accentMap[output[i]];
        }
    }
    return output;
}

ManageSport::ManageSport(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("ManageSport");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(50, 10, 10, 10);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget* scrollWidget = new QWidget(scrollArea);
    gridLayout = new QGridLayout(scrollWidget);
    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gridLayout->setSpacing(30);
    gridLayout->setContentsMargins(30, 10, 30, 10);

    scrollWidget->setLayout(gridLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    this->setStyleSheet(R"(
        QPushButton {
            background-color: #0d6efd;
            color: white;
            padding: 4px 10px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #0b5ed7;
        }
        /* === Main Widget Background === */
        #ManageSport {
            background-color: #3e4a59;
        }

        /* === General Font & Color === */
        QWidget {
            font-family: "Segoe UI", "Arial";
            font-size: 13px;
            color: #e0e0e0;
        }

        QScrollArea, QScrollArea > QWidget, QScrollArea > QWidget > QWidget {
            background: transparent;
            border: none;
        }

        /* === Sport Card Styling === */
        QWidget#sportCard {
            background-color: #2c3e50;
            border: 1px solid #4a637d;
            border-radius: 6px;
        }

        /* === Card Title Label === */
        QLabel#nameLabel {
            font-weight: bold;
            font-size: 15px;
            color: #ffffff;
        }

        /* === Primary Button (like 'Chọn') === */
        QPushButton#selectButton {
            background-color: #0d6efd;
            color: white;
            padding: 4px 10px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton#selectButton:hover {
            background-color: #0b5ed7;
        }

        /* === Menu Button on Card (⋮) === */
        QPushButton#menuButton {
            background-color: rgba(0, 0, 0, 0.4);
            color: white;
            border: none;
            border-radius: 4px;
        }
        QPushButton#menuButton:hover {
            background-color: rgba(0, 0, 0, 0.6);
        }

        /* === Context Menu Styling === */
        QMenu {
            background-color: #34495e;
            border: 1px solid #4a637d;
            color: #e0e0e0;
            padding: 6px;
            font-weight: bold;
        }
        QMenu::item {
            padding: 6px 16px;
        }
        QMenu::item:selected {
            background-color: #0d6efd;
            color: #ffffff;
        }


    )");

    QString imageFolder = QCoreApplication::applicationDirPath() + "/images/sport/";
    QDir dir;
    if (!dir.exists(imageFolder)) {
        dir.mkpath(imageFolder);
    }
    loadSportsFromFile();
}

void ManageSport::loadSportsFromFile()
{
    QFile file(QCoreApplication::applicationDirPath() + "/data/sports.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        int row = 0;
        int col = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty())
            {
                existingSports.append(line);
                createSportCard(line, row, col);
                col++;
                if (col >= 3)
                {
                    col = 0;
                    row++;
                }
            }
        }
        file.close();
    }
}

void ManageSport::createSportCard(const QString& name, int row, int col)
{
    QWidget* card = new QWidget;
    card->setObjectName("sportCard");
    card->setFixedSize(300, 400);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignTop);

    QWidget* imageContainer = new QWidget;
    imageContainer->setFixedSize(280, 250);
    imageContainer->setStyleSheet("background-color: #4a637d; border-radius: 4px;");

    QStackedLayout* imageStack = new QStackedLayout(imageContainer);
    imageStack->setContentsMargins(0, 0, 0, 0);

    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(280, 280);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border-radius: 4px;");
    imageStack->addWidget(imageLabel);

    QPushButton* menuButton = new QPushButton("⋮", imageContainer);
    menuButton->setObjectName("menuButton");
    menuButton->setFixedSize(24, 24);
    menuButton->move(imageLabel->width() - 28, 4);
    menuButton->raise();

    QMenu* menu = new QMenu(menuButton);
    QAction* editAction = menu->addAction("Sửa");

    connect(menuButton, &QPushButton::clicked, this, [=]() {
        menu->exec(menuButton->mapToGlobal(QPoint(0, menuButton->height())));
    });
    connect(editAction, &QAction::triggered, this, [=]() {
        auto* editWindow = new AddSportWindow;
        editWindow->setEditMode(name, existingSports);
        editWindow->setAttribute(Qt::WA_DeleteOnClose);
        editWindow->show();
        connect(editWindow, &AddSportWindow::sportConfirmed, this, [=](const QString& newName) {
            if (newName != name) existingSports.removeAll(name);
            existingSports.append(newName);
            updateSport(name, newName);
        });
    });

    QString baseName = removeAccents(name).replace(" ", "_");
    QString imagePath;
    QStringList extensions = {"png", "jpg", "jpeg"};
    for (const QString& ext : extensions) {
        QString path = QCoreApplication::applicationDirPath() + "/images/sport/" + baseName + "." + ext;
        if (QFile::exists(path)) {
            imagePath = path;
            break;
        }
    }

    QPixmap pixmap(imagePath);
    if (pixmap.isNull())
    {
        imageLabel->setText(name);
        imageLabel->setStyleSheet("background-color: #4a637d; border-radius: 4px; font-size: 14px; font-weight: bold; color: white;");
    } else {
        QPixmap scaled = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaled);
    }
    layout->addWidget(imageContainer);

    QLabel* nameLabel = new QLabel(name);
    nameLabel->setObjectName("nameLabel");
    nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameLabel->setFixedHeight(28);
    layout->addWidget(nameLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    QPushButton* selectButton = new QPushButton("Chọn");
    selectButton->setObjectName("selectButton");
    selectButton->setFixedHeight(30);
    buttonLayout->addWidget(selectButton);
    layout->addLayout(buttonLayout);
    connect(selectButton, &QPushButton::clicked, this, [=](){
        FieldSelectionWindow* fieldWindow = new FieldSelectionWindow(name, this);
        MainWindow* mainWin = qobject_cast<MainWindow*>(this->window());
        mainWin->setCentralWidgetContent(fieldWindow);
    });
    gridLayout->addWidget(card, row, col);
}

void ManageSport::updateSport(const QString& oldName, const QString& newName)
{
    QFile file(QCoreApplication::applicationDirPath() + "/data/sports.txt");
    QStringList lines;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (line == oldName)
                lines.append(newName);
            else
                lines.append(line);
        }
        file.close();
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&file);
        for (const QString& line : lines)
        {
            out << line << "\n";
        }
        file.close();
    }
    QString basePath = QCoreApplication::applicationDirPath() + "/images/field/";
    QDir dir(basePath);
    dir.rename(oldName, newName);
    QString oldDataPath = QCoreApplication::applicationDirPath() + "/data/" + removeAccents(oldName).replace(" ", "_") + ".txt";
    QString newDataPath = QCoreApplication::applicationDirPath() + "/data/" + removeAccents(newName).replace(" ", "_") + ".txt";
    QFile::rename(oldDataPath, newDataPath);
    renameSportImage(removeAccents(oldName).replace(" ", "_"), removeAccents(newName).replace(" ", "_"));
    reloadSports();
}

void ManageSport::reloadSports()
{
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
    existingSports.clear();
    loadSportsFromFile();
}

void ManageSport::renameSportImage(const QString& oldName, const QString& newName)
{
    QString basePath = QCoreApplication::applicationDirPath() + "/images/sport/";
    QStringList extensions = {"png", "jpg", "jpeg"};
    for (const QString& ext : extensions) {
        QString oldPath = basePath + oldName + "." + ext;
        QString newPath = basePath + newName + "." + ext;
        if (QFile::exists(oldPath)) {
            QFile::rename(oldPath, newPath);
            break;
        }
    }
}
