# ⚽ Stadium Manager - OOP Project

## 👥 Thành viên nhóm
- Đào Minh Khoa - 24127422  
- Trần Lưu Gia Bảo - 24127018  
- Nguyễn Lê Thanh An - 24127269  
- Nguyễn Khánh Toàn - 24127252  
- Nguyễn Tiến Cường - 24127337  

GitHub Repository: [Manager-Stadium-OOP](https://github.com/NguyenToan-dev/Manager-Stadium-OOP)

---

## 📖 Mục lục
- [📌 Giới thiệu](#-giới-thiệu)
- [🏗️ Kiến trúc hệ thống](#️-kiến-trúc-hệ-thống)
- [🖥️ Tính năng chi tiết](#️-tính-năng-chi-tiết)
  - [🔑 Đăng nhập & Quản lý tài khoản](#-1-đăng-nhập--quản-lý-tài-khoản)
  - [🏟️ Quản lý sân & Đặt sân](#️-2-quản-lý-sân--đặt-sân)
  - [💳 Thanh toán & Dịch vụ](#-3-thanh-toán--dịch-vụ-kèm-theo)
  - [👥 Quản lý khách hàng](#-4-quản-lý-khách-hàng)
  - [👨‍💼 Quản lý nhân viên](#-5-quản-lý-nhân-viên)
  - [📊 Báo cáo & Thống kê](#-6-báo-cáo--thống-kê)
- [⚙️ Công nghệ sử dụng](#️-công-nghệ-sử-dụng)
- [🚀 Cách cài đặt & chạy dự án](#-cách-cài-đặt--chạy-dự-án)
- [📌 Ví dụ sử dụng](#-ví-dụ-sử-dụng)
- [🔮 Hướng phát triển](#-hướng-phát-triển)
- [📌 Kết luận](#-kết-luận)

---

## 📌 Giới thiệu
**Stadium Manager** là ứng dụng quản lý sân thể thao được xây dựng bằng **C++ và OOP**.  
Ứng dụng hỗ trợ quản lý toàn diện từ **đặt sân, quản lý khách hàng, nhân viên, tài khoản, dịch vụ kèm theo, đến báo cáo thống kê chi tiết**.  

### 🎯 Mục tiêu dự án
- Hỗ trợ chủ sân quản lý hoạt động một cách hiệu quả.  
- Giảm thiểu thao tác thủ công trong việc đặt sân, theo dõi khách hàng, quản lý nhân sự.  
- Đem đến trải nghiệm **thân thiện, trực quan, bảo mật cao** cho người dùng.  
- Ứng dụng kiến thức **Lập trình Hướng đối tượng (OOP)** vào một dự án thực tế.

---

## 🏗️ Kiến trúc hệ thống
Dự án được xây dựng theo mô hình **OOP** với các module chính:

- **Authentication (Đăng nhập & Quản lý tài khoản)**  
- **Stadium Management (Quản lý sân)**  
- **Customer Management (Khách hàng)**  
- **Employee Management (Nhân viên)**  
- **Payment & Service (Thanh toán & Dịch vụ)**  
- **Report & Statistics (Báo cáo & Thống kê)**  

---

## 🖥️ Tính năng chi tiết

### 🔑 1. Đăng nhập & Quản lý tài khoản
- Nhập tên đăng nhập + mật khẩu  
- Hiển thị/ẩn mật khẩu  
- Thông báo lỗi khi nhập sai  
- Phân quyền: Chủ, Quản lý sân, Quản lý nhân sự, Thu ngân, Kế toán  
- Tài khoản mới tự động được gán menu phù hợp với vai trò  

---

### 🏟️ 2. Quản lý sân & Đặt sân
- Người dùng chọn môn: **Bóng đá, Bóng chuyền, Bóng rổ**  
- Đặt sân:
  1. Chọn ngày & giờ bắt đầu/kết thúc  
  2. Nhập thông tin khách hàng  
  3. Xác nhận đặt sân  
- Thêm sân mới: nhập tên, giá, tải ảnh từ máy  
- Quản lý danh sách sân đã đặt (Huỷ / Thanh toán)  

---

### 💳 3. Thanh toán & Dịch vụ kèm theo
- Dịch vụ: Trọng tài, bảng tỉ số, đồ uống, giày thể thao, y tế  
- Người dùng chọn số lượng, thêm vào giỏ  
- Thanh toán + In hóa đơn  

---

### 👥 4. Quản lý khách hàng
- Danh sách khách hàng: **VIP** (tên màu vàng), **Thường**  
- Tìm kiếm theo SĐT  
- Xuất danh sách CSV/Excel  

---

### 👨‍💼 5. Quản lý nhân viên
- Quản lý toàn bộ thông tin nhân viên: ID, họ tên, ngày sinh, chức vụ, lương, SĐT  
- Chức năng: tìm kiếm, thêm, sửa, xóa, reset ngày nghỉ, lưu, xuất ra Word  

---

### 📊 6. Báo cáo & Thống kê
- Biểu đồ cột: giờ sử dụng theo sân  
- Biểu đồ đường: tần suất đặt sân  
- Biểu đồ tròn: doanh thu  
- Bộ lọc: tất cả / dịch vụ / đồ uống / y tế  
- Xuất dữ liệu CSV/Excel  

---

## ⚙️ Công nghệ sử dụng
- **C++ (OOP)**  
- **libsodium** (mã hóa mật khẩu)  
- **File I/O** – lưu trữ dữ liệu  
- **CSV / Excel / Word Export**  
- **Biểu đồ thống kê trực quan**  

---

## 🚀 Cách cài đặt & chạy dự án
1. Clone repo:
   ```bash
   git clone https://github.com/NguyenToan-dev/Manager-Stadium-OOP.git
