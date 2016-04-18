// {\tableofcontents\clearpage\section{Specification}
// This is a program in which the user controls the character on screen 
// with the W,A,S,D keys. The character must dodge the obects falling from 
// above. The number of collisions will be recorded and used to calculate 
// a score. If the character successfully finishes the level, a new 
// screen will pop up with a new background (level two). For the next
// level,the number of falling objects will increase therefore making it 
// difficult to  dodge the falling objects. There will also be more objects
// falling with greater frequency.
// \\Input-keyboard presses
// \\Output-position of character changes
// \\Calculation - score based on number of collisions and levels reached.
// }
// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include "labx.h"
// {\clearpage\section{Analysis}
// We can capture the keyboard and respond to those events by moving the 
// character animation left and right. We can animate the character by 
// scrolling through different frames of the animation as the character moves.
// \\We can rain objects from the sky by increasing their Y value on a timer.
// \\Then we can detect collisions between the character and objects
// by checking the overlap in the X and Y values.
// \\If there is a collision detected we can record that into the score variable.
// \\On level 2 we can define where the characted can move based on X and Y
// values of the character and the X and Y values of the level structure.
// i.e. the character is not allowed to move over the creek and can move up
// only when overlapping with a ladder object.
// \\When the user passes a level we can record the score to a file.
// \\When the user finishes the game we can open up the score files and 
// pass then to a calculator function that will add them and return total.
// \\Lastly, labels can be added by drawing text. 
// }
// {\clearpage\section{Design}
// \begin{itemize}
// \item{Overlapping:
// \\When the x coordinate of one image's coners enter the x boundaries 
// of another image and the  coordinates enter the  y-boundaries, a 
// collision occurs.
// \\INPUT-coordinates of images A and B.
// \\OUTPUT-boolean result signifying collision of lack thereof}
// \item{Load images: This function uses a "for" loop to load the images for
// animations into memory.
// \\No input or output.}
// \item{Object animate:
// \\This function goes through a loop and displays sequential images that were
// loaded by the "Load Images" function to create an animation.
// \\No input or output.}
// \item{Overlapping Check:
// \\Passes the objects to the Overlapping function and if the return is positive,
// displays a message and writes the event into a file for keeping track of the
// score.
// \\INPUT: none
// \\OUTPUT: none
// }
// \item{Draw:
// \\Draws labels and shapes in the window.
// \\INPUT: none
// \\OUTPUT: none}
// \item{Handle:
// \\This function captures the events and filters out ones of interest based on 
// pre-defined criteria. Therefore, if an event matches a criteria, actions are 
// executed.
// \\INPUT :integer
// \\OUTPUT :integer}
// \item{Main:
// \\Calls the functions to load images and create the window.
// \\No inputs or outputs}
// \item{Randomize:
// \\This function receives a range of values within which it needs to generate
// a random value and returns said random value to the function that called this
// random function. The random values are generated using the time-based seed to
// prevent repetitive patterns within generation.
// \\INPUT: value range
// \\OUTPUT: random number within the given range.}
// \item{Score counter:
// \\Score counter keeps track of collisions between the character and falling objects.
// This score modifier is later subtracted from the base score and if total score
// reaches less than zero, death occurs.
// \\INPUT: user reaction to the game interface
// \\OUTPUT: Score modifier}
// \item{Ladders:
// \\The ladders function keeps track of collisions between ladder objects and character.
// When overlapping, the charater is allowed to move up on the screen onto raised platforms.
// When not overlapping, the character is allowed movement only in the horizontal 
// dimension.
// \\INPUT: Pointers to object B and A.
// \\OUTPUT: Boolean}
// \item{Score Calculator:
// \\The score calculator takes in the value of the score timer that increments the
// score based on time spent in the level and subtracts the value from the score modifier
// returning the current calculated score.
// \\INPUT: current score timer, score modifier.
// \\OUTPUT: final score.}
// \item{Score Timer:
// \\Score timer keeps track of the time spent in the level on which the
// base score is counted.
// \\INPUT: none.
// \\OUTPUT: base score}
// \item{Level up:
// \\Triggers the level to be redrawn to the next level when user finishes the current level
// by getting to the level-up object.
// \\INPUT: boolean event from level up object collision function.
// \\OUTPUT: none}
// \item{Score recorder:
// \\Records score at the end of the level to a file.
// \\INPUT: score.
// \\OUTPUT: none}
// \item{Final score:
// \\When current level score drops below zero due to too many objects hitting the 
// character, death occurs. This will trigger the final score calculation which
// will read the score file and add up all the scores from previous levels.
// \\INPUT: death boolean.
// \\OUTPUT: final score}
// \item{Death:
// \\Checks current score and when it reaches 0 or a negative number, ends game
// and displays the final score.
// \\INPUT: score.
// \\OUTPUT: none}
// \end{itemize}
// }
// {\clearpage\section{Implementation}
// }
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/unistd.h>
#include <stdio.h>
#include <time.h>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_PNG_Image.H>
using namespace std;
static const int N1 = 1; //frames in level1
static const int N2 = 4; //frames in char
static const int N3 = 18; //frames in leaf
static const int SHURIKEN = 3; //shuriken
static Fl_GIF_Image* level1_images[N1]; 
static Fl_GIF_Image* level2_images[N1]; 
static Fl_GIF_Image* char_images_left[N2]; 
static Fl_GIF_Image* char_images_right[N2]; 
static Fl_GIF_Image* leaf_images[N3]; 
static Fl_PNG_Image* level_up_images[N1]; 
static Fl_PNG_Image* shuriken_images[SHURIKEN]; 
static int MyScoreL1 = 0; 
static int MyScoreL2 = 0; 
static ofstream out_file; 
static ifstream in_file; 

