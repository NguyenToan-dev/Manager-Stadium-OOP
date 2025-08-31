// nhanvienmanager.h
#ifndef NHANVIENMANAGER_H
#define NHANVIENMANAGER_H

#include <QMainWindow>
#include <QVector>
#include "NhanVien.h"
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include <QIntValidator>
#include <QLocale>
#include <QDoubleValidator>
QT_BEGIN_NAMESPACE
class QTableWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QGroupBox;
QT_END_NAMESPACE

class NhanVienManager : public QMainWindow {
    Q_OBJECT
public:
    explicit NhanVienManager(QWidget *parent = nullptr);
    ~NhanVienManager() override = default;

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onSave();
    void onResetNghi();
    void onRowSelected(int row, int column);
    void onExportToWord();
    void onLuongTextChanged(const QString &text);
    void onSoNgayNghiChanged(const QString &text);
    void onToggleInfoPanel();
    void onClearSearch();  // New slot
    void onSearch();  // Slot mới cho nút tìm kiếm

private:
    void updateLuongThucNhan();
    void adjustColumnWidths();
    void setupUi();
    void connectSignals();
    void loadData();
    void refreshTable();
    void clearInputs();
    void applyDarkTheme();
    void addRowToTable(const NhanVien &nv, const QLocale &locale);
    NhanVien currentInput() const;
    void populateInputs(const NhanVien &nv);
    void exportListToWord(const QString &filePath);
    QString toRtfString(const QString &text) const;
    QString formatCurrency(qlonglong value) const;
    void toggleInfoPanel(bool visible);

    QVector<NhanVien> m_list;
    QTableWidget *m_table;

    // Widgets nhập liệu
    QLineEdit *m_idEdit;
    QLineEdit *m_hoTenEdit;
    QLineEdit *m_gioiTinhEdit;
    QLineEdit *m_ngaySinhEdit;
    QLineEdit *m_chucVuEdit;
    QLineEdit *m_luongEdit;
    QLineEdit *m_soNgayNghiEdit;
    QLineEdit *m_sdtEdit;
    QLabel *m_luongThucNhanLabel;

    // Nút chức năng
    QPushButton *m_addBtn;
    QPushButton *m_editBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_saveBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_resetNghiBtn;

    // Thành phần mới
    QPushButton *m_togglePanelBtn;
    QPushButton *m_showPanelBtn;
    QGroupBox *m_infoPanel;

    // Search widget
    QLineEdit *m_searchEdit;

    // New clear button
    QPushButton *m_clearSearchBtn;

    // Nút tìm kiếm mới
    QPushButton *m_searchBtn;

    int m_currentRow;
    bool m_panelVisible;
};

#endif // NHANVIENMANAGER_H
