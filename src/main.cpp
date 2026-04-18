#include "raylib.h"
#include <vector>
#include <algorithm>
#include <cmath>

#include "core/Entity.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "skill/Skill.h"
#include "bullet/Bullet.h"
using namespace std;

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1, dy = y2 - y1;
    return sqrt(dx*dx + dy*dy);
}

void removeEntity(vector<Entity*>& entities, Entity* entity) {
    entities.erase(remove(entities.begin(), entities.end(), entity), entities.end());
}

void removeEnemy(vector<Entity*>& entities, vector<Enemy*>& enemies, int idx) {
    removeEntity(entities, enemies[idx]);
    delete enemies[idx];
    enemies.erase(enemies.begin() + idx);
}

int main() {
    InitWindow(800, 600, "PIXEL SURVIVOR");
    SetTargetFPS(60);

    Player player;
    vector<Entity*> entities;
    vector<Enemy*> enemies;
    vector<Bullet*> bullets;

    entities.push_back(&player);
    Skill* skill = new Skill(&player);
    entities.push_back(skill);

    while (!WindowShouldClose()) {
        if (player.getHp() <= 0) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAME OVER", 280, 250, 40, RED);
            DrawText(TextFormat("Score: %d", player.getExp()), 350, 320, 20, WHITE);
            EndDrawing();
            continue;
        }

        // Update
        for (auto e : entities) e->update();

        // Spawn enemies
        if (GetRandomValue(0,100)<3) {
            Enemy* e = new Enemy(&player);
            enemies.push_back(e);
            entities.push_back(e);
        }

        // Shoot bullets
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            Bullet* b = new Bullet(player.getX(), player.getY(), m.x, m.y);
            bullets.push_back(b);
            entities.push_back(b);
        }

        // Bullet-enemy collisions
        for (size_t i = 0; i < enemies.size(); i++) {
            for (size_t j = 0; j < bullets.size(); j++) {
                if (distance(bullets[j]->getX(), bullets[j]->getY(), 
                            enemies[i]->getX(), enemies[i]->getY()) < 15) {
                    enemies[i]->takeDamage(20);
                    bullets[j]->setX(-1000);
                    
                    if (enemies[i]->getHp() <= 0) {
                        player.addExp(10);
                        removeEntity(entities, bullets[j]);
                        removeEnemy(entities, enemies, i);
                        i--;
                        break;
                    }
                }
            }
        }

        // Skill-enemy collisions
        for (int i = (int)enemies.size() - 1; i >= 0; i--) {
            bool is_enemy_hit = false;

            // Check collision with each orbiting ball of the skill
            for (int j = 0; j < skill->getNumParticles(); j++) {
                
                // calculate the position of the ball 
                
                float p_angle = skill->getAngle() + (j * 2.0f * PI / skill->getNumParticles());
                float p_x = skill->getX() + cos(p_angle) * skill->getRadius();
                float p_y = skill->getY() + sin(p_angle) * skill->getRadius();

                // Check collision between ball j and enemy i
                if (distance(p_x, p_y, enemies[i]->getX(), enemies[i]->getY()) < 15) {
                    enemies[i]->takeDamage((int)skill->getDamage()); 
                    is_enemy_hit = true;
                    break; // Nếu quái đã trúng 1 viên rồi thì không cần check các viên khác trong cùng frame
                }
            }

            // Nếu quái chết sau khi trúng bi
            if (is_enemy_hit && enemies[i]->getHp() <= 0) {
                player.addExp(10); // +10 EXP for each monster killed.
                removeEnemy(entities, enemies, i); // delete enemy and remove from entities list
            }
        }
                

        // Player-enemy collisions
        for (auto enemy : enemies) {
            if (distance(player.getX(), player.getY(), 
                        enemy->getX(), enemy->getY()) < 20) {
                player.takeDamage(1);
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto e : entities) e->draw();
        
        DrawFPS(10, 10);
        DrawText(TextFormat("HP: %d", player.getHp()), 10, 30, 20, WHITE);
        DrawText(TextFormat("EXP: %d", player.getExp()), 10, 60, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