// load and process gif animations for display
void load_images() {
	
// background for level 1
  for(int i=0; i < N1; ++i)
{
 std::ostringstream oss; oss << i;
 std::string s = "levels/level1"+oss.str()+".gif";
 level1_images[i] = new Fl_GIF_Image(s.c_str());
}

// main character running left animation
for(int i=0; i < N2; ++i)
{
 std::ostringstream oss; oss << i;
 std::string s = "char/char_left_"+oss.str()+".gif";
 char_images_left[i] = new Fl_GIF_Image(s.c_str());
}

// main character running right animation
for(int i=0; i < N2; ++i)
{
 std::ostringstream oss; oss << i;
 std::string s = "char/char_right_"+oss.str()+".gif";
 char_images_right[i] = new Fl_GIF_Image(s.c_str());
}

// ?? leftover from previous developlment ??
for(int i=0; i < N3; ++i)
{
 std::ostringstream oss; 
 oss << setfill('0') << setw(2) << i;
 std::string s = "leaf/leaf"+oss.str()+".gif";
 leaf_images[i] = new Fl_GIF_Image(s.c_str());
}

// shuriken rotating animation
for(int i = 0; i < SHURIKEN; i++) //loads the images
{
 ostringstream oss;
 oss << setfill('0') << setw(3) << i;
 string s = "Shuriken/frame_";
 s += oss.str();
 s += ".png";
 shuriken_images[i] = new Fl_PNG_Image(s.c_str());
}

// level up image
for(int i=0; i < N1; ++i)
{
 std::ostringstream oss; oss << i;
 std::string s = "objects/level_up_"+oss.str()+".png";
 level_up_images[i] = new Fl_PNG_Image(s.c_str());
}
  
// background for level 2
for(int i=0; i < N1; ++i)
{
 std::ostringstream oss; oss << i;
 std::string s = "levels/level2"+oss.str()+".gif";
 level2_images[i] = new Fl_GIF_Image(s.c_str());
}
}

