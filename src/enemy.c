#include "enemy.h"
#include "screen.h"

extern int hp;
extern int map_index;
extern int playerDamageBlink;
extern int x;
extern int y;

extern int swordactivetime;
extern int swordstun;
extern int swordDamage;
extern int swordX;
extern int swordY;

extern int BlueOni_x;
extern int BlueOni_y;
extern int BlueOni_spawn;
extern int BlueOni_location;

extern int RedOni_x;
extern int RedOni_y;
extern int RedOni_spawn;
extern int RedOni_location;

extern int BossOni_hp;
extern int BossOni_location;
extern int BossOni_spawn;

//INIMIGOS

int collision(int originx, int originy, int targetx, int targety) {
  if((originx+2 == targetx || originx-2 == targetx || originx == targetx) && (originy+1 == targety || originy-1 == targety || originy == targety)) {
    return 1;
  } else {
    return 0;
  }
}

void enemyMoviment(int *enemy_spawn, int *enemyX, int *enemyY, int *enemy_damageBlink, int *enemy_hp, int enemy_speed, int *enemy_tickCount, int *enemy_stun, int *enemy_damaged, int enemy_damage) { 
  if(*enemy_stun > 0) {
    *enemy_stun -= 1;  
    return;
  }

  if(*enemy_tickCount < enemy_speed) {
      *enemy_tickCount += 1;   
  } else {
      *enemy_tickCount = 0;
      *enemy_damaged = 0;

      //receber dano do player
      if(swordactivetime > 0 && swordstun == 0) {
        if(collision(*enemyX, *enemyY, swordX, swordY) == 1) {
          swordstun = 60;
          swordactivetime = 0;
          screenGotoxy(swordX, swordY);
          printf(" ");
          
          *enemy_damageBlink = 2;
          *enemy_hp -= swordDamage;

          if(*enemy_hp <= 0) {
            *enemy_spawn = 1;
            screenGotoxy(*enemyX, *enemyY);
            printf(" ");
            return;
          }
          *enemy_stun = 30;
          *enemy_damaged = 1;

        }
      }

      // movimentação do inimigo
      if(*enemyX > x && *enemyX-2 != x && *enemyX-2 != swordX) {
          *enemyX -= 2;
      } else if(*enemyX < x && *enemyX+2 != x && *enemyX+2 != swordX) {
          *enemyX += 2;
      } 

      if(*enemyY > y && *enemyY-1 != y && *enemyY-1 != swordY) {
          *enemyY -= 1;
      } else if(*enemyY < y && *enemyY+1 != y && *enemyY+1 != swordY) {
          *enemyY += 1;
      } 

      // dano no player
      if(collision(*enemyX, *enemyY, x, y) == 1) {
        hp -= enemy_damage;
        *enemy_stun = 20;
        playerDamageBlink = 2;
      }
  }
}

int enemyCollision(int x, int y) {
    if(map_index == BlueOni_location) {
        if(BlueOni_spawn == 0 && BlueOni_x == x && BlueOni_y == y) return 1;
        else return 0;
    } else if(map_index == RedOni_location) {
        if(RedOni_spawn == 0 && RedOni_x == x && RedOni_y == y) return 1;
        else return 0;
    } else {
      return 0;
    }
}

void printEnemy(int enemyX, int enemyY, int *enemyPrevX, int *enemyPrevY, int *enemyDamageBlink, int enemyDamaged, int color) {
    screenGotoxy(*enemyPrevX, *enemyPrevY);
    printf(" ");


    screenGotoxy(enemyX, enemyY);

    if(*enemyDamageBlink == 0) {    
      if(enemyDamaged != 0) screenSetColor(YELLOW, DARKGRAY); 
      else if(color == 1)screenSetColor(BLUE, DARKGRAY);
      else if(color == 2)screenSetColor(RED, DARKGRAY);

      printf("鬼");
    } else {
      printf(" ");
      *enemyDamageBlink -= 1;
    }
    *enemyPrevX = enemyX;
    *enemyPrevY = enemyY;

}

//BOSS

