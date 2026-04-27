#include "SkillLevel.h"
#include "Skill.h" // Để dùng enum SkillType

const char* getSkillName(int skillType) {
    switch (skillType) {
        case (int)SkillType::LASER_BEAM:     return "Tia Laser";
        case (int)SkillType::HAMMER:         return "Búa Thor";
        case (int)SkillType::THUNDER_STRIKE: return "Sét Đánh";
        case (int)SkillType::SHIELD:         return "Khiên Năng Lượng";
        case (int)SkillType::SHURIKEN:       return "Phi Tiêu";
        case (int)SkillType::AUTO_BALLS:     return "Quả Cầu Quỹ Đạo";
        default: return "Kỹ Năng Lạ";
    }
}

SkillLevelData getSkillLevelData(int skillType, int level) {
    SkillLevelData data;
    data.level = level;
    data.isSpecial = (level == 10);
    data.damageBonus = 0.0f;
    data.rangeBonus = 0.0f;
    data.cooldownMult = 1.0f;
    data.particleBonus = 0;
    
    data.name = "Nâng Cấp";
    data.description = "Cải thiện sức mạnh kỹ năng.";

    switch (skillType) {
        case (int)SkillType::LASER_BEAM:
            switch (level) {
                case 1: data.name = "Tia Hội Tụ"; data.description = "Kích hoạt tia Laser cơ bản."; break;
                case 2: data.name = "Điện Áp Cao"; data.description = "Sát thương +20."; break;
                case 3: data.name = "Tầm Xa"; data.description = "Tầm bắn tăng thêm 100."; break;
                case 4: data.name = "Sạc Nhanh"; data.description = "Giảm thời gian hồi chiêu."; break;
                case 5: data.name = "Tia Cực Đại"; data.description = "Tăng độ rộng của tia Laser."; break;
                case 6: data.name = "Tầm Bắn Tỉa"; data.description = "Tầm bắn tăng thêm 200."; break;
                case 7: data.name = "Lõi Nóng Chảy"; data.description = "Sát thương tăng thêm 30."; break;
                case 8: data.name = "Tầm Xa Tuyệt Đối"; data.description = "Tầm bắn tăng thêm 300."; break;
                case 9: data.name = "Xung Điện Nhanh"; data.description = "Hồi chiêu siêu nhanh."; break;
                case 10: data.name = "Quá Tải Kép"; data.description = "Tiến hóa: Bắn 2 tia đối nghịch nhau!"; break;
            }
            break;

        case (int)SkillType::HAMMER:
            switch (level) {
                case 1: data.name = "Búa Sắt"; data.description = "Ném búa gây sát thương."; break;
                case 2: data.name = "Cú Nện Nặng"; data.description = "Sát thương +5."; break;
                case 3: data.name = "Tay Nhẹ"; data.description = "Hồi chiêu x0.9."; break;
                case 4: data.name = "Cạnh Thép"; data.description = "Sát thương +10."; break;
                case 5: data.name = "Ném Tốc Độ"; data.description = "Hồi chiêu x0.8."; break;
                case 6: data.name = "Sức Mạnh Nghiền Nát"; data.description = "Sát thương +10."; break;
                case 7: data.name = "Cú Vung Khổng Lồ"; data.description = "Sát thương +15."; break;
                case 8: data.name = "Tốc Độ Bão Tố"; data.description = "Hồi chiêu x0.7."; break;
                case 9: data.name = "Uy Lực Thần Thánh"; data.description = "Sát thương +20."; break;
                case 10: data.name = "Rìu Chiến Thor"; data.description = "Tiến hóa: Biến thành Rìu với sát thương hủy diệt!"; break;
            }
            break;

        case (int)SkillType::THUNDER_STRIKE:
            switch (level) {
                case 1: data.name = "Tĩnh Điện"; data.description = "Gọi một tia sét từ trời xuống."; break;
                case 2: data.name = "Tăng Điện Áp"; data.description = "Sát thương +10."; break;
                case 3: data.name = "Đoản Mạch"; data.description = "Hồi chiêu x0.9."; break;
                case 4: data.name = "Sóng Năng Lượng"; data.description = "Sát thương +20."; break;
                case 5: data.name = "Sét Đôi"; data.description = "Đánh xuống 2 tia sét cùng lúc."; break;
                case 6: data.name = "Tần Số Cao"; data.description = "Sát thương +30."; break;
                case 7: data.name = "Bão Tố"; data.description = "Hồi chiêu x0.7."; break;
                case 8: data.name = "Thần Sấm"; data.description = "Sát thương +40."; break;
                case 9: data.name = "Điểm Chớp Cháy"; data.description = "Hồi chiêu x0.6."; break;
                case 10: data.name = "Sét Lan"; data.description = "Tiến hóa: Sét đánh nảy lan sang các mục tiêu lân cận!"; break;
            }
            break;

        case (int)SkillType::SHIELD:
            switch (level) {
                case 1: data.name = "Màn Chắn"; data.description = "Tạo một khiên năng lượng nảy quanh màn hình."; break;
                case 2: data.name = "Năng Lượng Ổn Định"; data.description = "Giảm nhẹ hồi chiêu."; break;
                case 3: data.name = "Vệ Sĩ Đôi"; data.description = "Tạo 2 khiên mỗi lần kích hoạt."; break;
                case 4: data.name = "Bán Kính Lớn"; data.description = "Kích thước khiên tăng thêm."; break;
                case 5: data.name = "Rào Chắn Nhanh"; data.description = "Hồi chiêu x0.8."; break;
                case 6: data.name = "Tam Khiên"; data.description = "Tạo 3 khiên mỗi lần kích hoạt."; break;
                case 7: data.name = "Vạn Lý Trường Thành"; data.description = "Kích thước khiên cực đại."; break;
                case 8: data.name = "Siêu Cấp Bảo Vệ"; data.description = "Hồi chiêu x0.7."; break;
                case 9: data.name = "Cạnh Plasma"; data.description = "Sát thương +20."; break;
                case 10: data.name = "Khiên Thần Thánh"; data.description = "Tiến hóa: Tạo 4 khiên nảy bảo vệ tuyệt đối!"; break;
            }
            break;
            
        case (int)SkillType::SHURIKEN:
            switch (level) {
                case 1: data.name = "Lưỡi Dao Xoay"; data.description = "Một phi tiêu xoay quanh người."; break;
                case 2: data.name = "Thêm Phi Tiêu"; data.description = "Số lượng phi tiêu +1."; break;
                case 3: data.name = "Quỹ Đạo Rộng"; data.description = "Vòng xoay rộng hơn."; break;
                case 4: data.name = "Cạnh Sắc Bén"; data.description = "Sát thương +10."; break;
                case 5: data.name = "Bầy Phi Tiêu"; data.description = "Số lượng phi tiêu +1."; break;
                case 6: data.name = "Tốc Độ Cao"; data.description = "Sát thương +10."; break;
                case 7: data.name = "Bão Tố Phi Tiêu"; data.description = "Số lượng phi tiêu +1."; break;
                case 8: data.name = "Quỹ Đạo Đại"; data.description = "Vòng xoay rất rộng."; break;
                case 9: data.name = "Bậc Thầy Vũ Khí"; data.description = "Sát thương +15."; break;
                case 10: data.name = "Vòng Xoay Tuyệt Diệt"; data.description = "Số lượng +2. Quét sạch mọi kẻ thù!"; break;
            }
            break;
    }

    return data;
}