// takes the scores from the first and second 
// levels and sums them.
int score_sum(int score1, int score2) {
  //add up the scores
int sum = 0;
sum = score1 + score2;
return sum;
}

void level1_animate(void*) {
  //animation for level 1 in case we want to add animation within the level
static int i = 0;
level1_back->image(level1_images[i]);
i = (i + 1) % N1;
level1_back->parent()->redraw();
overlapping_check();
double t = 1.0/8;
Fl::repeat_timeout(t,level1_animate);
}

void level2_animate(void*) {
  //animation for level 1 in case we want to add animation within the level
static int i = 0;
level2_back->image(level2_images[i]);
i = (i + 1) % N1;
level2_back->parent()->redraw();
overlapping_check();
double t = 1.0/8;
Fl::repeat_timeout(t,level2_animate);
}

void char_animate_left(void*) {
  //animation routine for character moving left
static int i = 0;
character->image(char_images_left[i]);
i = (i + 1) % N2;
//using magic numbers since the window is resizable, should change to percentages
int x = character->x();
int y = character->y();
int right = win->x()+win->w();
 
win->redraw(); //do we need this?
double t = 1.0/12;
Fl::repeat_timeout(t,char_animate_left);
}

void char_animate_right(void*) {
  //animation routine for character moving right
static int i = 0;
character->image(char_images_right[i]);
i = (i + 1) % N2;
//using magic numbers since the window is resizable, should change to percentages
int x = character->x();
int y = character->y();
int right = win->x()+win->w();
 
win->redraw(); //do we need this?
double t = 1.0/12;
Fl::repeat_timeout(t,char_animate_right);
}

void leaf_animate(void*) {
  //leaf animation if we want to add multiple different falling objects
/*
static int i = 0;
leaf->image(leaf_images[i]);
i = (i + 1) % N3;
//using magic numbers since the window is resizable, should change to percentages
int x = leaf->x();
int y = leaf->y();
int top = win->y()+win->h();

if(y <= win->h())
{
  leaf->position(x, y+3);
}
else
{
  leaf->position(x, -25);
};



 
win->redraw(); //do we need this?
//maybe redraw only for the fastest animation(may affect smoothness)
double t = 1.0/18;
Fl::repeat_timeout(t,leaf_animate);
*/
}

void level_up_animate(void*) {
  //animation for the level_up object in case we want it animated
static int i = 0;
level_up->image(level_up_images[i]);
i = (i + 1) % N1;
level_up->parent()->redraw();
overlapping_check();
double t = 1.0/8;
Fl::repeat_timeout(t,level_up_animate);
  //animation routine for level_up object on the second level
level_up2->image(level_up_images[i]);
i = (i + 1) % N1;
overlapping_check();
}

