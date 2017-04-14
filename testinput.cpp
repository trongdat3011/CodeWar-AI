#include <bits/stdc++.h>
using namespace std;

//Some playground constants
const int max_row = 20;
const int max_col = 30;
const int max_player = 4;

//Status of cells
int status[9]; //status[x] => status of cell with number x
const int EMPTY = 0;
const int MYSTABLE = 1;
const int MYUNSTABLE = 2;
const int ENEMYSTABLE = 3;
const int ENEMYUNSTABLE = 4;

//Direction - get opposite of direction x by 3-x 
int dr[4] = {0,-1, 1, 0};
int dc[4] = {-1, 0, 0, 1};
const int LEFT = 0;
const int RIGHT = 3;
const int UP = 1;
const int DOWN = 2;
const int NOTYET = -1;

//State of the game
int num_player, my_id; 
pair<int, int> position[max_player]; //Position of Players
int lastDirection[max_player]; //Direction Player is facing
string a[max_row]; //Input matrix - current state of the game


void printTest(){
    for(int i=0; i<9; i++)
        cout << status[i] << " ";
    cout << endl;
    for(int i=0; i<4; i++)
        cout << dr[i] << " " << dc[i] << endl;
    for(int i=0; i<max_player; i++)
        cout << lastDirection[i] << " ";
    cout << endl;
}

void init(){
    // Init status
    memset(status, EMPTY, sizeof(status));    
    for(int i=1; i<9; i+=2)
        status[i] = ENEMYSTABLE;
    status[my_id*2-1] = MYSTABLE;
    for(int i=2; i<9; i+=2)
        status[i] = ENEMYUNSTABLE;
    status[my_id*2] = MYUNSTABLE;
    //Init last direction
    memset(lastDirection, NOTYET, sizeof(lastDirection));
    printTest();
}
int solve(){

}

int main(){
    cin >> num_player;
    cin >> my_id;
    init();
    cin.ignore();
    string s;
    while (getline(cin, s)){
        a[0] = s;
        for(int i=1; i<max_row; i++)
            getline(cin, a[i]);
        int x, y;
        for(int i=0; i<num_player; i++){
            cin >> x >> y;
            position[i] = make_pair(x, y);
        }
        cin.ignore();
        // for(int i=0; i<max_row; i++)
            // cout << a[i] << endl;
        solve();
    }
    return 0;
}
