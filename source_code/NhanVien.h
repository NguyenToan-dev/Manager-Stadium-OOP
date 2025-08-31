// NhanVien.h
#ifndef NHANVIEN_H
#define NHANVIEN_H

#include <QString>
#include <QLocale>
#include <QString>
#include <algorithm>
class NhanVien {
public:
    NhanVien(const QString &id = "", const QString &hoTen = "",
             const QString &gioiTinh = "", const QString &ngaySinh = "",
             const QString &chucVu = "", qlonglong luong = 0, int soNgayNghi = 0,
             const QString &sdt = "");  // Đổi từ taiKhoan sang sdt

    bool operator==(const NhanVien &other) const;

    // Getters
    QString id() const;
    QString hoTen() const;
    QString gioiTinh() const;
    QString ngaySinh() const;
    QString chucVu() const;
    qlonglong luong() const;  // Đổi từ int sang qlonglong
    QString luongFormatted() const;
    int soNgayNghi() const;
    QString sdt() const;  // Đổi từ taiKhoan() sang sdt()
    qlonglong luongThucNhan() const;
    QString luongThucNhanFormatted() const;

    // Setters
    void setId(const QString &id);
    void setHoTen(const QString &hoTen);
    void setGioiTinh(const QString &gioiTinh);
    void setNgaySinh(const QString &ngaySinh);
    void setChucVu(const QString &chucVu);
    void setLuong(qlonglong luong);  // Đổi từ int sang qlonglong
    void setSoNgayNghi(int ngay);
    void setSdt(const QString &sdt);  // Đổi từ setTaiKhoan sang setSdt

private:
    QString m_id;
    QString m_hoTen;
    QString m_gioiTinh;
    QString m_ngaySinh;
    QString m_chucVu;
    qlonglong m_luong;  // Đổi từ int sang qlonglong
    int m_soNgayNghi;
    QString m_sdt;  // Đổi từ m_taiKhoan sang m_sdt
};

#endif // NHANVIEN_H