void shuriken_animate(void*) {
  //animation routine for falling shurikens
static int i = 0;
shuriken10->image(shuriken_images[i]);
i = (i + 1) % SHURIKEN;

/*
int shuriken_x = random(0, win -> w() - shuriken -> w());
int shuriken_y = random(0, win -> h() - shuriken -> h());
shuriken -> position(shuriken_x, shuriken_y);
*/

int x10 = shuriken10 -> x();
int y10 = shuriken10 -> y();

//modifying score in case of impact
int FirstLevel;
in_file.open("scores.txt");
in_file >> FirstLevel >> MyScoreL1;
score_count->value(MyScoreL1);

if (y10 < 240)
  {
   shuriken10 -> position(x10, y10 + 5);
  } else if (y10 <= 300)
  {
   int shuriken_x10 = random(0, win -> w() - shuriken10 -> w());
   int shuriken_y10 = rand() % (-200) + (-200);
   shuriken10 -> position(shuriken_x10, shuriken_y10);
   MyScoreL1 += 2;
  }
  
shuriken10->parent()->redraw();
double t = 1.0/24;
Fl::repeat_timeout(t,shuriken_animate);
  shuriken11->image(shuriken_images[i]);

int x11 = shuriken11 -> x();
int y11 = shuriken11 -> y();

if (y11 < 240)
  {
   shuriken11 -> position(x11, y11 + 5);
  } else if (y11 <= 300)
  {
   int shuriken_x11 = random(0, win -> w() - shuriken11 -> w());
   int shuriken_y11 = rand() % (-200) + (-200);
   shuriken11 -> position(shuriken_x11, shuriken_y11);
   MyScoreL1 += 2;
  }
  shuriken12->image(shuriken_images[i]);

int x12 = shuriken12 -> x();
int y12 = shuriken12 -> y();

if (y12 < 240)
  {
   shuriken12 -> position(x12, y12 + 5);
  } else if (y12 <= 300)
  {
   int shuriken_x12 = random(0, win -> w() - shuriken12 -> w());
   int shuriken_y12 = rand() % (-200) + (-200);
   shuriken12 -> position(shuriken_x12, shuriken_y12);
   MyScoreL1 += 2;
  }
  shuriken20->image(shuriken_images[i]);

int x20 = shuriken20 -> x();
int y20 = shuriken20 -> y();
//score
int SecondLevel;
in_file.open("scores.txt");
in_file >> SecondLevel >> MyScoreL2;
score_count2->value(MyScoreL2);

if (y20 < 240)
  {
   shuriken20 -> position(x20, y20 + 5);
  } else if (y20 <= 300)
  {
   int shuriken_x20 = random(0, win -> w() - shuriken20 -> w());
   int shuriken_y20 = rand() % (-200) + (-200);
   shuriken20 -> position(shuriken_x20, shuriken_y20);
   MyScoreL2 += 2;
  }
  
shuriken20->parent()->redraw();
  shuriken21->image(shuriken_images[i]);

int x21 = shuriken21 -> x();
int y21 = shuriken21 -> y();

if (y12 < 240)
  {
   shuriken21 -> position(x21, y21 + 5);
  } else if (y21 <= 300)
  {
   int shuriken_x21 = random(0, win -> w() - shuriken21 -> w());
   int shuriken_y21 = rand() % (-200) + (-200);
   shuriken21 -> position(shuriken_x21, shuriken_y21);
   MyScoreL2 += 2;
  }
  shuriken22->image(shuriken_images[i]);

int x22 = shuriken22 -> x();
int y22 = shuriken22 -> y();

if (y22 < 240)
  {
   shuriken22 -> position(x22, y22 + 5);
  } else if (y22 <= 300)
  {
   int shuriken_x22 = random(0, win -> w() - shuriken22 -> w());
   int shuriken_y22 = rand() % (-200) + (-200);
   shuriken22 -> position(shuriken_x22, shuriken_y22);
   MyScoreL2 += 2;
  }
  shuriken23->image(shuriken_images[i]);

int x23 = shuriken23 -> x();
int y23 = shuriken23 -> y();

if (y23 < 240)
  {
   shuriken23 -> position(x23, y23 + 5);
  } else if (y23 <= 300)
  {
   int shuriken_x23 = random(0, win -> w() - shuriken23 -> w());
   int shuriken_y23 = rand() % (-200) + (-200);
   shuriken23 -> position(shuriken_x23, shuriken_y23);
   MyScoreL2 += 2;
  }
  shuriken24->image(shuriken_images[i]);

int x24 = shuriken24 -> x();
int y24 = shuriken24 -> y();

if (y24 < 240)
  {
   shuriken24 -> position(x24, y24 + 5);
  } else if (y24 <= 300)
  {
   int shuriken_x24 = random(0, win -> w() - shuriken24 -> w());
   int shuriken_y24 = rand() % (-200) + (-200);
   shuriken24 -> position(shuriken_x24, shuriken_y24);
   MyScoreL2 += 2;
  }
}

