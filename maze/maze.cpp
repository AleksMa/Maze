#include <bits/stdc++.h>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

struct point {
  point() {};
  point(int x, int y) : x(x), y(y) {};
  int x;
  int y;
};

bool is_exit(int H, int W, int i, int j, const vector<string> &maze) {
  return (i == 0 || i == H - 1 || j == 0 || j == W - 1) && maze[i][j] == ' ';
}

int amount(int i, int j, const vector<string> &maze){
  return maze[i][j] == '.' ? 2 : 1;
}

int main() {

  vector<string> maze;
  point start, exit;

  string temp;
  //for(int i = 0; i < 4; i++){
  while (cin) {
    std::getline(cin, temp);
    int start_pos = temp.find('s');
    if (start_pos != -1)
      start = point(start_pos, maze.size());
    maze.push_back(temp);

  }

  int H = maze.size(), W = maze[0].size();
  vector<vector<int>> routes(H, vector<int>(W, INT_MAX));
  routes[start.y][start.x] = 0;

  bool escape = false;
  std::queue<point> Q;
  Q.push(start);
  while (!escape && !Q.empty()) {
    point cur = Q.front();
    Q.pop();
    if(is_exit(H, W, cur.y, cur.x, maze)){
      exit = point(cur.x, cur.y);
      escape = true;
      break;
    }
    if (cur.y + 1 < H && maze[cur.y + 1][cur.x] != '#' && routes[cur.y + 1][cur.x] > routes[cur.y][cur.x] + 1) {
      routes[cur.y + 1][cur.x] = routes[cur.y][cur.x] + amount(cur.y, cur.x, maze);
      Q.push(point(cur.x, cur.y + 1));
    }
    if (cur.x + 1 < W && maze[cur.y][cur.x + 1] != '#' && routes[cur.y][cur.x + 1] > routes[cur.y][cur.x] + 1) {
      routes[cur.y][cur.x  + 1] = routes[cur.y][cur.x] + amount(cur.y, cur.x, maze);;
      Q.push(point(cur.x  + 1, cur.y));
    }
    if (cur.y - 1 >= 0 && maze[cur.y - 1][cur.x] != '#' && routes[cur.y - 1][cur.x] > routes[cur.y][cur.x] + 1) {
      routes[cur.y - 1][cur.x] = routes[cur.y][cur.x] + amount(cur.y, cur.x, maze);;
      Q.push(point(cur.x, cur.y - 1));
    }
    if (cur.x - 1 >= 0 && maze[cur.y][cur.x - 1] != '#' && routes[cur.y][cur.x - 1] > routes[cur.y][cur.x] + 1) {
      routes[cur.y][cur.x - 1] = routes[cur.y][cur.x] + amount(cur.y, cur.x, maze);;
      Q.push(point(cur.x - 1, cur.y));
    }
  }

  point t;
  t.x = exit.x;
  t.y = exit.y;

  while(true){
    if(t.x > 0 && routes[t.y][t.x - 1] < routes[t.y][t.x]){
      maze[t.y][t.x--] = '*';
    }
    else if(t.y > 0 && routes[t.y - 1][t.x] < routes[t.y][t.x]){
      maze[t.y--][t.x] = '*';
    }
    else if(t.x < W - 1 && routes[t.y][t.x + 1] < routes[t.y][t.x]){
      maze[t.y][t.x++] = '*';
    }
    else if(t.y < H - 1 && routes[t.y + 1][t.x] < routes[t.y][t.x]){
      maze[t.y++][t.x] = '*';
    }
    if(t.x == start.x && t.y == start.y)
      break;
  }


  for (int i = 0; i < maze.size(); i++) {
    cout << maze[i];
    if(i < H - 1)
      cout << endl;
  }

  return 0;

}
