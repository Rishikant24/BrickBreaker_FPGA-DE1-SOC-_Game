#define KEY_BASE              0xFF200050
#define   MPCORE_PRIV_TIMER   0xFFFEC600
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030



char seg7[10] =	{0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 
						 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

void write_pixel(int x, int y, short colour);
void clear_screen();
void write_char(int x, int y, char c);
void draw_board(int x);
void draw_brick(int x,int y,int clr1,int clr2);
void draw_ball(int x,int y,int colour);
void draw_crack(int x,int y,int clr1,int clr2);
int move_ball();
void check_collision();

int game_play = 0;
int delay = 6000;



int i,j;

int no_of_bricks=24;
int bricks[50][3];

int board_x=160;
int board_speed=2;
int board_dir=0;

int ball_y=226;
int ball_x=160;
int ball_dx=1;
int ball_dy=-1;

int score = 0;
int lives = 3;
int cur_lives = 3;
int cur_lvl = 1;
int main () {

  if(lives<=0)
  {
    score = 0;
    lives = 3;
    cur_lvl = 1;
  }
  
  int key_capture, status, i;
  volatile int * KEYS_ptr = (int *) KEY_BASE;
  volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;

  volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
	volatile int * HEX5_HEX4_ptr = (int *) HEX5_HEX4_BASE;

clear_screen();
if(game_play == 0)
  	{
  		while(1)
	    {	

	            char *st2 = "Choose Your Difficulty: ";
        		char *st1 = "Press Key 0 for Easy, Key 1 for Medium, Key 2 for Hard, Key 3 for Insane";

                char *st3 = "BRICK BREAKER !!!";

		        int x = 30;
		        while(*st2)
		        {
		          write_char(x, 30, *st2);
		          x++;
		          st2++;
		        }

		         x = 5;
		        while(*st1)
		        {
		          write_char(x, 33, *st1);
		          x++;
		          st1++;
		        }

                x = 34;
		        while(*st3)
		        {
		          write_char(x, 15, *st3);
		          x++;
		          st3++;
		        }

	      key_capture = *(KEYS_ptr);
	        if(key_capture&0x1)
	        { 
	        	*(KEYS_ptr) = key_capture;
	          game_play = 1;
	          delay = 2500000;
	          clear_screen();
	          break;
	        }

	        else if(key_capture&0x2)
	        { 	
	        	*(KEYS_ptr) = key_capture;
	          game_play = 1;
	          delay = 2000000;
	          clear_screen();
	          break;
	        }

	        else if(key_capture&0x4)
	        { 	
	        	*(KEYS_ptr) = key_capture;
	          game_play = 1;
	          delay = 1500000;
	          clear_screen();
	          break;
	        }

            else if(key_capture&0x8)
	        { 	
	        	*(KEYS_ptr) = key_capture;
	          game_play = 1;
	          delay = 1100000;
	          clear_screen();
	          break;
	        }
	    }
  	}


  //populating bricks
  if(lives==3 && cur_lvl == 1)
  {
    no_of_bricks=24;
    
    for(i=0;i<no_of_bricks;i++)  // 3 lines of bricks
    {
      bricks[i][0]=45+(i%8)*30;
      bricks[i][1]=30+(i/8)*12;
      bricks[i][2]=1;
    }
  }

  else if(cur_lvl == 2 && lives == cur_lives)
  {
    no_of_bricks=30;
    {
      for(i=0;i<no_of_bricks/3;i++)  // 3 lines of bricks
      {
        bricks[i][0]=145+(i%2)*30;
        bricks[i][1]=30+(i/2)*12;
        bricks[i][2]=1;
      }
      for(i=no_of_bricks/3;i<2*no_of_bricks/3;i++)  // 3 lines of bricks
      {
        bricks[i][0]=260+(i%2)*30;
        bricks[i][1]=80+(i/2 - 5)*12;
        bricks[i][2]=2;
      }
      for(i=2*no_of_bricks/3;i<no_of_bricks;i++)  // 3 lines of bricks
      {
        bricks[i][0]=30+(i%2)*30;
        bricks[i][1]=80+(i/2 -10)*12;
        bricks[i][2]=2;
      }
      
    }
  }

  clear_screen();
  
  for(i=0;i<no_of_bricks;i++) //print the bricks
    {
      if(bricks[i][2]==1 && cur_lvl == 1)
      {
        draw_brick(bricks[i][0],bricks[i][1],0x2222,0x6666);
      }
      else if(bricks[i][2]==1 && cur_lvl == 2)
      {
      	draw_brick(bricks[i][0],bricks[i][1],0x3333,0x8888);
      	draw_crack(bricks[i][0],bricks[i][1],0,0);
      }
      else if(bricks[i][2] == 2)
      {
      	draw_brick(bricks[i][0],bricks[i][1],0x3333,0x8888);
      }
    }

  //  Ball ball = {159, 200, 7, 1, 3, 3, 0, 0xFFFF, 0};
  //  draw_ball(&ball, 0xffff);

  *A9_priv_timer_ptr = 20000;  // timeout = 1/(200 MHz) x 2x10^8 = 1 sec
  *(A9_priv_timer_ptr+2) = 0b011; 
  while(1) {




    
  *HEX3_HEX0_ptr = seg7[score/100] << 16;
  *HEX3_HEX0_ptr |= seg7[(score/10)%10] << 8;
	*HEX3_HEX0_ptr |= seg7[score%10];

	*HEX5_HEX4_ptr = seg7[lives] ;
*HEX5_HEX4_ptr |= seg7[0]<<8 ;

    key_capture = *(KEYS_ptr);
    if(key_capture & 0x1) {
      if(board_x+30+board_speed<=319){
        for(i=230;i<240;i++)
          {
            for(j=board_x-30;j<board_x-(30-board_speed-1);j++)
            {
              write_pixel(j,i,0);
            }
          }
        board_x += board_speed;
        board_dir=1;
      }
      
    } 
    else if(key_capture & 0x2) {
      if(board_x-30>=0){
        for(i=230;i<240;i++)
          {
            for(j=board_x+30;j>board_x+30-board_speed-1;j--)
            {
              write_pixel(j,i,0);
            }
          }

          board_x -= board_speed;
      }
      board_dir=-1;
    }
    else if((key_capture & 0x4)==0)
    {
      board_dir=0;
    }
    // key_capture = *(KEYS_ptr+3);
    // if(key_capture & 0x8)
    // {
    //     *(KEYS_ptr+3)=key_capture;
    //     while(1)
    //     {
    //         key_capture = *(KEYS_ptr+3);
    //         if(key_capture & 0x8)
    //         {
    //             break;
    //         }
    //     }
    // }
      draw_board(board_x);
      check_collision();

      if(score==24 && cur_lvl == 1)
      { 
        cur_lvl++;
        cur_lives = lives;
        char *st1 = "Press Key 2 to go to Level 2!!!";
        char *st2 = "Congratulations !!!";

        int x = 30;
        while(*st2)
        {
          write_char(x, 30, *st2);
          x++;
          st2++;
        }

         x = 25;
        while(*st1)
        {
          write_char(x, 33, *st1);
          x++;
          st1++;
        }

        
        while(1)
    	{
      	key_capture = *(KEYS_ptr);
        if(key_capture&0x4)
        { 
          *(KEYS_ptr) = key_capture;
          board_x=160;
          ball_x=160;
          ball_y=226;
          main();
        }
    }
    }

    if(score==54 && cur_lvl == 2)
      { 
        cur_lvl = 1;
        cur_lives = 3;
        lives = 3;
        score = 0;
        char *st1 = "Level 3 in progress...  Press Key 2 to return to home screen and play again";
        char *st2 = "Congratulations on clearing Level 2!!!";

        int x = 30;
        while(*st2)
        {
          write_char(x, 30, *st2);
          x++;
          st2++;
        }

         x = 10;
        while(*st1)
        {
          write_char(x, 33, *st1);
          x++;
          st1++;
        }

        
        while(1)
    	{
      	key_capture = *(KEYS_ptr);
        if(key_capture&0x4)
        { 
        game_play = 0;
          *(KEYS_ptr) = key_capture;
          board_x=160;
          ball_x=160;
          ball_y=226;
          main();
        }
    }
    }

    
      
      if(move_ball())
      { 
        lives--;
        break;
      }
      // update_ball(&ball);
      // add a delay of 10000 iterations before updating the screen again
      for(i = 0; i < delay; i++) {
        // do nothing
      }

  }
  while(1)
  {//key_capture = *(KEYS_ptr);


    if(lives>0)
    {
          char *st = "Press Key 2 to Respawn";

        int x = 30;
        while(*st)
        {
          write_char(x, 30, *st);
          x++;
          st++;
        }

    while(1)
    {
      key_capture = *(KEYS_ptr);
        if(key_capture&0x4)
        { 
          *(KEYS_ptr) = key_capture;
          board_x=160;
          ball_x=160;
          ball_y=226;
          main();
        }
    }
    }
    else
    {
        char *st1 = "Press Key 2 to Restart";
        char *st2 = "Game Over !!!";
	
	game_play = 0;
        int x = 30;
        while(*st2)
        {
          write_char(x, 30, *st2);
          x++;
          st2++;
        }

         x = 25;
        while(*st1)
        {
          write_char(x, 33, *st1);
          x++;
          st1++;
        }
	
    while(1)
    {
      key_capture = *(KEYS_ptr+3);
        if(key_capture&0x4)
        { 
          *(KEYS_ptr+3) = key_capture;
          board_x=160;
          ball_x=160;
          ball_y=226;
          main();
        }
    }
    }

    while(1)
    {
      key_capture = *(KEYS_ptr+3);
        if(key_capture&0x4)
        { 
          *(KEYS_ptr+3) = key_capture;
          board_x=160;
          ball_x=160;
          ball_y=226;
          main();
        }
    }

  }

  return 0;
}

/* set a single pixel on the screen at x,y
 * x in [0,319], y in [0,239], and colour in [0,65535]
 */
void write_pixel(int x, int y, short colour) {
  volatile short *vga_addr=(volatile short*)(0xC8000000 + (y<<10) + (x<<1));
  *vga_addr=colour;
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
	  write_pixel(x,y,0);
	}
  }

  for (x = 0; x < 80; x++) {
    for (y = 0; y < 60; y++) {
	  
    write_char(x, y, ' ');
	}
  }
}

