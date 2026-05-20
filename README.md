# Arcane Rampage

**Arcane Rampage** là game hành động sinh tồn 2D được xây dựng bằng **C++** và **Raylib**. Người chơi điều khiển nhân vật né quái, tiêu diệt kẻ địch, nhặt kinh nghiệm, lên cấp và chọn nâng cấp để mạnh hơn qua từng đợt quái.

Mục tiêu chính: **sống sót càng lâu càng tốt, vượt qua các wave và đánh bại boss cuối.**

---

## Cách chơi nhanh

Trong game, người chơi sẽ liên tục di chuyển trên bản đồ để tránh quái. Quái sẽ xuất hiện theo từng đợt và ngày càng mạnh hơn. Khi tiêu diệt quái, chúng có thể rơi ra vật phẩm kinh nghiệm. Nhặt đủ kinh nghiệm sẽ giúp nhân vật lên cấp.

Mỗi lần lên cấp, game sẽ hiện ra một bảng nâng cấp. Người chơi có thể chọn mở khóa vũ khí mới, mở khóa kỹ năng mới hoặc nâng cấp vũ khí/kỹ năng đã có.

Vòng lặp gameplay cơ bản:

```
Di chuyển → Né quái → Tấn công → Nhặt EXP → Lên cấp → Chọn nâng cấp → Tiếp tục sống sót
```

---

## Điều khiển

| Phím / Chuột | Chức năng |
|---|---|
| `W A S D` | Di chuyển nhân vật |
| Chuột trái | Tấn công bằng vũ khí hiện tại |
| Di chuyển chuột | Chọn hướng tấn công / chọn nâng cấp |
| `1 / 2 / 3` | Chọn độ khó lúc bắt đầu game |
| `ESC` | Tạm dừng / tiếp tục game |
| Click nút Pause | Tạm dừng / tiếp tục game |
| `F5` | Lưu game |
| `F9` | Tải lại game đã lưu |

---

## Một số thuật ngữ dễ hiểu

| Thuật ngữ | Hiểu đơn giản là |
|---|---|
| **Player** | Nhân vật người chơi điều khiển |
| **Enemy** | Quái / kẻ địch |
| **Boss** | Quái mạnh xuất hiện ở cuối game |
| **EXP** | Điểm kinh nghiệm, dùng để lên cấp |
| **Level** | Cấp độ của nhân vật, vũ khí hoặc kỹ năng |
| **Wave** | Đợt quái xuất hiện theo thời gian |
| **Weapon** | Vũ khí người chơi chủ động sử dụng bằng chuột |
| **Skill** | Kỹ năng tự động hoạt động sau khi được mở khóa |
| **Cooldown** | Thời gian hồi chiêu, phải chờ hết mới dùng tiếp được |
| **Range** | Tầm đánh hoặc phạm vi hoạt động |
| **Damage** | Sát thương gây ra cho quái |

---

## Vũ khí hoạt động như thế nào?

**Weapon** là vũ khí chủ động. Người chơi sử dụng weapon bằng chuột trái. Mỗi weapon có cách tấn công riêng, ví dụ đánh gần, bắn theo hướng chuột hoặc tự tìm mục tiêu.

Người chơi có thể mở khóa tối đa **2 weapon** trong một lượt chơi. Weapon có thể được nâng cấp để tăng sát thương, giảm hồi chiêu, tăng tầm đánh hoặc thêm hiệu ứng mạnh hơn.

### Hammer

**Hammer** là vũ khí đánh gần. Khi sử dụng, nó gây sát thương trong khu vực gần nhân vật.

Phù hợp khi:

- Quái áp sát người chơi.
- Người chơi muốn dọn nhóm quái đứng gần.
- Cần một vũ khí đơn giản, dễ hiểu và dễ dùng.

Cách hoạt động:

```
Player dùng Hammer → Hammer đánh vùng quanh player → Quái trong vùng đánh bị mất máu
```

---

### Magic Wand

**Magic Wand** là gậy phép có khả năng tự tìm mục tiêu trong tầm đánh.

Phù hợp khi:

- Người chơi muốn tấn công dễ hơn.
- Không muốn phải ngắm quá chính xác.
- Muốn tập trung né quái trong lúc vũ khí hỗ trợ bắn mục tiêu.

