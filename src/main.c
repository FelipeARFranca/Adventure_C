#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#include "enemy.h"
#include "map.h"
#include "player.h"
#include "objects.h"

#include <stdio.h>
#include <string.h>

//mapa
mapa game_map[8];
int map_index = 0;

//inventario
inventory player_inventory;

object katana;
object magatama;
object seitou;

// player
int hp = 6;
int x = 20, y = 20;
char viewside;
int playerDamageBlink = 0;

int swordX, swordY;
int swordactivetime = 0;
int swordstun = 0;
int swordDamage = 1;

// inimigo BlueOni
int BlueOni_spawn = 0;
int BlueOni_location = 1;

int BlueOni_hp = 3;
int BlueOni_speed = 3, BlueOni_tickCount = 0;
int BlueOni_x = 10, BlueOni_y = 10; // x e y iniciais do inimigo
int BlueOni_prevX, BlueOni_prevY;
int BlueOni_damage = 1;

int BlueOni_stun = 0;   
int BlueOni_damageBlink = 0;
int BlueOni_damaged = 0;

// inimigo RedOni
int RedOni_spawn = 0;
int RedOni_location = 2;

int RedOni_hp = 4;
int RedOni_speed = 5, RedOni_tickCount = 0;
int RedOni_x = 20, RedOni_y = 20;
int RedOni_prevX, RedOni_prevY;
int RedOni_damage = 2;

int RedOni_stun = 0;
int RedOni_damageBlink = 0;
int RedOni_damaged = 0;

int BossOni_spawn = 0;
int BossOni_location = 5;

int BossOni_hp = 10;
int BossOni_speed = 3, BossOni_tickCount = 0;
int BossOni_x = 40, BossOni_y = 5; 
int BossOni_prevX, BossOni_prevY;
int BossOni_damage = 2;

int BossOni_stun = 50;
int BossOni_damageBlink = 0;
int BossOni_damaged = 0;


void printxy() {
  screenSetColor(RED, DARKGRAY);
  screenGotoxy(0, 24);
  if (hp == 6){
    printf("      ");
    printf("LIFE: ❤ ❤ ❤ ❤ ❤ ❤");
  }
  else if (hp == 5){
    printf("      ");
    printf("LIFE: ❤ ❤ ❤ ❤ ❤  ");
  }
  else if (hp == 4){
    printf("      ");
    printf("LIFE: ❤ ❤ ❤ ❤    ");
  }
  else if (hp == 3){
    printf("      ");
    printf("LIFE: ❤ ❤ ❤      ");
  }
  else if (hp == 2){
    printf("      ");
    printf("LIFE: ❤ ❤        ");
  }
  else if (hp == 1){
    printf("      ");
    printf("LIFE: ❤          ");
  }
  else if (hp <= 0){
    printf("      ");
    printf("LIFE:             ");
  }
  //screenGotoxy(2, 25);
  //printf("Player | HP: %d | X: %d | Y: %d | S: %c | T: %d ", hp, x, y, viewside, BossOni_stun);
  //screenGotoxy(2, 25);
  //printf("Blue Oni | HP: %d | X: %d | Y: %d   ", BlueOni_hp, BlueOni_x, BlueOni_y);
  //screenGotoxy(2, 26);
  //printf("Red Oni | HP: %d | X: %d | Y: %d   ", RedOni_hp, RedOni_x, RedOni_y);
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(34, 22);
  printf("Key code: %d ", ch);
}