/* write a single character to the character buffer at x,y
 * x in [0,79], y in [0,59]
 */
void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0xC9000000 + (y<<7) + x);
  *character_buffer = c;
}

void draw_board(int x){
  for(i=230;i<240;i++)
  {
    for(j=x-30;j<x+30;j++)
    {
      write_pixel(j,i,0xf5f5);
    }
  }

}

void draw_crack(int x, int y, int clr1, int clr2)
{
    int i = x - 15, j = y - 6;
    while (i <= x + 15 && j <= y + 6)
    {
        if (i == x || j == y)
        {
            write_pixel(i, j, 0);
            i++;
            j++;
        }
        else if ((i - x) % 2 == 0)
        {
            write_pixel(i, j, 0);
            i++;
        }
        else
        {
            write_pixel(i, j, 0);
            i++;
            j++;
      }
    }
}

void draw_brick(int x,int y,int clr1,int clr2)
{
  for(i=x-15;i<=x+15;i++)
  {
    for(j=y-6;j<=y+6;j++)
    {
      if((i==x-15)||(i==x-14)||(i==x+14)||(i==x+15)|| (j==y-6 ||j==y-5||j==y+5||j==y+6))
      {
        write_pixel(i,j,clr1);
      }
      else{
        write_pixel(i,j,clr2);
      }
      
    }
  }
}
void draw_ball(int x,int y,int colour)
{
    for(i=x-3;i<=x+3;i++)
    {
      for(j=y-3;j<=y+3;j++)
      {
        write_pixel(i,j,colour);
      }
    }
}