bool overlapping(Fl_Box *a1, Fl_Box *a2) {
  return (a1->x()+a1->w() > a2->x() && a2->x() && a1->x() < a2->x()+a2->w() &&
        a1->y()+a1->h() > a2->y() && a2->y() && a1->y() < a2->y()+a2->h());
}

void overlapping_check() {
  //check overlapping for everything
out_ouch -> hide();
out_ouch2 -> hide();

//Score allocation
int MyScoreL1Temp = 0;
int MyScoreL2Temp = 0;
score_count->value(MyScoreL1);
score_count2->value(MyScoreL2);
//in_file.open("scores.txt");



//Here be collision check.
//if(overlapping(character,leaf)) 
//{out_ouch -> show();}
int score_modifier = 2; //how many points get taken off when you get hit
if(overlapping(character,shuriken10))
 {out_ouch -> show();
 MyScoreL1 = MyScoreL1 - score_modifier;}//if you get hit with a shuriken, points deducted
if(overlapping(character,shuriken11))
 {out_ouch -> show();
 MyScoreL1 = MyScoreL1 - score_modifier;}
if(overlapping(character,shuriken12))
 {out_ouch -> show();
 MyScoreL1 = MyScoreL1 - score_modifier;}
  
if(overlapping(character2,shuriken20))
 {out_ouch2 -> show();
 MyScoreL2 = MyScoreL2 - score_modifier;}
if(overlapping(character2,shuriken21))
 {out_ouch2 -> show();
 MyScoreL2 = MyScoreL2 - score_modifier;}
if(overlapping(character2,shuriken22))
 {out_ouch2 -> show();
 MyScoreL2 = MyScoreL2 - score_modifier;}
if(overlapping(character2,shuriken23))
 {out_ouch2 -> show();
 MyScoreL2 = MyScoreL2 - score_modifier;}
if(overlapping(character2,shuriken24))
 {out_ouch2 -> show();
 MyScoreL2 = MyScoreL2 - score_modifier;}

//Level1 to Level2
if(overlapping(character,level_up)) 
 {
 character->position(25, 169);
 level2_win->show();
 win->hide();
 //writing score
 ofstream out_file;
 out_file.open("score1.txt");
 out_file << MyScoreL1 << " " << endl;
 //out_file.close();
 MyScoreL2 = 0;
 }

//Level2 to Score Screen
if(overlapping(character2,level_up2))
 {
 character2->position(25,169);
 score_screen->show();
 level2_win->hide();
 
 //writing score
 ofstream out_file;
 out_file.open("score2.txt");
 out_file << MyScoreL2 << " " << endl;
 out_file.close();
 //reading scores
 ifstream in_file;
 in_file.open("score1.txt");
 in_file >> MyScoreL1Temp;
 in_file.close();
 
 in_file.open("score2.txt");
 in_file >> MyScoreL2Temp;
 in_file.close();
 //outputting scores
 final_score2->value(MyScoreL2Temp);
 final_score1->value(MyScoreL1Temp);
 final_total->value(score_sum(MyScoreL1Temp,MyScoreL2Temp));
 }
 
win->redraw();
level2_win->redraw();
score_screen->redraw();
}

int random(int a, int b) {
  //generate random positions
return rand() % (b - a + 1) + a;
}

Canvas::Canvas(int x,int y,int w,int h) : Fl_Double_Window(x,y,w,h,"Canvas") {
}

void Canvas::draw() {
  Fl_Double_Window :: draw();
  fl_font(FL_HELVETICA,20);
fl_color(0,0,0);
fl_draw("Hail Satan!", win->w()/2 , win->h()/4);
fl_color(250,250,250); //set color to white
fl_draw("a: move left, d: move right" , 10, 50);
}

