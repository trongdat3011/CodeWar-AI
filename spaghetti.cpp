#include <bits/stdc++.h>
using namespace std;

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

//State of the game
int num_player, my_id, turn  = 0; 
pair< int, int > position[max_player], tuonglaiPosition[max_player]; //Position of Players
pair< int, int > lastPosition[max_player];
int direction[max_player], tuonglaiDirection[max_player]; 
int a[num_row][num_col], tuonglai[num_row][num_col], predict[num_row][num_col];
pair< int, int > my_queue[num_col*num_row];
bool visitted[num_row][num_col];
int dist[max_player][num_row][num_col];
int trace[max_player][num_row][num_col]; //0 enable moving on self-stable
int distToSafety;
int distToKillMe;

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
bool inside(pair< int, int > x){
    return (x.first >= 0 && x.first < num_row && x.second >= 0 && x.second < num_col);
}
bool inside(int x, int y){
    return (x >= 0 && x < num_row && y >= 0 && y < num_col);
}
bool isBorder(int x, int y, int (&state)[num_row][num_col] ){
    for(int i=0; i<num_direction; i++){
        int u = x + dr[i];
        int v = y + dc[i];
        if (inside(u, v) && state[u][v] != 1)
            return true;
    }
    return false;
}
int evaluate(int state[num_row][num_col]){
    bool visited[num_row][num_col];
    queue< pair< int, int> > q;
    int stable = 1, unstable = 2;
    int ans = 0, k = 0;
    memset(visited , false , sizeof visited);

    for (int i = 0; i < num_row; i++)
        for (int j = 0; j < num_col; j++)
            if (i == 0 || j == 0 || i == num_row - 1 || j == num_col - 1) {
                k = state[i][j];
                if (k != stable && k != unstable && !visited[i][j]) {
                    visited[i][j] = true;
                    q.push(make_pair(i, j));
                }
            }
    int stableCount = 0;
    for (int i = 0; i < num_row; i++)
        for (int j = 0; j < num_col; j++)
            if (state[i][j] == stable)
                stableCount++;
    while (!q.empty()) {
        ans++;
        pair<int, int > p = q.front();q.pop();
        for (int i = 0; i < 4; i++) {
            pair<int , int > tmp = make_pair(p.first + dr[i] , p.second + dc[i]);
            if(inside(tmp) && !visited[tmp.first][tmp.second]) {
                k = state[tmp.first][tmp.second];
                if (k != stable && k != unstable) {
                    visited[tmp.first][tmp.second] = true;
                    q.push(tmp);
                }
            }


        }
    }

    ans = 600 - ans - stableCount;
    return ans;
}
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
    pair< int, int > temp;
    temp = position[0];
    position[0] = position[my_id];
    position[my_id] = temp;
}

void getDirection(){
    for(int i=0; i<num_player; i++){
        if (turn == 1){
            direction[i] = -1;
            continue;
        }
        for(int j=0; j<num_direction; j++)
            if (lastPosition[i].first + dr[j] == position[i].first && lastPosition[i].second + dc[j] == position[i].second)
                direction[i] = j;
    }
    for(int i=0; i<num_player; i++)
        lastPosition[i] = position[i];
}

void createState(int (&clone)[num_row][num_col], int (&origin)[num_row][num_col] ){
    for(int i=0; i<num_row; i++){
        for(int j=0; j<num_col; j++)
            clone[i][j] = origin[i][j];
    }
}
void update(int (&state)[num_row][num_col], int id){
    for(int i=0; i<num_row; i++){
        for(int j=0; j<num_col; j++)
            if (state[i][j] == id*2 + 1)
                state[i][j] == id*2 + 2;
    }
}

void bfs(int current_id, int (&state)[num_row][num_col], int (&faceTo)[max_player] ){
    
    memset(visitted, false, sizeof(visitted));
    int bot = -1;
    int top = 0;
    my_queue[top] = position[current_id];
    dist[current_id][ my_queue[top].first ][ my_queue[top].second ] = 0;
    visitted[ my_queue[top].first ][ my_queue[top].second ] = true;

    while (true) {
        bot++;
        if (bot > top) break;
        int x = my_queue[bot].first;
        int y = my_queue[bot].second;

        int cur_dist = dist[current_id][ x ][ y ];
        for(int i=0; i<num_direction; i++){
            if (bot == 0  && i == 3-faceTo[current_id]) continue;
            int u = x + dr[i];
            int v = y + dc[i];

            if (inside(u, v) && !visitted[u][v]  && state[u][v] != current_id*2+2){
                visitted[u][v] = true;
                my_queue[++top] = make_pair(u, v);
                dist[current_id][u][v] = cur_dist+1;
                trace[current_id][u][v] = i;
            }
        }
    }

}
void extractInformation(int (&state)[num_row][num_col]){
    distToSafety = INF;
    for(int i=0; i<num_row; i++)
        for(int j=0; j<num_col; j++)
            if (state[i][j] == 1){
                if (dist[0][i][j] != -1){
                    distToSafety = min(distToSafety, dist[0][i][j]);
                }
            }
    distToKillMe = INF;
    for(int i=0; i<num_row; i++)
        for(int j=0; j<num_col; j++)
            if (state[i][j] == 2)
                if (dist[1][i][j] != -1)
                    distToKillMe = min(distToKillMe, dist[1][i][j]);
    
}
int track(int current_id, int x, int y){
    if (!inside(x, y)) return -1;
    int dir = -1;
    while (trace[current_id][x][y] != -1){
        dir = trace[current_id][x][y];
        x = x + dr[3 - dir];
        y = y + dc[3 - dir];
    }
    return dir;
}

