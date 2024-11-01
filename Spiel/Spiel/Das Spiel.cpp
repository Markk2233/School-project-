#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define   proccess_button(b, vk); \
 case vk: {\
input.buttons[b].changed = is_down != input.buttons[b].is_down; \
input.buttons[b].is_down = is_down;\
            } break;



struct Platform {

  float positionX, positionY, half_sizeX, half_sizeY, Pcolor;
  

  Platform(float platform_positionX, float platform_positionY, float platform_half_sizeX, 
    float platform_half_sizeY, float color, float plPosXd, float plPosYd) {


    positionX = platform_positionX;
    positionY = platform_positionY;
    half_sizeX = platform_half_sizeX;
    half_sizeY = platform_half_sizeY;
    Pcolor = color;

    Draw(plPosXd, plPosYd);
  }


  void Draw(float posXd, float posYd) {
    positionX += posXd;
    positionY += posYd;
    draw_rect(positionX, positionY, half_sizeX, half_sizeY, Pcolor);
    
  }

};

bool isJumping = false;
float player_half_sizeY = 6.f, player_half_sizeX = 3.f;
float plPosXd = 0.f, plPosYd = 0.f, dplPosYd = 0.f;
float velocityY = 0.f;

static void simulate_game(Input *input, float dt) {
  clear_screen(0xD15EAB);
  float platform_ddpX = 0.f, 
        gravity = 100.f,
        platform_ddpY = 0.f;

  if (is_down(BUTTON_RIGHT)) {
    platform_ddpX -= 50;
  }
  if (is_down(BUTTON_LEFT)) {
    platform_ddpX += 50;
  }
  if (pressed(BUTTON_UP) && !isJumping) {
      platform_ddpY -= 38000;
  } 

  isJumping = true;

  Platform ground(0, -45, (float)render_state.width, 35, 0x996699, plPosXd, plPosYd);

  if (ground.half_sizeY + ground.positionY  > -player_half_sizeY) {
    isJumping = false;
    velocityY = 0.f;
    gravity = 0.f;
  }



  Platform obstacle(212, 80, 2, 5, 0xE51717, plPosXd, plPosYd);

  if (obstacle.half_sizeY + obstacle.positionY > -player_half_sizeY &&
    obstacle.positionY - obstacle.half_sizeY < player_half_sizeY &&
    (obstacle.positionX + obstacle.half_sizeX > -player_half_sizeX &&
      obstacle.positionX - obstacle.half_sizeX < player_half_sizeX)) {
    plPosXd = 0.f;
    plPosYd = 0.f;
  }

  Platform obstPlatform(220, 87, 10, 2, 0x996F99, plPosXd, plPosYd);


  Platform obstacle1(212, 47, 2, 5, 0xE51717, plPosXd, plPosYd);

  if (obstacle1.half_sizeY + obstacle1.positionY > -player_half_sizeY &&
    obstacle1.positionY - obstacle1.half_sizeY < player_half_sizeY &&
    (obstacle1.positionX + obstacle1.half_sizeX > -player_half_sizeX &&
      obstacle1.positionX - obstacle1.half_sizeX < player_half_sizeX)) {
    plPosXd = 0.f;
    plPosYd = 0.f;
  }

  Platform obstacle2(50, 42, 2, 5, 0xE51717, plPosXd, plPosYd);

  if (obstacle2.half_sizeY + obstacle2.positionY > -player_half_sizeY &&
    obstacle2.positionY - obstacle2.half_sizeY < player_half_sizeY &&
    (obstacle2.positionX + obstacle2.half_sizeX > -player_half_sizeX &&
      obstacle2.positionX - obstacle2.half_sizeX < player_half_sizeX)) {
    plPosXd = 0.f;
    plPosYd = 0.f;
  }


  Platform platform1(-30, 8, 10, 2, 0x996699, plPosXd, plPosYd);

 
  if (platform1.half_sizeY + platform1.positionY > -player_half_sizeY && 
    platform1.positionY - platform1.half_sizeY < player_half_sizeY &&
    (platform1.positionX + platform1.half_sizeX > -player_half_sizeX &&
      platform1.positionX - platform1.half_sizeX < player_half_sizeX)) {
    
    isJumping = false;
   
 if (platform1.positionX < 0) {
    if (platform1.positionY < -7) {
      velocityY = 0.f;
      gravity = 0.f;
    }
    else {
      platform_ddpX -= 50.f;
    }

    if (platform1.positionY - platform1.half_sizeY > 0) {
      velocityY = 0.f;
      platform_ddpY += 10000;
    }
  }

  if (platform1.positionX > 0) {
    if (platform1.positionY < -7) {
      velocityY = 0.f;
      gravity = 0.f;
    }
    else {
      platform_ddpX += 50.f;
    }

    if (platform1.positionY > 0) {
      velocityY = 0.f;
      platform_ddpY += 10000;
    }
  }
  


  }

  Platform platform2(50, 35, 10, 2, 0x996699, plPosXd, plPosYd);

  if (platform2.half_sizeY + platform2.positionY > -player_half_sizeY && 
    platform2.positionY - platform2.half_sizeY < player_half_sizeY &&
    (platform2.positionX + platform2.half_sizeX > -player_half_sizeX &&
      platform2.positionX - platform2.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform2.positionX < 0) {
      if (platform2.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform2.positionY - platform2.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform2.positionX > 0) {
      if (platform2.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform2.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }


  }


  Platform platform4(-68, 27, 10, 2, 0x996699, plPosXd, plPosYd);

  if ((platform4.half_sizeY + platform4.positionY > -player_half_sizeY &&
    platform4.positionY - platform4.half_sizeY < player_half_sizeY) &&
    (platform4.positionX + platform4.half_sizeX > -player_half_sizeX &&
      platform4.positionX - platform4.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform4.positionX < 0) {
      if (platform4.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform4.positionY - platform4.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform4.positionX > 0) {
      if (platform4.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform4.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }


  }

  Platform platform5(-10, 40, 10, 2, 0x996699, plPosXd, plPosYd);

  if (platform5.half_sizeY + platform5.positionY > -player_half_sizeY &&
    platform5.positionY - platform5.half_sizeY < player_half_sizeY &&
    (platform5.positionX + platform5.half_sizeX > -player_half_sizeX &&
      platform5.positionX - platform5.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform5.positionX < 0) {
      if (platform5.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform5.positionY - platform5.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform5.positionX > 0) {
      if (platform5.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform5.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }
  }

  Platform platform6(100, 49, 10, 2, 0x996699, plPosXd, plPosYd);

  if (platform6.half_sizeY + platform6.positionY > -player_half_sizeY &&
    platform6.positionY - platform6.half_sizeY < player_half_sizeY &&
    (platform6.positionX + platform6.half_sizeX > -player_half_sizeX &&
      platform6.positionX - platform6.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform6.positionX < 0) {
      if (platform6.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform6.positionY - platform6.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform6.positionX > 0) {
      if (platform6.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform6.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

  }

  Platform platform7(160, 30, 10, 2, 0xFFFF23, plPosXd, plPosYd);

  if (platform7.half_sizeY + platform7.positionY > -player_half_sizeY &&
    platform7.positionY - platform7.half_sizeY < player_half_sizeY &&
    (platform7.positionX + platform7.half_sizeX > -player_half_sizeX &&
      platform7.positionX - platform7.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform7.positionX < 0) {
      if (platform7.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
        platform_ddpY *= 1.4;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform7.positionY - platform7.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform7.positionX > 0) {
      if (platform7.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform7.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

  }

  Platform platform8(220, 40, 10, 2, 0x996699, plPosXd, plPosYd);

  if (platform8.half_sizeY + platform8.positionY > -player_half_sizeY &&
    platform8.positionY - platform8.half_sizeY < player_half_sizeY &&
    (platform8.positionX + platform8.half_sizeX > -player_half_sizeX &&
      platform8.positionX - platform8.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform8.positionX < 0) {
      if (platform8.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform8.positionY - platform8.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform8.positionX > 0) {
      if (platform8.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform8.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

  }

  Platform platform9(290, 50, 25, 2, 0xFFAA00, plPosXd, plPosYd);

  if (platform9.half_sizeY + platform9.positionY > -player_half_sizeY &&
    platform9.positionY - platform9.half_sizeY < player_half_sizeY &&
    (platform9.positionX + platform9.half_sizeX > -player_half_sizeX &&
      platform9.positionX - platform9.half_sizeX < player_half_sizeX)) {
    isJumping = false;

    if (platform9.positionX < 0) {
      if (platform9.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX -= 50.f;
      }

      if (platform9.positionY - platform9.half_sizeY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

    if (platform9.positionX > 0) {
      if (platform6.positionY < -7) {
        velocityY = 0.f;
        gravity = 0.f;
      }
      else {
        platform_ddpX += 50.f;
      }

      if (platform9.positionY > 0) {
        velocityY = 0.f;
        platform_ddpY += 10000;
      }
    }

  }



  Platform WinSquare(290, 62, 10, 10, 0xE51717, plPosXd, plPosYd);
  Platform Outline1(280, 62, 1, 10, 0xFFAA00, plPosXd, plPosYd);
  Platform Outline2(300, 62, 1, 10, 0xFFAA00, plPosXd, plPosYd);
  Platform Outline3(290, 71, 10, 1, 0xFFAA00, plPosXd, plPosYd);


 

  velocityY = velocityY + (platform_ddpY + gravity) * dt;
  plPosXd = plPosXd + platform_ddpX * dt;
  plPosYd = plPosYd + velocityY * dt + (platform_ddpY + gravity) * dt * dt * 0.5;



  draw_rect(0, 0, player_half_sizeX, player_half_sizeY, 0xCFF46E);

  if (WinSquare.half_sizeY + WinSquare.positionY > -player_half_sizeY &&
    WinSquare.positionY - WinSquare.half_sizeY < player_half_sizeY &&
    (WinSquare.positionX + WinSquare.half_sizeX > -player_half_sizeX &&
      WinSquare.positionX - WinSquare.half_sizeX + 2.f < player_half_sizeX)) {


    MessageBox(NULL, L"You won!", L"Victory", MB_OK);
    running = false;

  }


}


