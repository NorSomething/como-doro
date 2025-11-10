#include <iostream>
#include <fstream>
#include <chrono> //for chrono::seconds
#include <ctime>
#include <string>
#include <thread> //for time.sleep() like thing
#include <ncurses.h>                  

using namespace std;

class pomo {
  private:
    int time_studied_secs;
    string sesh_name;

  public:
    void count_down(int sesh_len_min);
    int get_current_time_in_mins();
    void draw_tui();
};

void pomo::draw_tui() {
  
  initscr();
  noecho();
  cbreak();

  int xmax, ymax;

  getmaxyx(stdscr, ymax, xmax);

  WINDOW *heading_win = newwin(ymax, xmax, 0, 0);
  refresh();
  
  box(heading_win, 0, 0);
  mvwprintw(heading_win, 1,xmax/2, "Nirmal's PoMo");
  wrefresh(heading_win);

  mvwprintw(heading_win, 2, 1, "Enter length of the pomodoro session to start (in minutes) : ");
  wrefresh(heading_win);

  echo(); //temp echo open so that user can enter minutes

  char input[10];
  wgetnstr(heading_win, input, 9); //reads upto 9 characters
  
  int sesh_len = stoi(input); //converts to int
                              
  noecho(); //temp echo close
  
  count_down(sesh_len);


  getch();
  endwin();


}


void pomo::count_down(int sesh_len_min) {
  
  int current_min = get_current_time_in_mins();
  //int target_min = current_min + sesh_len_min; ->> this breaks after 59 like no circular flow back to 0th min
  int elapsed = 0;
  
  cout << "\n Pomo session for " << sesh_len_min << " minutes has started.\n";

  while(elapsed < sesh_len_min) {
    //cout << ("pomo sesh underway\n");

    this_thread::sleep_for(chrono::seconds(1));

    int current_sesh_min = get_current_time_in_mins();
    if (current_sesh_min != current_min) {
      current_min = current_sesh_min;
      elapsed++;
    }
  }

  //cout << "sesh over...\n";

}

int pomo::get_current_time_in_mins() {
  time_t tt; //holds current time?
  struct tm *st; //pointer to tm struct to hold local time 
  time(&tt); //gets current time 
             //till here -> tt gives number of seconds passed from unix epoch
  st = localtime(&tt); //convert to local time   
  //return asctime(st); //return local time as string                   
  return (st->tm_min);
                      
}

int main() {

  pomo p;

  //p.count_down(1);
  //
  
  p.draw_tui();


  return 0;
}