int Canvas::handle(int event) {
  //capturing events in level1 and handling them
int r = 0;
static int i = 0;

//using magic numbers since the window is resizable, should change to percentages
int x = character->x();
int y = character->y();
int right = win->x()+win->w();
switch(event)
{ 
  case FL_PUSH:
  cout << "got it"<< endl; r=1; break;
  case FL_KEYBOARD: FL_FOCUS: FL_UNFOCUS:
    if(Fl::event_key() == 'a')
    {
    r=1;
    
    character->image(char_images_left[i]);
    i = (i + 1) % N2;
        
    character->position(x-5, y); //move  the character left 5px
    }
    if(Fl::event_key() == 'd')
    {
    r=1; 
    
    character->image(char_images_right[i]);
    i = (i + 1) % N2;
    
    character->position(x+5, y); //move  the character right 5px
    }
    break;    
  
}
return r;
}

Level2::Level2(int x,int y,int w,int h) : Fl_Double_Window(x,y,w,h,"Canvas") {
}

int Level2::handle(int event) {
  //capturing events in level 2 and handling them and level structure
int r = 0;
static int i = 0;

//using magic numbers since the window is resizable, should change to percentages
int x = character2->x();
int y = character2->y();
int right = win->x()+win->w();
switch(event)
{ 
  case FL_PUSH:
  cout << "got it"<< endl; r=1; break;
  case FL_KEYBOARD: FL_FOCUS: FL_UNFOCUS:
    if(Fl::event_key() == 'a')
    {
    r=1;
    
    character2->image(char_images_left[i]);
    i = (i + 1) % N2;
        
    character2->position(x-5, y); //move  the character left 5px
    }
    if(Fl::event_key() == 'd')
    {
    r=1; 
    
    character2->image(char_images_right[i]);
    i = (i + 1) % N2;
    
    character2->position(x+5, y); //move  the character right 5px
    }
    if(overlapping(character2,ladder1) && Fl::event_key() == 'w')  
    {
      r=1; 
      character2->image(char_images_right[i]);
      i = (i + 1) % N2;
    
      character2->position(x, y-5); //move  the character up 5px
    }
    if(overlapping(character2,ladder1) && Fl::event_key() == 's')  
    {
      r=1; 
      if((character2 -> y()) < (ladder1 -> y() + ladder1 -> h() - character2 -> h()))
      {character2->image(char_images_right[i]);
      i = (i + 1) % N2;
    
      character2->position(x, y+5); //move  the character down 5px
      }
    }
    
    if(overlapping(character2,ladder2) && Fl::event_key() == 'w')  
    {
      r=1; 
      character2->image(char_images_right[i]);
      i = (i + 1) % N2;
    
      character2->position(x, y-5); //move  the character right 5px
    }
    if(overlapping(character2,ladder2) && Fl::event_key() == 's')  
    {
      r=1; 
      if((character2 -> y()) < (ladder2 -> y() + ladder2 -> h() - character2 -> h()))
      {character2->image(char_images_right[i]);
      i = (i + 1) % N2;
    
      character2->position(x, y+5); //move  the character right 5px
      }
      
    }
    break;
    
}

//GRAVITY RESEARCH for falling off of platforms
  if(character2->y() < 169 && (character2->x() < 110 || character2->x() > 205 ))
{character2->position(character2->x(),character2->y()+5);}

  if(character2->x() > 116 && character2->x() < 180 && character2->y() > 144)
{character2->position(116,character2->y());}

  if(character2->x() < 185 && character2->x() >= 180 && character2->y() > 144)
{character2->position(185,character2->y());}

return r;
}

Fl_Double_Window *score_screen=(Fl_Double_Window *)0;

Fl_Box *final1=(Fl_Box *)0;

static void cb_final1(Fl_Box*, void*) {
  // take the scores.txt and output into flbox;
}

Fl_Box *final2=(Fl_Box *)0;

Fl_Box *finalTotal=(Fl_Box *)0;

Fl_Value_Output *final_score2=(Fl_Value_Output *)0;

Fl_Value_Output *final_score1=(Fl_Value_Output *)0;

Fl_Value_Output *final_total=(Fl_Value_Output *)0;

Level2 *level2_win=(Level2 *)0;

