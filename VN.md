
# 🧨 FakeSignal

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/Giấy%20phép-WTFPL-brightgreen.svg)](http://www.wtfpl.net/)
[![Status](https://img.shields.io/badge/Tình%20trạng-WONTFIX-red.svg)]()
[![Nền tảng](https://img.shields.io/badge/nền%20tảng-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)]()
[![PRs Chào đón](https://img.shields.io/badge/PRs-ch%C3%A0o%20đón-brightgreen.svg)](CONTRIBUTING.md)

> **"Fake trong tên, thật trong bản chất – cũng như ý chí sống sót của tôi sau khi debug cái này."**

Tiếng Việt | [English](README.md)

## 📦 FakeSignal là gì?

**FakeSignal** là một thư viện C++ cung cấp hàm `raise()` cho các **tín hiệu giả**… nhưng **thực sự crash chương trình của bạn** bằng các hành vi không xác định (undefined behavior) có thật.

Đúng vậy, bạn đọc không nhầm đâu.  
Đây là thư viện dành cho những lập trình viên muốn mô phỏng tín hiệu *theo cách khó* — bằng cách tự gây segfault, chia cho 0, tràn stack, hoặc rò rỉ bộ nhớ cho đến khi OOM killer xuất hiện.

### Tại sao?

Bởi vì đôi khi bạn muốn kiểm tra:
- Trình xử lý tín hiệu của bạn (liệu nó có sống sót sau crash thật không? spoiler: không ❌)
- Kỹ năng debug của bạn 🐛
- Ý chí tiếp tục lập trình của bạn 😭

Ngoài ra: `std::raise(SIGSEGV)` quá nhàm chán. Chúng tôi ưu chuộng các **cú crash được xác nhận bằng phần cứng** hơn.

---

## 🚀 Bắt đầu nhanh

### 1. Include header

```cpp
#include "fakesignal.h"
```

### 2. Gọi raise() với tín hiệu giả yêu thích của bạn

```cpp
int main() {
    fakesignal::raise("SIGSEGV");
    // Bạn sẽ không bao giờ đến được dòng này.
    // Trừ khi bạn đang ở vũ trụ song song nơi dereference nullptr là hợp pháp.
    return 0;
}
```

### 3. Biên dịch (nhớ include fakesignal.cpp)

```bash
g++ -std=c++17 -pthread main.cpp fakesignal.cpp -o demo
./demo
Segmentation fault (core dumped)  # ✅ Hoạt động như thiết kế
```

Vậy đấy. Chương trình của bạn giờ đây crash theo yêu cầu.

---

## 🧨 Các "tín hiệu" được hỗ trợ

| Tín hiệu | Cách triển khai | Crash thật? | Mức độ hài hước |
|----------|----------------|-------------|------------------|
| `SIGSEGV` | `int* p = nullptr; *p = 42;` | ✅ Có | 💀💀💀 |
| `SIGFPE` | `int x = 1 / 0;` | ✅ Có* | 💀💀 |
| `SIGABRT` | `std::abort();` | ✅ Có | 💀 |
| `SIGILL` | `asm volatile("ud2");` | ✅ Có (x86) | 💀💀💀 |
| `INFINITE_LOOP` | `while(true) {}` | ⚠️ Không crash, chỉ treo | 💀 |
| `STACK_OVERFLOW` | Đệ quy `raise()` | ✅ Có | 💀💀💀💀 |
| `MEMORY_LEAK` | `while(true) new int[1000000];` | ✅ Có (cuối cùng) | 💀💀 |
| `DEADLOCK` | Hai mutex, hai thread | ⚠️ Treo vĩnh viễn | 💀💀💀 |

*\*Có thể bị tối ưu hóa bởi compiler khá hung dữ – chúng tôi gọi đó là "tính năng, không phải lỗi"*

---

## 📖 Ví dụ đầy đủ

### Ví dụ 1: Crash cơ bản

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raise("SIGSEGV");  // 💥
}
```

### Ví dụ 2: Chọn tín hiệu tương tác

```cpp
#include <iostream>
#include "fakesignal.h"

int main() {
    std::string sig;
    std::cout << "Nhập tín hiệu (SIGSEGV, SIGFPE, INFINITE_LOOP): ";
    std::cin >> sig;
    fakesignal::raise(sig);  // 💥 hoặc 🔄
}
```

### Ví dụ 3: Thử bắt thứ không thể bắt

```cpp
#include <csignal>
#include <iostream>
#include "fakesignal.h"

void handler(int) {
    std::cout << "Đã bắt được! Nhưng tôi có phục hồi được không? 😅" << std::endl;
}

int main() {
    signal(SIGSEGV, handler);
    fakesignal::raise("SIGSEGV");
    // Output: Đã bắt được! Nhưng tôi có phục hồi được không?
    // Sau đó: Segmentation fault (core dumped)
    // Spoiler: bạn không thể phục hồi từ UB
}
```

### Ví dụ 4: Raise tất cả tín hiệu (crash ngay cái đầu tiên)

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::raiseAll();  // 💥💥💥
}
```

### Ví dụ 5: Chạy thử (không crash, nhàm chán nhưng tồn tại)

```cpp
#include "fakesignal.h"

int main() {
    fakesignal::dryRun("SIGSEGV");
    std::cout << "Chương trình vẫn chạy tiếp... (khác với bản crash thật)" << std::endl;
}
```

---

## 📁 Cấu trúc dự án

```
fakesignal/
├── README.md
├── CONTRIBUTING.md
├── LICENSE
├── fakesignal.h          ← Header (khai báo)
├── fakesignal.cpp        ← Định nghĩa (các crash thật sự)
└── examples/
    ├── crash_demo.cpp
    ├── signal_selector.cpp
    ├── crash_all.cpp
    └── dry_run_demo.cpp
```

---

## 🛠 Cài đặt

### Cách 1: Copy-paste (khuyến nghị cho đau khổ nhanh)

Chỉ cần copy `fakesignal.h` và `fakesignal.cpp` vào dự án của bạn và biên dịch cùng nhau.

```bash
g++ -std=c++17 -pthread code_cua_ban.cpp fakesignal.cpp -o chuong_trinh_cua_ban
```

### Cách 2: Thư viện tĩnh

```bash
g++ -std=c++17 -c fakesignal.cpp -o fakesignal.o
ar rcs libfakesignal.a fakesignal.o
g++ -std=c++17 code_cua_ban.cpp -L. -lfakesignal -pthread -o chuong_trinh_cua_ban
```

### Cách 3: CMake (dành cho người thích đau khổ)

```bash
git clone https://github.com/tencuaban/fakesignal.git
cd fakesignal
mkdir build && cd build
cmake .. -DFAKESIGNAL_BUILD_EXAMPLES=ON
make
sudo make install  # nếu bạn dám
```

Sau đó trong `CMakeLists.txt` của bạn:

```cmake
find_package(fakesignal REQUIRED)
target_link_libraries(target_cua_ban fakesignal)
```

---

## 🧪 Biên dịch và chạy ví dụ

```bash
git clone https://github.com/tencuaban/fakesignal.git
cd fakesignal

# Biên dịch một ví dụ thủ công
g++ -std=c++17 -pthread examples/crash_demo.cpp fakesignal.cpp -o crash_demo
./crash_demo

# Hoặc dùng CMake
mkdir build && cd build
cmake .. -DFAKESIGNAL_BUILD_EXAMPLES=ON
make
./examples/crash_demo
./examples/signal_selector
./examples/crash_all
```

---

## ⚠️ Cảnh báo quan trọng

| Cảnh báo | Giải thích |
|----------|------------|
| **Thư viện này KHÔNG phải giả** | Các crash là 100% thật. Chương trình của bạn SẼ kết thúc bất thường. |
| **KHÔNG dùng trong production** | Trừ khi bạn ghét công việc của mình và muốn xuất hiện trên [The Daily WTF](https://thedailywtf.com). |
| **KHÔNG mở issue nói "chương trình tôi bị crash"** | Đó là toàn bộ mục đích. Đọc README. |
| **Trình xử lý tín hiệu sẽ không cứu bạn** | Hành vi không xác định là, theo định nghĩa, không xác định. Bạn không thể phục hồi từ dereference nullptr. |
| **Có thể gây mất dữ liệu** | Nếu bạn chưa lưu công việc của mình, thì bây giờ là thời điểm tốt để làm điều đó. |

### Điều gì xảy ra khi bạn bắt SIGSEGV?

```cpp
void handler(int) {
    // Bạn đang ở đây. Nhưng instruction đã gây lỗi?
    // Nó sẽ không thử lại. Bạn không thể "sửa" nullptr.
    // longjmp? UB. Sửa instruction pointer? Chúc may mắn.
    std::cout << "Tôi đang gặp nguy hiểm 🫠" << std::endl;
}
```

Điều duy nhất có thể làm được trong trình xử lý SIGSEGV là:
- Ghi log lỗi (có thể)
- Flush logs
- `_exit(128 + SIGSEGV)`

---

## 🧪 Kiểm thử

FakeSignal có các bài kiểm thử mà **mong đợi sẽ crash**. Đó là cách chúng tôi biết chúng đạt yêu cầu.

```bash
cd build
cmake .. -DFAKESIGNAL_BUILD_TESTS=ON
make
ctest
```

Đầu ra mong đợi:
```
100% tests passed, 0 tests failed out of 4
```
…mặc dù mọi bài kiểm thử đều crash. Đó là phép thuật của `WILL_FAIL`.

---

## 🤝 Đóng góp

Chúng tôi chào đón sự đóng góp… nhưng tại sao bạn lại muốn thế?

Xem [CONTRIBUTING.md](CONTRIBUTING.md) để biết hướng dẫn.

**Ý tưởng cho các "tín hiệu" mới:**
- `SIGDANGLING` – use-after-free
- `SIGDOUBLEFREE` – double deletion
- `SIGRACECOND` – data race
- `SIGFORMAT` – mô phỏng tấn công định dạng chuỗi

Mở PR nếu bạn dũng cảm (hoặc khờ khạo).

---

## 📜 Giấy phép

**WTFPL** – Do What The Fuck You Want To Public License (Hãy làm cái quái gì bạn muốn).

Nói đơn giản:
- ✅ Dùng ở bất cứ đâu
- ✅ Sửa đổi thoải mái
- ✅ Đốt nó, in nó, gấp thành máy bay giấy
- ❌ Đừng kiện tôi nếu nó crash (nó sẽ crash)
- ❌ Đừng hỏi tôi sửa nó (tôi không thể sửa undefined behavior)

[![License: WTFPL](https://img.shields.io/badge/Giấy%20phép-WTFPL-brightgreen.svg)](http://www.wtfpl.net/)

*"Sức mạnh lớn kèm theo segfault lớn."*

---

## 🙏 Lời cảm ơn

- **nullptr** – vì luôn ở đó khi chúng tôi cần crash
- **Undefined Behavior** – vì không bao giờ làm chúng tôi thất vọng
- **Ủy ban Tiêu chuẩn C++** – vì đã làm cho điều này khả thi
- **Bác sĩ trị liệu của tôi** – vì đã lắng nghe tôi giải thích tại sao tôi viết cái này

---

## ⭐ Lịch sử Star

Nếu bạn star repo này, code của bạn vẫn sẽ crash.  
Nhưng ít nhất bạn sẽ có một ngôi sao sáng. ⭐

---

## 📞 Hỗ trợ

**Không có hỗ trợ nào cả.**  
Đây là thư viện cố tình crash chương trình của bạn.  
Bạn mong đợi sự hỗ trợ kiểu gì? 😅

Để được hỗ trợ về mặt hiện sinh, hãy liên hệ với triết gia địa phương của bạn.

---

## 📋 Yêu cầu

- C++17 trở lên
- Một máy tính có thể segfault (tất cả đều được)
- Khiếu hài hước 🤡
- Không có ý chí sống (không bắt buộc)
- `pthread` (cho `DEADLOCK` và các tính năng đa luồng)

### Hỗ trợ nền tảng

| Nền tảng | Trình biên dịch | Trạng thái |
|----------|----------------|------------|
| Linux | GCC 7+, Clang 5+ | ✅ Hỗ trợ đầy đủ |
| Windows | MSVC 2017+, MinGW | ✅ Hỗ trợ |
| macOS | Apple Clang | ✅ Hỗ trợ |
| Nhúng | Bất kỳ compiler C++17 nào | ⚠️ Có thể làm hỏng thiết bị của bạn (không phải lỗi của chúng tôi) |

---

## 🐛 Các vấn đề đã biết

| Vấn đề | Trạng thái | Giải pháp |
|--------|-----------|------------|
| `SIGFPE` có thể bị tối ưu hóa | WONTFIX | Dùng `volatile` hoặc `-O0` |
| `SIGILL` không hoạt động trên ARM | WONTFIX | Mua CPU x86 |
| Deadlock không bao giờ trả về (hiển nhiên) | WONTFIX | Đó là nghĩa của deadlock |
| Người dùng cứ mở báo cáo crash | WONTFIX | Đọc README |

---

## 🔮 Kế hoạch tương lai

- [ ] Thêm `SIGDANGLING` (use-after-free)
- [ ] Thêm `SIGCORRUPT` (heap buffer overflow)
- [ ] Thêm `SIGRACE` (data race với sanitizers)
- [ ] Viết bài blog "Tại sao tôi viết thư viện crash chương trình của bạn"
- [ ] Được lên r/programminghorror
- [ ] Hối hận về mọi thứ

---

**Được làm với ❤️ và 💀 bởi một người đáng lẽ nên dùng Rust.**

---

*Những lời cuối trước khi segfault: "Máy tôi chạy được mà."*
