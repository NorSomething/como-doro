#include <iostream>
#include <fstream>
#include <chrono> //for chrono::seconds
#include <ctime>
#include <string>
#include <thread> //for time.sleep() like thing

using namespace std;

class pomo {
  private:
    int time_studied_secs;
    string sesh_name;

  public:
    void count_down(int sesh_len_min);
    int get_current_time_in_mins();
};

void pomo::count_down(int sesh_len_min) {
  
  int current_min = get_current_time_in_mins();
  //int target_min = current_min + sesh_len_min; ->> this breaks after 59 like no circular flow back to 0th min
  int elapsed = 0;
  
  cout << "Pomo session for " << sesh_len_min << " minute has started.\n";

  while(elapsed < sesh_len_min) {
    cout << ("pomo sesh underway\n");

    this_thread::sleep_for(chrono::seconds(1));

    int current_sesh_min = get_current_time_in_mins();
    if (current_sesh_min != current_min) {
      current_min = current_sesh_min;
      elapsed++;
    }
  }

  cout << "sesh over...\n";

}

int pomo::get_current_time_in_mins() {
  time_t tt; //holds current time?
  struct tm *st; //pointer to tm struct to hold local time 
  time(&tt); //gets current time 
             //till here -> tt gives number of seconds passed from unix epoch
  st = localtime(&tt); //convert to local time   
  //return asctime(st); //return local time as string                   
  return st->tm_min;
                      
}

int main() {

  pomo p;

  p.count_down(1);
  
  return 0;
}