int main() {
  static int ch = 0;

  screenInit(0);
  keyboardInit();
  timerInit(50);
  print_MAP();
  printPlayer(x, y);
  screenUpdate();

  draw_map(); 

  int newX = 40;
  int newY = 19;

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();
    }

    // Update game state (move elements, verify collision, etc)
    if (timerTimeOver() == 1) {

      //W
      if (ch == 119 /*&& y - 1 >= MINY + 1*/ && map_collision(x, y - 1) == 0 && enemyCollision(x, y - 1) == 0 && Boss_collision(BossOni_x, BossOni_y,x, y - 1) == 0){
        if (map_index == 0 && x == 40 && y == 16){
          map_clear();
          map_index = map_change(map_index, x, y - 1);
          newY = 23;
        }
        else if(y == 1) {
          map_clear();
          map_index = map_change(map_index, x, y - 1);
          newY = 23;  
        } else {
          if (map_index == 0 && x == katana.object_x && y - 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 2 && x == magatama.object_x && y - 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          if (map_index == 7 && x == seitou.object_x && y - 1 == seitou.object_y && player_inventory.weapon == 0){
            player_inventory.weapon += 2;
          }
          newY = y - 1;
        }
        viewside = 'U';
        ch = 0;

      //S
      } else if (ch == 115 /*&& y + 1 <= MAXY - 1*/ && map_collision(x, y + 1) == 0 && enemyCollision(x, y + 1) == 0 && Boss_collision(BossOni_x, BossOni_y,x, y + 1) == 0){
        if(y == 23) {
          map_clear();
          map_index = map_change(map_index, x, y + 1);
          if (map_index == 0){
            newY = 16;
          }else {
            newY = 1;
          }
        } else {
          if (map_index == 0 && x == katana.object_x && y + 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 2 && x == magatama.object_x && y + 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          if (map_index == 7 && x == seitou.object_x && y + 1 == seitou.object_y && player_inventory.weapon == 0){
            player_inventory.weapon += 2;
          }
          newY = y + 1;
        }
        viewside = 'D';
        ch = 0;

      //A
      } else if (ch == 97 /*&& x - 2 >= MINX + 1*/ && map_collision(x - 2, y) == 0 && enemyCollision(x - 2, y) == 0 && Boss_collision(BossOni_x, BossOni_y,x - 2,y) == 0) {
        if (x == 0){
          map_clear();
          map_index = map_change(map_index, x - 2, y);
          newX = 80;
        } else {
          if (map_index == 0 && x - 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
              player_inventory.weapon++;
            }
          if (map_index == 2 && x - 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
              player_inventory.key++;
            }
          if (map_index == 7 && x - 2 == seitou.object_x && y == seitou.object_y && player_inventory.weapon == 0){
              player_inventory.weapon += 2;
            }
          newX = x - 2;
        }
        viewside = 'L';
        ch = 0;

      //D
      } else if (ch == 100 /*&& x + 2 < MAXX - 1*/ && map_collision(x + 2, y) == 0 && enemyCollision(x + 2, y) == 0 && Boss_collision(BossOni_x, BossOni_y,x + 2 ,y) == 0) {
        if (x == 80){
          map_clear();
          map_index = map_change(map_index, x + 2, y);
          newX = 0;
        } else {
          if (map_index == 0 && x + 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
              player_inventory.weapon++;
            }
          if (map_index == 2 && x + 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
              player_inventory.key++; 
            }
          if (map_index == 7 && x + 2 == seitou.object_x && y == seitou.object_y && player_inventory.weapon == 0){
              player_inventory.weapon += 2;
            }
          newX = x + 2;
        }
        viewside = 'R';
        ch = 0;
      }
      

      if(swordactivetime > 0 && swordstun == 0) {
          printSword();
          swordactivetime--;
          if(swordactivetime == 0) {
            screenGotoxy(swordX, swordY);
            printf(" ");

            swordX = 0;
            swordY = 0;
          }
      } else if(swordstun > 0) {
        swordstun--;
        if(swordstun == 0) {
          swordX = 0;
          swordY = 0;
        }
      }

      if (player_inventory.weapon != 0 && swordstun == 0){
        if(ch == 32 ) {
          if(swordactivetime == 0) {
            swordactivetime = 15;
            printSword();
            ch = 0;
          }
        }
      }

      if(player_inventory.weapon > 1){
        swordDamage = 4;
      }

      if(BossOni_hp > 0){
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][1] = '1';
        }
      }
      else{
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][1] = '0';
        }
      }

      if (map_index == 5 && newY < 23){
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][23] = '1';
        }
      }

      if (map_index == 3 && x == 22 && y == 12){
        if (player_inventory.weapon == 1 || player_inventory.key == 0){
          screenGotoxy(38, 6);
          screenSetColor(WHITE, DARKGRAY);
          printf("。。。");
        }
        else{
          screenGotoxy(38, 6);
          screenSetColor(WHITE, DARKGRAY);
          printf("左の方へ");
        }
      }
      else{
        screenGotoxy(32, 6);
        printf("                       ");
      }


      if (ch == 44) {
        map_index = 0;
      }
      if (ch == 46) {
        map_clear();
        map_index = 5;
      }

      print_MAP();

      if (map_index == 3){
        screenGotoxy(20, 12);
        if (player_inventory.weapon == 1 || player_inventory.key == 0){
          printf("?");
        }
        else {
          printf("!");
        }
      }
      else{
        screenGotoxy(20, 12);
        printf("  ");
      }

      if (map_index == 0 && player_inventory.weapon != 1){
        print_object(katana.object_index, katana.object_x, katana.object_y);
      }
      else{
        screenGotoxy(katana.object_x, katana.object_y);
        printf(" ");
      }
      if (map_index == 2 && player_inventory.key != 1){
        print_object(magatama.object_index, magatama.object_x, magatama.object_y);
      }
      else{
        screenGotoxy(magatama.object_x, magatama.object_y);
        printf(" ");
      }
      if (map_index == 7 && player_inventory.weapon < 2){
        print_object(seitou.object_index, seitou.object_x, seitou.object_y);
      }
      else{
        screenGotoxy(seitou.object_x, seitou.object_y);
        printf(" ");
      }

      if(BlueOni_spawn == 0 && map_index == BlueOni_location) {
        enemyMoviment(&BlueOni_spawn, &BlueOni_x, &BlueOni_y, &BlueOni_damageBlink, &BlueOni_hp, BlueOni_speed, &BlueOni_tickCount, &BlueOni_stun, &BlueOni_damaged, BlueOni_damage);
        printEnemy(BlueOni_x, BlueOni_y, &BlueOni_prevX, &BlueOni_prevY, &BlueOni_damageBlink, BlueOni_damaged, BlueOni_damage);
      }

      if(RedOni_spawn == 0 && map_index == RedOni_location) {
        enemyMoviment(&RedOni_spawn, &RedOni_x, &RedOni_y, &RedOni_damageBlink, &RedOni_hp, RedOni_speed, &RedOni_tickCount, &RedOni_stun, &RedOni_damaged, RedOni_damage);
        printEnemy(RedOni_x, RedOni_y, &RedOni_prevX, &RedOni_prevY, &RedOni_damageBlink, RedOni_damaged, RedOni_damage);
      }

      if (BossOni_spawn == 0 && map_index == BossOni_location) {
          BossMoviment(&BossOni_spawn, &BossOni_x, &BossOni_y, &BossOni_damageBlink, &BossOni_hp, BossOni_speed, &BossOni_tickCount, &BossOni_stun, &BossOni_damaged, BossOni_damage);
          printBoss(BossOni_x, BossOni_y, &BossOni_prevX, &BossOni_prevY, &BossOni_damageBlink, BossOni_damaged);
      }


      printPlayer(newX, newY);
      printxy();
      print_inv_hud(player_inventory.weapon, player_inventory.key);

      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
