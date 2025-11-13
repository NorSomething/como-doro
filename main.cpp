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
    int xmax, ymax;

  public:
    void count_down(int sesh_len_min, WINDOW *heading_win);
    int get_current_time_in_mins();
    void draw_main_tui(int choice);
    int draw_menu_driven_tui();

    int choice;
};

void pomo::draw_main_tui(int choice) {
  
  initscr();
  noecho();
  cbreak();

  getmaxyx(stdscr, ymax, xmax);

  WINDOW *heading_win = newwin(ymax, xmax, 0, 0);
  refresh();
  
  box(heading_win, 0, 0);
  mvwprintw(heading_win, 1,xmax/2-13, "Nirmal's PoMo");
  wrefresh(heading_win);


  int sesh_len;

  if (choice == 2) {

  mvwprintw(heading_win, 2, 1, "Enter length of the pomodoro session to start (in minutes) : ");
  wrefresh(heading_win);

  echo(); //temp echo open so that user can enter minutes

  char input[10];
  wgetnstr(heading_win, input, 9); //reads upto 9 characters
  
  sesh_len = stoi(input); //converts to int
  
  noecho();  //temp echo close
  }
            //
  else if (choice == 1) {
    sesh_len = 25;
  } 

  else {
    mvwprintw(heading_win, 10, 1, "Invalid Choice.");
  }

  count_down(sesh_len, heading_win);

  getch();
  endwin();


}

int pomo::draw_menu_driven_tui() {
  
  initscr();
  noecho();
  cbreak();
  
  getmaxyx(stdscr, ymax, xmax);
  WINDOW *menu_win = newwin(ymax, xmax, 0, 0);
  refresh();

  box(menu_win, 0, 0);

  mvwprintw(menu_win, 1,1, "Welcome to Nirmal's PomoDoro!");
  mvwprintw(menu_win, 2,1, "Press 1 for default 25 minutes Pomo. \t\t\t Press 2 for custom Pomo timer.");
  wrefresh(menu_win);

  echo();
  
  char input[10];
  wgetnstr(menu_win, input, 9);

  int choice = stoi(input);

  noecho(); 

  endwin();

  return choice;


}

void pomo::count_down(int sesh_len_min, WINDOW *heading_win) {
  
  int current_min = get_current_time_in_mins();
  //int target_min = current_min + sesh_len_min; ->> this breaks after 59 like no circular flow back to 0th min
  int elapsed = 0;

  mvwprintw(heading_win, 3, 1, "Pomo Session for %d minutes has started.", sesh_len_min);
  wrefresh(heading_win);

  auto start = chrono::steady_clock::now(); //gets current time as a chrono object 
  auto duration = chrono::minutes(sesh_len_min); //converts user mins input into a chrono object 
  
  while (chrono::steady_clock::now() - start < duration) {
    auto mins_left = chrono::steady_clock::now() - start;
    int seconds = chrono::duration_cast<chrono::seconds>(mins_left).count(); //duration_cast (seconds) -> converts mis_left into seconds
                                                                             //count -> spits out integer value of it
    mvwprintw(heading_win, 4, 1, "Time left : ");
    mvwprintw(heading_win, 4, 14, "%d", sesh_len_min - (seconds/60));
    wrefresh(heading_win);
    this_thread::sleep_for(chrono::seconds(1));
  }

  mvwprintw(heading_win, 5, 1, "Pomo Session has ended.");
  wrefresh(heading_win);

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
  
  p.choice = p.draw_menu_driven_tui();
  p.draw_main_tui(p.choice);


  return 0;
}