void markPath(int current_id, int (&state)[num_row][num_col]){
    int minDist = INF;
    pair < int, int > minBlock;
    for(int i=0; i<num_row; i++)
        for(int j=0; j<num_col; j++){
            if (state[i][j] == 1)
                if (dist[current_id][i][j] != -1 && dist[current_id][i][j] < minDist){
                    minDist = dist[current_id][i][j];
                    minBlock = make_pair(i,j);
                }   
        }
    int x = minBlock.first;
    int y = minBlock.second;
    while (trace[current_id][x][y] != -1){
        int dir = trace[current_id][x][y];
        x = x + dr[3 - dir];
        y = y + dc[3 - dir];
        state[x][y] = 2;
    }
}


void solve(){
    if ( a[position[0].first][position[0].second] == 1 && !isBorder(position[0].first, position[0].second, a)){
        memset(trace, -1, sizeof(trace));
        bfs(0, a, direction);
        int minDist = INF;
        pair < int, int > minBlock;
        for(int i=0; i<num_row; i++)
            for(int j=0; j<num_col; j++){
                if (isBorder(i, j, a)){
                    if (dist[0][i][j] != -1 && dist[0][i][j] < minDist){
                        minBlock = make_pair(i,j);
                        minDist = dist[0][i][j];
                    }   
                }
            }
        speakOutLoud(track(0, minBlock.first, minBlock.second));
        return;
    }
    // vector< tuple< int, int, int, int > > moveList;
    vector< pair< int, pair<int, pair< int, int> > > > moveList;
    moveList.clear();
    int x = position[0].first;
    int y = position[0].second;
    for(int i=0; i<num_direction; i++)
        if (i != 3-direction[0]){
            int u = x + dr[i];
            int v = y + dc[i];
            if ( !inside(u,v) || a[u][v] == 2) continue; 
            
            position[0] = make_pair(u,v);
            tuonglaiDirection[0] = i;
            createState(tuonglai, a);


            if (tuonglai[u][v] == 1){  
                update(tuonglai, 0);
            } else tuonglai[u][v] = 2;

            int danger = 0;
            int ex = position[1].first;
            int ey = position[1].second;

            for(int j=0; j<num_direction; j++)
                if (j != 3-direction[1]){
                    int eu = ex + dr[j];
                    int ev = ey + dc[j];
                    if ( !inside(eu,ev) || tuonglai[eu][ev] == 4 ) continue;
                    
                    position[1] = make_pair(eu, ev);
                    tuonglaiDirection[1] = j;
                    createState(predict, tuonglai);


                    if (predict[eu][ev] == 2){
                        danger++;
                        continue;
                    }
                    if ( predict[eu][ev] == 3) {
                        update(predict, 1);
                    } else predict[eu][ev] = 4;

                    memset(dist, -1, sizeof(dist));
                    bfs(0, predict, tuonglaiDirection);
                    bfs(1, predict, tuonglaiDirection);
                    extractInformation(predict);
                    if (distToKillMe < distToSafety) {
                        danger++;
                        break;
                    }
                }
            position[1] = make_pair(ex, ey);
            memset(dist, -1, sizeof(dist));
            memset(trace, -1, sizeof(trace));
            bfs(0, tuonglai, tuonglaiDirection);
            markPath(0, tuonglai);
            extractInformation(tuonglai);
            // moveList.push_back( make_tuple(danger, -evaluate(tuonglai), distToSafety, i ));
            moveList.push_back(make_pair( danger, make_pair(-evaluate(tuonglai), make_pair(distToSafety, i))));
        }
    position[0] = make_pair(x,y);
    sort(moveList.begin(), moveList.end());
    speakOutLoud(moveList[0].second.second.second);
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
            position[i].first = x;
            position[i].second = y;
        }
        cin.ignore();
        standardize();
        getDirection();
        solve();
    }
    return 0;
}