// NhanVien.cpp
#include "NhanVien.h"
NhanVien::NhanVien(const QString &id, const QString &hoTen,
                   const QString &gioiTinh, const QString &ngaySinh,
                   const QString &chucVu, qlonglong luong, int soNgayNghi,
                   const QString &sdt)
    : m_id(id), m_hoTen(hoTen), m_gioiTinh(gioiTinh),
    m_ngaySinh(ngaySinh), m_chucVu(chucVu), m_luong(luong),
    m_soNgayNghi(soNgayNghi), m_sdt(sdt) {}

bool NhanVien::operator==(const NhanVien &other) const {
    return m_id == other.m_id &&
           m_hoTen == other.m_hoTen &&
           m_gioiTinh == other.m_gioiTinh &&
           m_ngaySinh == other.m_ngaySinh &&
           m_chucVu == other.m_chucVu &&
           m_luong == other.m_luong;
}

// Getters
QString NhanVien::id() const { return m_id; }
QString NhanVien::hoTen() const { return m_hoTen; }
QString NhanVien::gioiTinh() const { return m_gioiTinh; }
QString NhanVien::ngaySinh() const { return m_ngaySinh; }
QString NhanVien::chucVu() const { return m_chucVu; }
qlonglong NhanVien::luong() const { return m_luong; }
int NhanVien::soNgayNghi() const { return m_soNgayNghi; }
QString NhanVien::sdt() const { return m_sdt; }

QString NhanVien::luongFormatted() const {
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    return locale.toString(m_luong) + " VND";
}

qlonglong NhanVien::luongThucNhan() const {
    if (m_soNgayNghi == 0) return m_luong;
    return m_luong - (m_luong / 30 * m_soNgayNghi);
}

QString NhanVien::luongThucNhanFormatted() const {
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    return locale.toString(luongThucNhan()) + " VND";
}

// Setters
void NhanVien::setId(const QString &id) { m_id = id; }
void NhanVien::setHoTen(const QString &hoTen) { m_hoTen = hoTen; }
void NhanVien::setGioiTinh(const QString &gioiTinh) { m_gioiTinh = gioiTinh; }
void NhanVien::setNgaySinh(const QString &ngaySinh) { m_ngaySinh = ngaySinh; }
void NhanVien::setChucVu(const QString &chucVu) { m_chucVu = chucVu; }
void NhanVien::setLuong(qlonglong luong) { m_luong = luong; }

void NhanVien::setSoNgayNghi(int ngay) {
    m_soNgayNghi = qMax(0, qMin(ngay, 31)); // Giới hạn 0-31 ngày
}

void NhanVien::setSdt(const QString &sdt) {
    m_sdt = sdt;
}
