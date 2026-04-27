#ifndef SKILL_LEVEL_H
#define SKILL_LEVEL_H

#include <string>
#include <vector>

// Cấu trúc chứa thông tin chi tiết của từng cấp độ kỹ năng
struct SkillLevelData {
    int level;
    std::string name;        // Tên của cấp độ (VD: "Laser Tăng Cường")
    std::string description; // Mô tả (VD: "Sát thương +10, Tầm xa +50")
    
    // Các chỉ số cộng thêm hoặc thay đổi (để hiển thị nếu cần)
    float damageBonus;
    float rangeBonus;
    float cooldownMult;
    int particleBonus;
    bool isSpecial;          // Đánh dấu nếu là cấp độ tiến hóa (như Lv 10)
};

// Hàm lấy tên gốc của Skill dựa trên enum SkillType (mày dùng int hoặc enum đều được)
const char* getSkillName(int skillType);

// Hàm lấy dữ liệu nâng cấp của một Skill tại một Level cụ thể
SkillLevelData getSkillLevelData(int skillType, int level);

#endif // SKILL_LEVEL_H