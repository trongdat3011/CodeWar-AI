#include <bits/stdc++.h>
using namespace std;

//Define Position
struct cell {
    int row, col;
};

//Some playground constants
const int num_row = 20;
const int num_col = 30;
const int max_player = 4;
const int num_direction = 4;
const int INF = 1e9;

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
cell position[max_player]; //Position of Players
cell lastPosition[max_player];
int direction[max_player]; //Direction Player is facing
int a[num_row][num_col]; //Input matrix - current state of the game
int turn = 0; //current turn;
int dist[max_player][num_row][num_col];
int trace[max_player][num_row][num_col];

int distToSafety[max_player];
int distToKillMe[max_player];
int distToKillThem[max_player];
cell safeBlock[max_player];
bool running = false;

//Shared variables for functions
cell my_queue[num_col*num_row];
bool visitted[num_row][num_col];


//Debug mode - print out a lot of shits
bool debugging = true;

//Print direction number -> string
void speakOutLoud(int dir){
    string verbalDir;
    switch(dir){
        case 0: verbalDir = "LEFT";
                break;
        case 1: verbalDir = "UP";
                break;
        case 2: verbalDir = "DOWN";
                break;
        case 3: verbalDir = "RIGHT";
                break;
    }   
    cout << verbalDir << endl;
}
//refactor my_id to 0
void standardize(){
    for(int i=0; i<num_row; i++)
        for(int j=0; j<num_col; j++){
            if (a[i][j] == 1){
                a[i][j] = my_id*2+1;
                 continue;
            }
            if (a[i][j] == 2){
                a[i][j] = my_id*2+2;
                continue;
            }
            if (a[i][j] == my_id*2+1){
                a[i][j] = 1;
                continue;
            }
                
            if (a[i][j] == my_id*2+2){
                a[i][j] = 2;
                continue;
            }
        }
    cell temp;
    temp = position[0];
    position[0] = position[my_id];
    position[my_id] = temp;
}

void getDirection(){
    //Get current direction base on last turn position
    for(int i=0; i<num_player; i++){
        if (turn == 1){
            direction[i] = -1;
            continue;
        }
        for(int j=0; j<num_direction; j++)
            if (lastPosition[i].row + dr[j] == position[i].row && lastPosition[i].col + dc[j] == position[i].col)
                direction[i] = j;
    }
    //Update last turn position to this round position
    for(int i=0; i<num_player; i++)
        lastPosition[i] = position[i];
}

bool inside(cell x){
    return (x.row >= 0 && x.row < num_row && x.col >= 0 && x.col < num_col);
}

void bfs(int current_id){
    memset(visitted, false, sizeof(visitted));
    int bot = -1;
    int top = 0;
    my_queue[top] = position[current_id];
    dist[current_id][ my_queue[top].row ][ my_queue[top].col ] = 0;
    visitted[ my_queue[top].row ][ my_queue[top].col ] = true;
    while (true) {
        bot++;
        if (bot > top) break;
        cell temp = my_queue[bot];
        int cur_dist = dist[current_id][ temp.row ][ temp.col ];
        for(int i=0; i<num_direction; i++){
            if (bot == 0  && i == 3-direction[current_id]) continue;
            cell next;
            next.row = temp.row + dr[i];
            next.col = temp.col + dc[i];
            if (inside(next) && visitted[ next.row ][ next.col ] == false && a[next.row][next.col] != current_id*2+2){
                visitted[ next.row ][ next.col ] = true;
                my_queue[++top] = next;
                trace[current_id][ next.row ][ next.col ] = i;
                dist[current_id][ next.row ][ next.col ] = cur_dist+1;
            }
        }
    }
    if (debugging){
        cout << bot << " " << top << endl;
        cout << my_queue[top].row << " " << my_queue[top].col << endl;
        cout << "Dist for ID: " << current_id << endl;
        for(int i=0; i<num_row; i++){
            for(int j=0; j<num_col; j++)
                cout << setw(3) << dist[current_id][i][j];
            cout << endl;
        }
        cout << "Done dist for ID: " << current_id << endl;
    }
}

void calculateDistanceToSafety(int current_id){
    int res = INF;
    for(int i=0; i<num_row; i++)
        for(int j=0; j<num_col; j++)
            if (a[i][j] == current_id*2+1){
                if (dist[current_id][i][j] != -1){
                    if (dist[current_id][i][j] < res){
                        safeBlock[current_id].row = i;
                        safeBlock[current_id].col =j;
                    }
                    res = min(res, dist[current_id][i][j]);
                }
            }
    distToSafety[current_id] = res;
}

int trackSafety(int current_id, int x, int y){
    int dir = -1;
    while (trace[current_id][x][y] != -1){
        // cout << x << " " << y << " " << trace[current_id][x][y] << endl;
        dir = trace[current_id][x][y];
        x = x + dr[3 - dir];
        y = y + dc[3 - dir];
    }
    return dir;
}

void solve(){

    getDirection();
    memset(dist, -1, sizeof(dist));
    memset(trace, -1, sizeof(trace));
    for(int i=0; i<num_player; i++)
        bfs(i);
    for(int i=0; i<num_player; i++){
        calculateDistanceToSafety(i);
        if (debugging) cout << "Dist to safety " << i << " " << distToSafety[i] << endl;
    }

    if (running){
        int dir = track(0, safeBlock[0].row, safeBlock[0].col);
        if (dir == -1){
            running = false;
        } else {
            speakOutLoud(dir);
            return;
        }
    }

    //For enemy - Trying to find my unstable
    for(int current_id  = 1; current_id < num_player; current_id++){
        distToKillMe[current_id] = INF;
        for(int i=0; i<num_row; i++)
            for(int j=0; j<num_col; j++)
                if (a[i][j] == 2)
                    if (dist[current_id][i][j] != -1)
                        distToKillMe[current_id] = min(distToKillMe[current_id], dist[current_id][i][j]);
    }
    //For myself - Trying to find enemy's unstable
    for(int current_id  = 1; current_id < num_player; current_id++){
        distToKillMe[current_id] = INF;
        for(int i=0; i<num_row; i++)
            for(int j=0; j<num_col; j++)
                if (a[i][j] == current_id*2 + 2)
                    if (dist[0][i][j] != -1)
                        distToKillMe[current_id] = min(distToKillMe[current_id], dist[0][i][j]);
    }

}


int main(){
    cin >> num_player;
    cin >> my_id;
    my_id--;
    cin.ignore();

    string s;
    while (getline(cin, s)){
        turn++;

        for(int i=0; i<num_col; i++)
            a[0][i] = s[i] -'0';
        for(int i=1; i<num_row; i++){
            getline(cin, s);
            for(int j=0; j<num_col; j++)
                a[i][j] = s[j] -'0';
        }
        int x, y;
        for(int i=0; i<num_player; i++){
            cin >> x >> y;
            position[i].row = x;
            position[i].col = y;
        }
        cin.ignore();

        standardize();
        solve();

        if (debugging){
            for(int i=0; i<num_row; i++){
                for(int j=0; j<num_col; j++)
                    cout << a[i][j];
                cout << endl;
            }
        }
        
    }
    return 0;
}