Fl_Box *level2_back=(Fl_Box *)0;

Fl_Box *character2=(Fl_Box *)0;

Fl_Box *leaf2=(Fl_Box *)0;

Fl_Box *level_up2=(Fl_Box *)0;

Fl_Box *ladder1=(Fl_Box *)0;

Fl_Box *ladder2=(Fl_Box *)0;

Fl_Box *shuriken20=(Fl_Box *)0;

Fl_Box *shuriken21=(Fl_Box *)0;

Fl_Box *shuriken22=(Fl_Box *)0;

Fl_Box *shuriken23=(Fl_Box *)0;

Fl_Box *shuriken24=(Fl_Box *)0;

Fl_Output *out_ouch2=(Fl_Output *)0;

Fl_Value_Output *score_count2=(Fl_Value_Output *)0;

Canvas *win=(Canvas *)0;

Fl_Box *level1_back=(Fl_Box *)0;

Fl_Box *character=(Fl_Box *)0;

Fl_Box *leaf=(Fl_Box *)0;

Fl_Box *level_up=(Fl_Box *)0;

Fl_Box *shuriken10=(Fl_Box *)0;

Fl_Box *shuriken11=(Fl_Box *)0;

Fl_Box *shuriken12=(Fl_Box *)0;

Fl_Output *out_ouch=(Fl_Output *)0;

Fl_Value_Output *score_count=(Fl_Value_Output *)0;

