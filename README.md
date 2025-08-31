# ⚽️🏟️ Stadium Manager – Dự án OOP (Qt) — Bản phát hành Windows .exe ✨💻

## 👥✨ Thành viên nhóm 🌟
- 🧑‍💻🔥 **Nguyễn Khánh Toàn** — 24127252  ![Team Lead](https://img.shields.io/badge/Role-Leader-blue?style=for-the-badge&logo=leadership)
- 🧑‍💻🌈 **Đào Minh Khoa** — 24127422 ![Member](https://img.shields.io/badge/Role-Member-green?style=for-the-badge&logo=team)
- 🧑‍💻🚀 **Trần Lưu Gia Bảo** — 24127018 ![Member](https://img.shields.io/badge/Role-Member-green?style=for-the-badge&logo=team)
- 🧑‍💻🎉 **Nguyễn Lê Thanh An** — 24127269 ![Member](https://img.shields.io/badge/Role-Member-green?style=for-the-badge&logo=team)
- 🧑‍💻💥 **Nguyễn Tiến Cường** — 24127337 ![Member](https://img.shields.io/badge/Role-Member-green?style=for-the-badge&logo=team)

---

## 📖✨ Mục lục 🌈
- [Giới thiệu](#giới-thiệu) 📝✨
- [Công nghệ sử dụng](#công-nghệ-sử-dụng) 🛠️🚀
- [Cấu trúc dự án](#cấu-trúc-dự-án) 📂🌟
- [Cách chạy](#cách-build--chạy) 🔨🏃‍♂️
- [Chức năng chi tiết](#chức-năng-chi-tiết) ⚙️🎯
  - [Đăng nhập](#đăng-nhập) 🔑🔒
  - [Menu chính](#menu-chính) 📋🏠
  - [Quản lý sân & đặt sân](#quản-lý-sân--đặt-sân) ⚽️🗓️
  - [Thanh toán & dịch vụ](#thanh-toán--dịch-vụ) 💳🛒
  - [Quản lý khách hàng](#quản-lý-khách-hàng) 👥🏅
  - [Quản lý nhân viên](#quản-lý-nhân-viên) 🧑‍💼📊
  - [Thống kê & báo cáo](#thống-kê--báo-cáo) 📊📈
  - [Quản lý tài khoản](#quản-lý-tài-khoản) 🔐👤
- [Định hướng phát triển](#định-hướng-phát-triển) 🚀🌱
- [Giấy phép & Liên hệ](#giấy-phép--liên-hệ) 📜📧

---

## Giới thiệu 📝✨
**Stadium Manager** là phần mềm quản lý sân thể thao được phát triển bằng **C++/Qt**. 🌟  
Ứng dụng bao quát toàn bộ quy trình: **đặt sân → thanh toán → thống kê báo cáo → quản lý khách hàng & nhân viên → phân quyền tài khoản**. 💼⚡  

### Điểm nổi bật 🌟🔥
- **Giao diện thân thiện**, dễ sử dụng. 😊👌  
- **Bảo mật cao**: mật khẩu được mã hóa bằng **libsodium**. 🔒🛡️  
- **Tích hợp đầy đủ tính năng** từ đặt sân, quản lý nhân viên đến xuất báo cáo. 📈🗂️  
- **Đóng gói dạng `.exe`**, người dùng chỉ cần mở là chạy được ngay, không cần cài Qt. 🚀💻  

---

## Công nghệ sử dụng 🛠️🚀
- **C++17** – lập trình hướng đối tượng. 📚🧩  
- **Qt 6 (Widgets)** – xây dựng giao diện người dùng. 🎨🖼️  
- **libsodium** – mã hóa mật khẩu. 🔑🔐  
- **Xuất dữ liệu** – hỗ trợ định dạng **CSV / Excel / Word**. 📁📊  
- **Build tool** – có thể dùng **CMake** hoặc **qmake** (nên chọn 1 cách cho nhất quán). 🔧🛠️  

---

## Cấu trúc dự án 📂🌟
YOUR_OLD_TRAFFORD_AT_HOME/  
├─ Headers/     # header files for classes and windows 📄🗂️  
│  ├─ AddFieldWindow.h  
│  ├─ AddSportWindow.h  
│  ├─ AuthManager.h  
│  ├─ Bao_Cao_Thong_Ke.h  
│  ├─ Bill.h  
│  ├─ BookingInfoWindow.h  
│  ├─ BookingRecord.h  
│  ├─ CalendarWindow.h  
│  ├─ Cap_Tai_Khoan.h  
│  ├─ discountbill.h  
│  ├─ FieldSelectionWindow.h  
│  ├─ HomePage.h  
│  ├─ LoginWindow.h  
│  ├─ mainwindow.h  
│  ├─ ManageBookedFields.h  
│  ├─ ManageSport.h  
│  ├─ normalbill.h  
│  ├─ NhanVien.h  
│  ├─ nhanvienmanager.h  
│  ├─ PriceWindow.h  
│  ├─ Quan_Ly_Khach_Hang.h  
│  ├─ RemoveSport.h  
│  ├─ SportVenueManager.h  
│  ├─ TimeWindow.h  
│  └─ Thanh_Toan.h  
├─ Sources/     # C++ implementation files 💻🛠️  
│  ├─ AddFieldWindow.cpp  
│  ├─ AddSportWindow.cpp  
│  ├─ AuthManager.cpp  
│  ├─ Bao_Cao_Thong_Ke.cpp  
│  ├─ Bill.cpp  
│  ├─ BookingInfoWindow.cpp  
│  ├─ BookingRecord.cpp  
│  ├─ CalendarWindow.cpp  
│  ├─ Cap_Tai_Khoan.cpp  
│  ├─ discountbill.cpp  
│  ├─ FieldSelectionWindow.cpp  
│  ├─ HomePage.cpp  
│  ├─ LoginWindow.cpp  
│  ├─ main.cpp  
│  ├─ mainwindow.cpp  
│  ├─ ManageBookedFields.cpp  
│  ├─ ManageSport.cpp  
│  ├─ normalbill.cpp  
│  ├─ NhanVien.cpp  
│  ├─ nhanvienmanager.cpp  
│  ├─ PriceWindow.cpp  
│  ├─ Quan_Ly_Khach_Hang.cpp  
│  ├─ RemoveSport.cpp  
│  ├─ SportVenueManager.cpp  
│  └─ TimeWindow.cpp  
├─ Forms/       # Qt Designer UI files 🎨🖌️  
│  ├─ HomePage.ui  
│  ├─ mainwindow.ui  
│  ├─ NoiQuyDialog.ui  
│  └─ SportVenueManager.ui  
├─ Resources/   # Qt resource collection (icons, images) 🖼️📸  
│  └─ resources.qrc  

---

## Cách chạy 🔨🏃‍♂️
- Vào thư mục **build**  để tìm file `.exe`. 📂🔎  
- Người dùng chỉ cần **chạy file `.exe`** để khởi động ứng dụng. ▶️  

---

## Chức năng chi tiết ⚙️🎯

### Đăng nhập 🔑🔒
- Nhập tài khoản & mật khẩu. 📝🔐  
- Nút **Hiện/Ẩn mật khẩu**. 👁️‍🗨️🚫  
- Thông báo lỗi khi nhập sai. ⚠️❌  
- Mật khẩu được mã hóa bằng **libsodium**. 🛡️🔒  

### Menu chính 📋🏠
- **Home** 🏠✨  
- **Sân** ⚽️🏟️  
- **Các sân đang đặt** 📅🗂️  
- **Quản lý** 🛠️👥  
- **Báo cáo thống kê** 📊📈  
- **Cấp tài khoản** 🔐👤  
- **Đăng xuất** 🚪🔓  

### Quản lý sân & đặt sân ⚽️🗓️
- Chọn môn: **Bóng đá, Bóng chuyền, Bóng rổ**. ⚽️🏐🏀  
- Đặt sân: chọn ngày, giờ, nhập thông tin khách hàng. 📆⏰👤  
- Thêm sân mới (tên, giá, hình ảnh). ➕🏟️💰🖼️  
- Danh sách sân đã đặt: **Hủy đặt / Thanh toán**. ❌💳  

### Thanh toán & dịch vụ 💳🛒
- Dịch vụ kèm theo:  
  - Trọng tài, bảng tỉ số 🏅📋  
  - Giày, tất, băng cổ chân 👟🧦🩹  
  - Đồ uống: Sting, Revive, Coca, C2… 🥤🍹  
  - Y tế: sơ cứu, thuốc giảm đau… 🩹💊  
- Thêm dịch vụ vào giỏ, thanh toán & in hóa đơn. 🛒💳🖨️  

### Quản lý khách hàng 👥🏅
- Phân loại: **VIP** (tên vàng) và **Thường**. 🏆⭐  
- Tìm kiếm theo SĐT. 🔍📞  
- Xuất danh sách sang CSV/Excel. 📤📊  

### Quản lý nhân viên 🧑‍💼📊
- Thêm nhân viên mới (kiểm tra trùng ID). ➕🆔  
- Tìm kiếm theo **ID/SĐT**. 🔍🆔📞  
- Hiển thị thông tin chi tiết: họ tên, ngày sinh, chức vụ, lương, ngày nghỉ, SĐT, lương thực nhận. 📋👤  
- Sửa / Xóa nhân viên. ✏️❌  
- Reset toàn bộ ngày nghỉ. 🔄📅  
- Xuất danh sách ra Word. 📤📝  

### Thống kê & báo cáo 📊📈
- Thống kê theo môn: **Bóng đá, Bóng rổ, Bóng chuyền**. ⚽️🏀🏐  
- **Biểu đồ cột**: giờ sử dụng theo sân. 📊🕒  
- **Biểu đồ đường**: tần suất đặt sân. 📈🗓️  
- **Biểu đồ tròn**: doanh thu. 🥧💰  
- Thống kê dịch vụ kèm theo (lọc theo loại). 🛒🔍  
- Xuất dữ liệu CSV/Excel. 📤📁  

### Quản lý tài khoản 🔐👤
- Tạo tài khoản mới:  
  - Tên người dùng (bắt buộc) 👤📝  
  - Họ tên  
  - Vai trò: **Chủ, Quản lý sân, Quản lý nhân sự, Thu ngân, Kế toán** 🏆🛠️  
  - Mật khẩu (nhập hoặc sinh ngẫu nhiên) 🔑🎲  
  - Nút hiện mật khẩu 👁️‍🗨️  
- Phân quyền hiển thị menu theo vai trò. 🔒📋  

---

## 🚀 Định hướng phát triển
- 🗄️ Chuyển đổi từ lưu trữ file sang **Database** (SQLite/MySQL/PostgreSQL) để đảm bảo tính ổn định và khả năng mở rộng.  
- 🌐 Hỗ trợ **Remote Database Access** thông qua kết nối LAN/Wi-Fi, cho phép nhiều client truy cập và thao tác đồng thời.   
- 💳 Bổ sung tính năng **QR Code** và **Online Payment** để nâng cao trải nghiệm người dùng.  
---

## Giấy phép & Liên hệ 📜📧
- **Giấy phép:** MIT. 📄🆓  
- **Liên hệ:** Vui lòng mở **Issues** trong GitHub repo. 🐙🔍  