Cách hoạt động:

```
Giữ chuột trái → Magic Wand tìm quái trong tầm → Bắn phép vào mục tiêu
```

---

### Knife

**Knife** là vũ khí phóng dao theo hướng chuột.

Phù hợp khi:

- Người chơi muốn tự điều khiển hướng tấn công.
- Muốn đánh chính xác vào mục tiêu cụ thể.
- Thích lối chơi chủ động hơn.

Cách hoạt động:

```
Người chơi hướng chuột → Bấm/giữ chuột trái → Dao bay theo hướng chuột
```

---

### Spell Book

**Spell Book** là sách phép, dùng để bắn ra đòn phép có khả năng gây sát thương diện rộng.

Phù hợp khi:

- Quái xuất hiện đông.
- Cần dọn nhiều mục tiêu cùng lúc.
- Muốn tạo lượng sát thương lớn trên một vùng.

Cách hoạt động:

```
Dùng Spell Book → Bắn phép → Đòn phép gây sát thương lên quái, có thể ảnh hưởng nhiều mục tiêu
```

---

## Skill hoạt động như thế nào?

**Skill** là kỹ năng tự động. Sau khi được mở khóa, skill sẽ tự hoạt động mà người chơi không cần bấm nút riêng.

Khác với weapon:

- **Weapon**: người chơi chủ động dùng bằng chuột.
- **Skill**: tự chạy theo thời gian, theo vị trí player hoặc theo danh sách quái.

Người chơi có thể mở khóa tối đa **3 skill** trong một lượt chơi. Skill có thể được nâng cấp để tăng sát thương, tăng số lượng đòn đánh, tăng tầm hoạt động, giảm thời gian hồi hoặc mở hiệu ứng đặc biệt.

---

### Laser Beam

**Laser Beam** bắn một tia laser về phía quái gần nhất.

Cách hoạt động:

```
Skill tìm quái gần player nhất → Tính hướng đến quái → Bắn tia laser theo hướng đó
```

Điểm đặc biệt:

- Laser đánh theo đường thẳng.
- Những quái nằm trên đường laser đều có thể bị trúng đòn.
- Khi nâng cấp cao, laser có thể bắn thêm tia ngược hướng.

Dễ hiểu hơn:

> Laser giống như một tia sáng bắn thẳng từ người chơi đến quái gần nhất. Nếu nhiều quái đứng trên cùng đường bắn, chúng đều có thể bị sát thương.

---

### Thunder Strike

**Thunder Strike** gọi sét đánh xuống quái.

Cách hoạt động:

```
Skill chọn ngẫu nhiên quái trên bản đồ → Sét đánh xuống vị trí đó → Quái bị mất máu
```

Điểm đặc biệt:

- Không cần ngắm.
- Có thể đánh trúng quái ở xa.
- Khi nâng cấp, số lượng mục tiêu bị sét đánh có thể tăng.

Dễ hiểu hơn:

> Thunder là kỹ năng tự động chọn quái và giáng sét xuống đầu chúng. Người chơi chỉ cần mở khóa, skill sẽ tự hoạt động.

---

### Shuriken

**Shuriken** tạo phi tiêu quay quanh player.

Cách hoạt động:

```
Phi tiêu quay quanh nhân vật → Quái chạm vào phi tiêu → Quái bị mất máu
```

Điểm đặc biệt:

- Hoạt động liên tục sau khi mở khóa.
- Hữu ích khi quái áp sát từ nhiều phía.
- Khi nâng cấp, số lượng phi tiêu hoặc phạm vi quay có thể tăng.

Dễ hiểu hơn:

> Shuriken giống như một vòng bảo vệ quanh người chơi. Quái càng áp sát thì càng dễ bị phi tiêu chạm trúng.

---

### Shield

**Shield** tạo ra các khiên bay quanh màn hình.

Cách hoạt động:

```
Skill sinh khiên gần player → Khiên bay ra ngoài → Khiên nảy trong khung hình → Chạm quái thì gây sát thương
```

Điểm đặc biệt:

- Khiên có thể nảy lại khi chạm biên màn hình.
- Có thể gây sát thương nhiều lần khi va chạm quái.
- Hữu ích khi quái xuất hiện đông và từ nhiều hướng.