int move_ball()
{
  draw_ball(ball_x,ball_y,0);
  if(ball_x+3+ball_dx>319 || ball_x-3+ball_dx<0)
  {
    ball_dx*=-1;
    
  }

  if( ball_y-3+ball_dy<0)
  {
    ball_dy*=-1;
    
  }
  else if(ball_y+3+ball_dy>239)
  {
    draw_ball(ball_x,ball_y,0xffff);
    return 1;
  }
  else if(ball_y+3+ball_dy>=230 && ball_y-3+ball_dy<=230 && (ball_x+3>=board_x-30 &&ball_x-3<=board_x+30))
  {
    ball_dy*=-1;
    if(ball_dx*board_dir<0)
    {
      ball_dx*=-1;
    }
    
  }
  ball_x+=ball_dx;
  ball_y+=ball_dy;
  draw_ball(ball_x,ball_y,0xffff);
  return 0;

}
void check_collision()//dont change ball speed..ever. it needs to be one so that edge collision and top and bottom collision to work
{
  for(i=0;i<no_of_bricks;i++)
  {
    if(bricks[i][2]==1)
    {
      if(ball_x>bricks[i][0]-18 && ball_x<bricks[i][0]+18)
      {
        if(ball_y<=bricks[i][1]+9 && ball_dy<0 && ball_y>=bricks[i][1]+6)
        {
          bricks[i][2]=0;
          ball_dy*=-1;
          draw_brick(bricks[i][0],bricks[i][1],0,0);
          score++;
        }
        else if(ball_y>=bricks[i][1]-9 &&ball_dy>0 && ball_y<=bricks[i][1]-6)
        {
          bricks[i][2]=0;
          ball_dy*=-1;
          draw_brick(bricks[i][0],bricks[i][1],0,0);
          score++;
        }
      }
      else if(ball_y>bricks[i][1]-9 && ball_y<bricks[i][1]+9)
      {
        if(ball_x==bricks[i][0]-18 && ball_dx>0)
        {
          ball_dx*=-1;
          bricks[i][2]=0;
          draw_brick(bricks[i][0],bricks[i][1],0,0);
          score++;
        }
        else if(ball_x==bricks[i][0]+18 && ball_dx<0)
        {
          ball_dx*=-1;
          bricks[i][2]=0;
          draw_brick(bricks[i][0],bricks[i][1],0,0);
          score++;
        }
      }
      //collision with sides yet to be done
    }
    else if(bricks[i][2] == 2)
    {
    	if(ball_x>bricks[i][0]-18 && ball_x<bricks[i][0]+18)
      {
        if(ball_y==bricks[i][1]+9 && ball_dy<0)
        {
          bricks[i][2]=1;
          ball_dy*=-1;
          draw_crack(bricks[i][0],bricks[i][1],0,0);
        }
        else if(ball_y==bricks[i][1]-9 &&ball_dy>0)
        {
          bricks[i][2]=1;
          ball_dy*=-1;
          draw_crack(bricks[i][0],bricks[i][1],0,0);
        }
      }
      else if(ball_y>bricks[i][1]-9 && ball_y<bricks[i][1]+9)
      {
        if(ball_x==bricks[i][0]-18 && ball_dx>0)
        {
          ball_dx*=-1;
          bricks[i][2]=1;
          draw_crack(bricks[i][0],bricks[i][1],0,0);
        }
        else if(ball_x==bricks[i][0]+18 && ball_dx<0)
        {
          ball_dx*=-1;
          bricks[i][2]=1;
          draw_crack(bricks[i][0],bricks[i][1],0,0);
        }
      }
      //collision with sides yet to be done
    }
  }
}








