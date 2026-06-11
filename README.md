# Modifying-the-file-system-to-support-large-files-using-indirect-blocks
# xv6 File System Extension – Double Indirect & Read Ahead

## 📌 Giới thiệu

Dự án này mở rộng hệ thống file của xv6 nhằm tăng kích thước file tối đa và cải thiện hiệu năng truy cập đĩa. Hệ thống gốc của xv6 chỉ hỗ trợ **Direct Block và Single Indirect Block**, dẫn đến giới hạn kích thước file nhỏ (~267 KB).

Trong dự án này, chúng tôi đã:

* Thêm **Double Indirect Block**
* Cập nhật các cơ chế quản lý block trong inode
* Sửa `bmap()`, `itrunc()`, `iappend()`
* Tăng số buffer cache (`NBUF`)
* Thêm cơ chế **Read Ahead** để tăng tốc đọc tuần tự

---

## ⚙️ Cấu trúc File System sau khi mở rộng

### Inode structure

* Direct blocks: `NDIRECT = 11`
* Single indirect block: 1
* Double indirect block: 1

Tổng khả năng quản lý:

* Direct: 11 block
* Single indirect: 256 block
* Double indirect: 256 × 256 = 65536 block

👉 Tổng: **65803 block (~67MB)**

---

## 🧠 Ý tưởng thiết kế

### 1. Direct Block

Inode trỏ trực tiếp tới các data block.

### 2. Single Indirect Block

Inode trỏ tới một block chứa danh sách các địa chỉ data block.

### 3. Double Indirect Block

Inode trỏ tới một block cấp 1, block này trỏ tới các block cấp 2, và các block cấp 2 trỏ tới data block.

---

## 🔧 Các thay đổi chính

### 1. bmap() – ánh xạ block logic → vật lý

* Hỗ trợ 3 mức:

  * Direct
  * Single indirect
  * Double indirect
* Cấp phát block khi cần (`balloc()`)

---

### 2. itrunc() – giải phóng block

* Giải phóng theo thứ tự:

  * Data block → Indirect → Double indirect
* Tránh memory/block leak

---

### 3. iappend() (mkfs.c)

* Cập nhật để hỗ trợ Double Indirect
* Đồng bộ với `bmap()` trong kernel
* Đảm bảo filesystem tạo ra tương thích với kernel mới

---

### 4. Buffer Cache (NBUF)

* Tăng từ 30 → 200
* Giảm lỗi thiếu buffer khi thao tác file lớn
* Tăng hiệu năng hệ thống

---

### 5. Read Ahead

* Tự động đọc block kế tiếp khi đọc tuần tự
* Giảm số lần I/O xuống disk
* Tăng tốc độ đọc file lớn

---

## 🚀 Cách build và chạy

```bash
make clean
make qemu
```

---

## 📊 Kết quả đạt được

* Tăng kích thước file tối đa từ ~267KB → ~67MB
* Hỗ trợ file system lớn hơn và ổn định hơn
* Tăng hiệu năng đọc tuần tự nhờ Read Ahead
* Giảm lỗi buffer exhaustion bằng tăng NBUF

---

## 🧩 Khái niệm quan trọng

* Inode
* Direct / Indirect / Double Indirect Block
* Logical vs Physical block
* Buffer Cache
* balloc / bfree
* bmap / itrunc / iappend
* Read Ahead

---

## 📌 Lưu ý

* mkfs và kernel phải đồng bộ cấu trúc inode
* Nếu chỉ sửa kernel mà không sửa mkfs → filesystem không tương thích
* itrunc phải giải phóng đúng thứ tự từ trong ra ngoài

---

## 👨‍💻 Tác giả

Dự án môn học – Hệ điều hành / xv6 extension
