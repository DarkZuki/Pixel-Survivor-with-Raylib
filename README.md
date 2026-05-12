# Arcane Rampage

## Giới thiệu đề tài

`Arcane Rampage` là game sinh tồn 2D được phát triển bằng C++ với thư viện Raylib. Trong game, người chơi điều khiển nhân vật chiến đấu với các đợt quái xuất hiện liên tục, thu thập kinh nghiệm để tăng cấp và lựa chọn nâng cấp trong quá trình chơi.

Dự án được tổ chức theo từng hệ thống riêng như người chơi, quái, boss, vũ khí, kỹ năng, vật phẩm, wave và nâng cấp. Mỗi phần đảm nhiệm một nhóm chức năng cụ thể để tạo thành vòng lặp gameplay hoàn chỉnh.

## Mô tả cách chơi

Khi bắt đầu, game hiển thị màn hình tiêu đề. Người chơi nhấn `Space` để vào phần chọn độ khó, sau đó chọn bằng phím `1`, `2` hoặc `3`.

Sau khi vào trận, người chơi di chuyển nhân vật trên bản đồ, né tránh quái và tấn công để sống sót qua từng wave. Quái bị tiêu diệt sẽ rơi ra vật phẩm kinh nghiệm hoặc vật phẩm hồi máu. Khi nhận đủ kinh nghiệm, người chơi tăng cấp và hệ thống sẽ mở menu nâng cấp để chọn thêm vũ khí mới, kỹ năng mới hoặc tăng cấp những gì đang có.

Độ khó của game tăng dần theo thời gian và theo wave. Ở các giai đoạn sau, số lượng quái nhiều hơn, chỉ số mạnh hơn và cuối cùng sẽ xuất hiện boss ở wave cuối. Người chơi chiến thắng khi vượt qua wave cuối và tiêu diệt boss.

## Thành phần chính trong game

- Người chơi có thể di chuyển, nhận sát thương, tăng cấp và tích lũy điểm số
- Hệ thống quái gồm nhiều loại khác nhau, bao gồm quái thường, quái đánh xa và boss
- Vũ khí có thể được mở khóa và nâng cấp trong trận
- Kỹ năng hỗ trợ chiến đấu được mở khóa thông qua hệ thống nâng cấp
- Wave quyết định nhịp độ spawn quái và mức độ khó của trận đấu
- Vật phẩm giúp hồi máu hoặc cộng kinh nghiệm cho người chơi

## Điều khiển

- `W`, `A`, `S`, `D`: di chuyển nhân vật
- `Chuột trái`: tấn công bằng vũ khí hiện tại
- `1`, `2`: chuyển giữa các ô vũ khí đang trang bị
- `Esc`: tạm dừng hoặc tiếp tục game
- `Space`: bắt đầu từ màn hình tiêu đề
- `1`, `2`, `3`: chọn độ khó trước khi vào trận

## Cấu trúc thư mục chính

```text
src/
|- main.cpp
|- boss/
|- bullet/
|- core/
|- enemy/
|- Item/
|- player/
|- skill/
|- upgrade/
|- wave/
`- weapon/
```

## Build và chạy

### Yêu cầu

- Trình biên dịch C++ hỗ trợ C++14
- Raylib
- `make` hoặc `mingw32-make`
- Visual Studio Code nếu muốn dùng task có sẵn trong `.vscode/`

### Build bằng VS Code

- Chạy task `build debug`
- Chạy task `build release`
- Nhấn `F5` để build và chạy với cấu hình debug

### Build bằng dòng lệnh trên Windows

```bash
C:/raylib/w64devkit/bin/mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main BUILD_MODE=DEBUG
```

## Tài nguyên sử dụng

Game hiện nạp tài nguyên từ các thư mục:

- `Graphics/`
- `Sounds/`

## Ghi chú

- Cửa sổ game hiện được khởi tạo với kích thước `1920 x 1040`
- Tên hiển thị của cửa sổ game hiện tại là `Arcane Rampage`