Dễ hiểu hơn:

> Shield là những chiếc khiên bay tự do quanh khu vực người chơi đang nhìn thấy. Khi khiên chạm quái, quái sẽ mất máu.

---

### Hammer Skill

**Hammer Skill** là kỹ năng tự động phóng búa về phía quái gần nhất.

Cách hoạt động:

```
Skill tìm quái gần nhất → Tính hướng bay → Phóng búa từ player về phía đó
```

Điểm đặc biệt:

- Tự tìm mục tiêu gần nhất.
- Khi nâng cấp, có thể phóng nhiều búa theo hình quạt.
- Ở cấp cao, búa có thể chuyển thành rìu để thể hiện nâng cấp mạnh hơn.

Dễ hiểu hơn:

> Hammer Skill giống như một đòn phóng búa tự động. Người chơi không cần ngắm, skill sẽ tự chọn quái gần nhất để tấn công.

---

## Nâng cấp hoạt động như thế nào?

Khi nhặt đủ EXP, player sẽ lên cấp. Mỗi lần lên cấp, game hiện ra các lựa chọn nâng cấp ngẫu nhiên.

Các lựa chọn có thể là:

- Mở khóa weapon mới.
- Nâng cấp weapon đang có.
- Mở khóa skill mới.
- Nâng cấp skill đang có.

Lưu ý:

> Player lên cấp không có nghĩa là mọi weapon và skill đều mạnh lên. Người chơi phải chọn nâng cấp cụ thể trong menu nâng cấp.

Ví dụ:

```
Lên cấp → Chọn nâng Laser → Laser mạnh hơn
Lên cấp → Chọn mở khóa Knife → Người chơi có thêm weapon mới
Lên cấp → Chọn nâng Shuriken → Shuriken có thể gây nhiều sát thương hơn hoặc có thêm phi tiêu
```

---

## Quái và boss

Game có nhiều loại quái với cách gây áp lực khác nhau:

| Loại quái | Đặc điểm |
|---|---|
| Normal Enemy | Quái cơ bản, chỉ số cân bằng |
| Fast Enemy | Máu thấp nhưng chạy nhanh |
| Tank Enemy | Máu cao, khó tiêu diệt hơn |
| Ranged Enemy | Giữ khoảng cách và bắn đạn vào player |

Ở wave cuối, boss sẽ xuất hiện. Boss có lượng máu lớn, có giáp giảm sát thương và trở nên nguy hiểm hơn khi máu thấp.

---

## Wave và độ khó

Game tiến triển theo các **wave**. Càng về sau, quái xuất hiện nhiều hơn và nguy hiểm hơn.

Người chơi có thể chọn độ khó khi bắt đầu:

| Độ khó | Mô tả |
|---|---|
| Easy | Dễ tiếp cận, phù hợp để làm quen |
| Hard | Quái mạnh hơn, nhịp game căng hơn |
| Hell | Khó nhất, cần né tốt và chọn nâng cấp hợp lý |

---

## Save / Load

Game hỗ trợ lưu và tải trạng thái chơi:

| Phím | Chức năng |
|---|---|
| `F5` | Lưu game |
| `F9` | Tải lại game đã lưu |

---

## Cấu trúc project

| Thư mục | Nội dung chính |
|---|---|
| `src/player` | Xử lý nhân vật người chơi |
| `src/enemy` | Xử lý quái |
| `src/boss` | Xử lý boss |
| `src/weapon` | Hệ thống weapon |
| `src/skill` | Hệ thống skill |
| `src/upgrade` | Menu nâng cấp |
| `src/wave` | Wave và độ khó |
| `src/Item` | Vật phẩm EXP và hồi máu |
| `Graphics` | Hình ảnh trong game |

---

## Tóm tắt cho người chơi mới

Nếu mới chơi, chỉ cần nhớ:
WASD để di chuyển
Chuột trái để tấn công bằng weapon
Nhặt EXP để lên cấp
Lên cấp thì chọn nâng cấp phù hợp
Weapon là vũ khí chủ động
Skill là kỹ năng tự động
Sống sót qua các wave và đánh boss cuối