void BossMoviment(int *boss_spawn, int *bossX, int *bossY, int *boss_damageBlink, int *boss_hp, int boss_speed, int *boss_tickCount, int *boss_stun, int *boss_damaged, int boss_damage) { 
    if (*boss_stun > 0) {
        *boss_stun -= 1;  
        return;
    }
    if (*boss_tickCount < boss_speed) {
        *boss_tickCount += 1;   
    } else {
        *boss_tickCount = 0;
        *boss_damaged = 0;

        // Receber dano do player
        if (swordactivetime > 0 && swordstun == 0) {
            if ((Boss_collision(*bossX+2, *bossY, swordX, swordY) == 1)||(Boss_collision(*bossX-2, *bossY, swordX, swordY) == 1)||(Boss_collision(*bossX, *bossY+1, swordX, swordY) == 1)||(Boss_collision(*bossX, *bossY-1, swordX, swordY) == 1) ) {
                swordstun = 40;
                swordactivetime = 0;
                screenGotoxy(swordX, swordY);
                printf(" ");

                *boss_damageBlink = 2;
                *boss_hp -= swordDamage;

                if (*boss_hp <= 0) {
                    *boss_spawn = 1;
                    
                    screenGotoxy(*bossX-2, *bossY-1);
                    printf("      ");
                    screenGotoxy(*bossX-2, *bossY);
                    printf("      ");
                    screenGotoxy(*bossX-2, *bossY+1);
                    printf("      ");
                    return;
                }
                *boss_stun = 30;
                *boss_damaged = 1;
                return;
            }
        }
        // Movimentação do Boss
        int dx = x - *bossX;
        int dy = y - *bossY;

        if (dx > 0 && Boss_collision(*bossX + 2, *bossY, x, y)==0 && Boss_collision(*bossX + 2, *bossY, swordX, swordY)==0) {
            *bossX += 2;
        } else if (dx < 0 && Boss_collision(*bossX - 2, *bossY, x, y)==0 && Boss_collision(*bossX -2, *bossY, swordX, swordY)==0) {
            *bossX -= 2;
        } 

        if (dy > 0 && Boss_collision(*bossX, *bossY + 1, x, y)==0 && Boss_collision(*bossX , *bossY+1, swordX, swordY)==0) {
            *bossY += 1;
        } else if (dy < 0 && Boss_collision(*bossX, *bossY - 1, x, y)==0 && Boss_collision(*bossX, *bossY-1, swordX, swordY)==0) {
            *bossY -= 1;
        } 

        // Verificar dano no player
        if (Boss_collision(*bossX+2, *bossY, x, y) == 1) {
                    hp -= boss_damage;
                    *boss_stun = 35;
                    playerDamageBlink = 2;
                }
        if (Boss_collision(*bossX-2, *bossY, x, y) == 1) {
                    hp -= boss_damage;
                    *boss_stun = 35;
                    playerDamageBlink = 2;
                }
        if (Boss_collision(*bossX, *bossY+1, x, y) == 1) {
                    hp -= boss_damage;
                    *boss_stun = 35;
                    playerDamageBlink = 2;
                }
        if (Boss_collision(*bossX, *bossY-1, x, y) == 1) {
                    hp -= boss_damage;
                    *boss_stun = 35;
                    playerDamageBlink = 2;
                }
        
    }
}

int Boss_collision(int originx, int originy, int x, int y) {
    if((originx+2 >= x && originx-2 <= x) && (originy+1 >= y && originy-1 <= y) && map_index == BossOni_location && BossOni_hp > 0) {
        return 1;
    } else {
        return 0;
    }
}



void printBoss(int bossX, int bossY, int *bossPrevX, int *bossPrevY, int *bossDamageBlink, int bossDamaged) {
    // Limpar a posição anterior do Boss
    screenGotoxy(*bossPrevX-2, *bossPrevY-1);
    printf("      ");
    screenGotoxy(*bossPrevX-2, *bossPrevY);
    printf("      ");
    screenGotoxy(*bossPrevX-2, *bossPrevY+1);
    printf("      ");

    if(*bossDamageBlink == 0) {    
      if(bossDamaged != 0) screenSetColor(YELLOW, DARKGRAY); 
      else screenSetColor(MAGENTA, DARKGRAY);
    }

    if(BossOni_spawn == 0) {
        screenGotoxy(bossX-2, bossY-1);
        printf("≊ 王 ≊");
        screenGotoxy(bossX-2, bossY);
        printf("⌈ 鬼 ⌉");
        screenGotoxy(bossX-2, bossY+1);
        printf("  ⌈ ⌉");
    }

    if (*bossDamageBlink > 0) {
        *bossDamageBlink -= 1;
    }

    *bossPrevX = bossX;
    *bossPrevY = bossY;
}