int main(int argc, char **argv) {
  srand (time(NULL));
load_images();
Fl::add_timeout(0,level1_animate);
//Fl::add_timeout(1,char_animate_right);
//Fl::add_timeout(2,leaf_animate);
Fl::add_timeout(0,level_up_animate);
Fl::add_timeout(0,level2_animate);
Fl::add_timeout(1,shuriken_animate);
  { score_screen = new Fl_Double_Window(410, 310);
    { final1 = new Fl_Box(103, 25, 57, 36, "Level 1:");
      final1->callback((Fl_Callback*)cb_final1);
      out_file << "scores.txt" << " " << MyScoreL1 << endl;
    } // Fl_Box* final1
    { final2 = new Fl_Box(280, 25, 45, 36, "Level 2:");
      out_file << "scores.txt" << " " << MyScoreL2 << endl;
    } // Fl_Box* final2
    { finalTotal = new Fl_Box(200, 109, 45, 36, "Total:");
      out_file << "scores.txt" << " " << MyScoreL2 << endl;
    } // Fl_Box* finalTotal
    { final_score2 = new Fl_Value_Output(288, 60, 30, 15);
      final_score2->labelcolor((Fl_Color)55);
    } // Fl_Value_Output* final_score2
    { final_score1 = new Fl_Value_Output(120, 60, 30, 15);
      final_score1->labelcolor((Fl_Color)48);
    } // Fl_Value_Output* final_score1
    { final_total = new Fl_Value_Output(210, 145, 30, 15);
      final_total->labelcolor((Fl_Color)48);
    } // Fl_Value_Output* final_total
    score_screen->end();
  } // Fl_Double_Window* score_screen
  { level2_win = new Level2(0, 0, 320, 240);
    level2_win->box(FL_FLAT_BOX);
    level2_win->color((Fl_Color)FL_BACKGROUND_COLOR);
    level2_win->selection_color((Fl_Color)FL_BACKGROUND_COLOR);
    level2_win->labeltype(FL_NO_LABEL);
    level2_win->labelfont(0);
    level2_win->labelsize(14);
    level2_win->labelcolor((Fl_Color)FL_FOREGROUND_COLOR);
    level2_win->align(FL_ALIGN_TOP);
    level2_win->when(FL_WHEN_RELEASE);
    { level2_back = new Fl_Box(0, 0, 320, 240);
    } // Fl_Box* level2_back
    { Fl_Box* o = character2 = new Fl_Box(25, 169, 17, 23);
      o->image(new Fl_GIF_Image("char/char_right_1.gif"));
    } // Fl_Box* character2
    { leaf2 = new Fl_Box(250, 0, 22, 21);
    } // Fl_Box* leaf2
    { level_up2 = new Fl_Box(299, 180, 13, 12);
    } // Fl_Box* level_up2
    { ladder1 = new Fl_Box(111, 142, 12, 50);
    } // Fl_Box* ladder1
    { ladder2 = new Fl_Box(195, 142, 12, 50);
    } // Fl_Box* ladder2
    { shuriken20 = new Fl_Box(15, -160, 31, 21);
    } // Fl_Box* shuriken20
    { shuriken21 = new Fl_Box(85, -60, 31, 21);
    } // Fl_Box* shuriken21
    { shuriken22 = new Fl_Box(150, -140, 31, 21);
    } // Fl_Box* shuriken22
    { shuriken23 = new Fl_Box(210, -195, 31, 21);
    } // Fl_Box* shuriken23
    { shuriken24 = new Fl_Box(260, -30, 31, 21);
    } // Fl_Box* shuriken24
    { Fl_Output* o = out_ouch2 = new Fl_Output(225, 111, 105, 37);
      out_ouch2->hide();
      o->value("FFFUUUUU!!!");
    } // Fl_Output* out_ouch2
    { score_count2 = new Fl_Value_Output(285, 220, 30, 15, "score:");
      score_count2->labelcolor((Fl_Color)55);
      score_count2->value(MyScoreL2);
    } // Fl_Value_Output* score_count2
    level2_win->end();
  } // Level2* level2_win
  { win = new Canvas(0, 0, 320, 240);
    win->box(FL_FLAT_BOX);
    win->color((Fl_Color)FL_BACKGROUND_COLOR);
    win->selection_color((Fl_Color)FL_BACKGROUND_COLOR);
    win->labeltype(FL_NO_LABEL);
    win->labelfont(0);
    win->labelsize(14);
    win->labelcolor((Fl_Color)FL_FOREGROUND_COLOR);
    win->align(FL_ALIGN_TOP);
    win->when(FL_WHEN_RELEASE);
    { level1_back = new Fl_Box(0, 0, 320, 240);
    } // Fl_Box* level1_back
    { Fl_Box* o = character = new Fl_Box(25, 169, 17, 23);
      o->image(new Fl_GIF_Image("char/char_right_1.gif"));
    } // Fl_Box* character
    { leaf = new Fl_Box(250, 0, 22, 21);
    } // Fl_Box* leaf
    { level_up = new Fl_Box(299, 180, 13, 12);
    } // Fl_Box* level_up
    { shuriken10 = new Fl_Box(15, -50, 31, 21);
    } // Fl_Box* shuriken10
    { shuriken11 = new Fl_Box(150, -80, 31, 21);
    } // Fl_Box* shuriken11
    { shuriken12 = new Fl_Box(260, -20, 31, 21);
    } // Fl_Box* shuriken12
    { Fl_Output* o = out_ouch = new Fl_Output(225, 111, 105, 37);
      out_ouch->hide();
      o->value("FFFUUUUU!!!");
    } // Fl_Output* out_ouch
    { score_count = new Fl_Value_Output(285, 220, 30, 15, "score:");
      score_count->labelcolor((Fl_Color)48);
      score_count->value(MyScoreL1);
    } // Fl_Value_Output* score_count
    win->end();
  } // Canvas* win
  win->show(argc, argv);
  return Fl::run();
}
// {\clearpage\section{Test}
// Freshly started:\\
// \includegraphics {labx.png}\\
// Hit by a Star:\\
// \includegraphics {labx2.png}\\
// \clearpage Level 2:\\
// \includegraphics {labx3.png}\\
// On Top of the Bridge:\\
// \includegraphics {labx4.png}\\
// \clearpage Final Score Display:\\
// \includegraphics {labx6.png}\\
// }
