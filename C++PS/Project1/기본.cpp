#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <math.h>

using namespace std;
/* 백준 : 16173 
int N;
int graph[3][3];
int vis[3][3];
int dx[2] = {0,1};
int dy[2] = {1,0};

string bfs() {
	queue<pair<int, int>> q;
	q.push(make_pair(0, 0));
	while (!q.empty())
	{
		int x = q.front().first;
		int y = q.front().second;
		q.pop();
		if (graph[x][y] == -1)
			return "HaruHaru";
		for (int i = 0; i < 2; i++) {
			int nx = x + dx[i] * graph[x][y];
			int ny = y + dy[i] * graph[x][y];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N)
				continue;
			if (!vis[nx][ny]) {
				vis[nx][ny] = 1;
				q.push(make_pair(nx, ny));
			}
		}
	}
	return "Hing";
	
}
int main(void) {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> graph[i][j];
		}
	}
	cout << bfs();
}
*/

/* 백준 : 14716 
int dx[8] = { 1, 0, -1, 0 , 1 , 1,-1,-1 };
int dy[8] = { 0, 1, 0 , -1, -1, 1,-1, 1 };
int vis[251][251];
int graph[251][251];
int N, M, result;

void bfs(int x, int y) {
	queue<pair<int, int>> q;
	vis[x][y] = 1;
	q.push(make_pair(x, y));
	while (!q.empty())
	{
		x = q.front().first;
		y = q.front().second;
		q.pop();
		for (int i = 0; i < 8; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (ny < 0 || nx < 0 || ny >= N || nx >= M)
				continue;
			if (graph[nx][ny] == 1 && !vis[nx][ny]) {
				vis[nx][ny] = 1;
				q.push(make_pair(nx, ny));
			}
		}
	}
}

int main(void) {
	cin >> M >> N;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			cin >> graph[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (graph[i][j] == 1 && !vis[i][j]) {
				bfs(i, j);
				result++;
			}
		}
	}
	cout << result;
}
*/ 

/* 백준 : 14716 
int N, M;
string graph[101];
int vis[101][101];
int w_c, b_c;
int temp = 1;
int dx[] = { 0,0,1,-1 };
int dy[] = { 1,-1,0,0 };

void dfs(int y, int x,char d) {
	vis[y][x] = 1;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];
		if (ny < 0 || nx < 0 || ny >= M || nx >= N)
			continue;
		if (graph[ny][nx] == d && vis[ny][nx] == 0) {
			temp++;
			dfs(ny, nx, graph[ny][nx]);
		}
	}
}

int main(void) {
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		cin >> graph[i];
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (graph[i][j] == 'W' && !vis[i][j]) {
				dfs(i, j,'W');
				w_c += pow(temp,2);
				temp = 1;
			}
		}
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (graph[i][j] == 'B' && !vis[i][j]) {
				dfs(i, j,'B');
				b_c += pow(temp, 2);
				temp = 1;
			}
		}
	}
	cout << w_c << " " << b_c;
}
*/ 
/* 백준 : 11725 
int N;
vector<int> graph[100001];
int parent[100001];

void dfs(int level) {
	for (auto i : graph[level]) {
		if (parent[i])
			continue;
		parent[i] = level;
		dfs(i);
	}
}
int main(void) {
	cin >> N;
	for (int i = 1; i < N; i++) {
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}
	dfs(1);
	for (int i = 2; i <= N; i++) {
		cout << parent[i] << "\n";
	}
}
*/

/* 백준 : 2178 
string graph[101];
int vis[101][101];
int N, M;
int dx[] = { 0,0,1,-1 };
int dy[] = { 1,-1,0,0 };

void bfs(int x, int y) {
	queue<pair<int,int>> q;
	q.push(make_pair(x,y));
	vis[x][y] = 1;
	while (!q.empty())
	{
		x = q.front().first;
		y = q.front().second;
		q.pop();
		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (nx < 0 || nx >= N || ny < 0 || ny >= M)
				continue;
			if(graph[nx][ny] == '1' && !vis[nx][ny]) {
				vis[nx][ny] = vis[x][y] + 1;
				q.push(make_pair(nx, ny));
			}
		}
	}
}

int main(void) {

	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> graph[i];
	}
	bfs(0, 0);
	cout << vis[N - 1][M - 1];
}
*/ 

/* 백준 : 1260 
int N, M, V;
int graph[1001][1001];
bool vis[1001];

void dfs(int start) {
	cout << start << " ";
	vis[start] = 1;
	for (int i = 1; i <= N; i++) {
		if(!vis[i] && graph[start][i])
			dfs(i);
	}
}

void bfs(int start) {
	int t = 0;
	vis[start] = 1;
	queue<int> q;
	q.push(start);

	while (!q.empty())
	{
		t = q.front();
		q.pop();
		cout << t << " ";
		for (int i = 1; i <= N; i++) {
			if (!vis[i] && graph[t][i]) {
				q.push(i);
				vis[i] = 1;
			}
		}
	}
}

int main(void) {
	cin >> N >> M >> V;
	for (int i = 0; i < M; i++) {
		int a, b;
		cin >> a >> b;
		graph[a][b] = 1; 
		graph[b][a] = 1;
	}
	dfs(V);
	for (int i = 1; i <= N; i++)
		vis[i] = 0;
	cout << "\n";
	bfs(V);
}
*/

/* 백준 : 11404 
int n, m;
vector<vector<int>> graph(101,vector<int>(101,1e9));

void floyd() {
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (i == j){
					continue;
				}
				else {
					graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
				}
			}
		}
	}
}

int main(void) {
	cin >> n >> m;
	for (int i = 0; i <m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		graph[a-1][b-1] = min(graph[a-1][b-1], c);
	}
	floyd();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (graph[i][j] == 1e9) {
				graph[i][j] = 0;
			}
			cout << graph[i][j] << " ";
		}
		cout << "\n";
	}
}
*/
/* 백준 : 1753
vector<pair<int, int>> V[20001];
int Dist[20002];
int v,e,start;
const int INF = 98765432;

void Dijkstra(int a,int b,int c) {
	for (int i = 1; i <= 20001; i++) Dist[i] = INF;
	priority_queue<pair<int, int>> pq;
	pq.push(make_pair(0, c));
	Dist[c] = 0;

	while (pq.empty()==0) {
		int Cost = -pq.top().first;
		int Cur = pq.top().second;
		pq.pop();

		for (int i = 0; i < V[Cur].size(); i++) {
			int next = V[Cur][i].first;
			int nCost = V[Cur][i].second;

			if (Dist[next] > Cost + nCost) {
				Dist[next] = Cost + nCost;
				pq.push(make_pair(-Dist[next], next));
			}
		}
	}
	for (int i = 1; i <= a; i++) {
		Dist[i] >= INF ? cout << "INF\n" : cout << Dist[i] << "\n";
	}
}
*/
/* 백준 : 15649
int n,m;
int graph[10] = { 0 };
int vis[10] = { 0 };

void dfs(int t) {
	if (t == m) {
		for (int i = 0; i < m; i++) {
			cout << graph[i] << " ";
		}
		cout << "\n";
		return;
	}
	for (int i = 0; i < n; i++) {
		if (vis[i] == 0) {
			graph[t] = i + 1;
			vis[i] = 1;
			dfs(t + 1);
			vis[i] = 0;
		}
	}
}
*/


/* 프로그래머스 : 숫자의 표현 
int solution(int n) {
	int answer = 0;
	for (int i = 1; i <= n; i++) {
		int temp = 0;
		for (int j = i; j <= n; j++) {
			temp += j;
			if (temp == n) {
				answer += 1;
				break;
			}
			else if (temp > n)
				break;
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 멀리 뛰기 
long long solution(int n) {
	long long answer = 0;
	int dp[2001] = { 0 };
	dp[1] = 1;
	dp[2] = 2;
	for (int i = 3; i <= n; i++) {
		dp[i] = (dp[i - 1] + dp[i - 2])%1234567;
	}
	answer = dp[n];
	return answer;
}
*/
/* 프로그래머스 : 올바른 괄호 
bool solution(string s)
{
	stack<char> st;
	for (char c : s) {
		if (c == '(') {
			st.push(c);
		}
		else if (c == ')') {
			if (st.empty()) {
				return false;
			}
			st.pop();
		}
	}
	if (st.empty())
		return true;
	else
		return false;
}
*/
/* 프로그래머스 : 주식가격
vector<int> solution(vector<int> prices) {
	vector<int> answer;
	for (int i = 0; i < prices.size(); i++) {
		int time = 0;
		for (int j = i + 1; j < prices.size(); j++) {
			if (prices[i] > prices[j]) {
				time++;
				break;
			}
			else {
				time++;
			}
		}
		answer.push_back(time);
	}
	return answer;
}
*/
/* 프로그래머스 : 카펫 
vector<int> solution(int brown, int yellow) {
	vector<int> answer;
	int len = brown / 2 + 2;
	int w = len - 3;
	int h = 3;
	while (w>=h)
	{
		if (w * h == (brown + yellow))
			break;
		w--;
		h++;
	}
	return vector<int>{w,h};
}
*/
/* 프로그래머스 : H-Index
int solution(vector<int> citations) {
	sort(citations.begin(), citations.end(), greater<int>());
	for (int i = 0; i < citations.size(); i++) {
		if (citations[i] < i + 1)
			return i;
	}
}
*/


/* 프로그래머스 : 짝지어 제거하기
int solution(string s)
{
	stack<char> st;
	for (int i = 0; i < s.length(); i++) {
		if (st.size() == 0) {
			st.push(s[i]);
		}
		else if (st.top() == s[i]) {
			st.pop();
		}
		else {
			st.push(s[i]);
		}
	}

	if (st.size() == 0)
		return 1;
	else
		return 0;
}
*/

/* 프로그래머스 : 해시 
int solution(vector<vector<string>> clothes) {
	int answer = 1;
	map<string, int> cloth;

	for (int i = 0; i < clothes.size();i++) {
		if (cloth.find(clothes[i][1]) == cloth.end()) {
			cloth[clothes[i][1]] = 1;
		}
		else {
			cloth[clothes[i][1]]++;
		}
	}
	for (auto i = cloth.begin(); i != cloth.end();i++) {
		answer *= (i->second) + 1;
	}
	answer--;
	return answer;
}
*/
/* 프로그래머스 : 2 x n 타일링 
int solution(int n) {
	int answer = 0;
	const int condition = 1000000007;
	int dp[60001] = { 0 };
	dp[1] = 1;
	dp[2] = 2;

	for (int i = 3; i <= n; i++) {
		dp[i] = (dp[i - 1] + dp[i - 2]) % condition;
	}
	return dp[n]%condition;
}
*/

/* 프로그래머스 : 배달 
vector<pair<int, int>> V[55];
vector<int> Dist;

void Dijkstra(int n) {
	priority_queue<pair<int, int>> pq;
	pq.push(make_pair(0, 1));
	Dist[1] = 0;

	while (pq.empty() == 0) {
		int Cost = -pq.top().first;
		int Cur = pq.top().second;
		pq.pop();

		for (int i = 0; i < V[Cur].size(); i++) {
			int next = V[Cur][i].first;
			int nCost = V[Cur][i].second;

			if (Dist[next] > Cost + nCost) {
				Dist[next] = Cost + nCost;
				pq.push(make_pair(-Dist[next], next));
			}
		}
	}
}

int solution(int N, vector<vector<int> > road, int K) {
	Dist.resize(N + 1, 2e9);
	for (int i = 0; i < road.size(); i++) {
		int N1 = road[i][0];
		int N2 = road[i][1];
		int Dist = road[i][2];

		V[N1].push_back(make_pair(N2, Dist));
		V[N2].push_back(make_pair(N1, Dist));
	}
	Dijkstra(N);
	int answer = 0;
	for (int i = 1; i <= N; i++) {
		if (Dist[i] <= K)
			answer++;
	}
	return answer;
}
*/
/* 프로그래머스 : 괄호 회전하기 
bool check(string s) {
	stack<char> stack;
	for (char c : s) {
		if (c == '(' || c == '[' || c == '{') {
			stack.push(c);
		}
		else if (c == ')') {
			if (stack.empty())
				return false;
			if (stack.top() == '(') {
				stack.pop();
			}
		}
		else if (c == ']') {
			if (stack.empty())
				return false;
			if (stack.top() == '[') {
				stack.pop();
			}
		}
		else if (c == '}') {
			if (stack.empty())
				return false;
			if (stack.top() == '{') {
				stack.pop();
			}
		}
	}
	if (stack.empty()) {
		return true;
	}
	else {
		return false;
	}
}
int solution(string s) {
	int answer = 0;
	for (int i = 0; i < s.length(); i++) {
		if (check(s)) {
			answer++;
		}
		rotate(s.begin(), s.begin() + 1, s.end());
	}
	return answer;
}
*/

/* 프로그래머스 : 예상 대진표 
int solution(int n, int a, int b)
{
	int answer = 0;
	while (a != b) {
		answer++;
		a = (a + 1) / 2;
		b = (b + 1) / 2;
	}
	return answer;
}
*/
/* 프로그래머스 : 게임맵 최단 거리
int check[101][101];
int dx[] = { 0,0,1,-1 };
int dy[] = { 1,-1,0,0 };
int solution(vector<vector<int> > maps)
{
	int answer = 0;
	int n, m;
	n = maps.size();
	m = maps[0].size();
	queue<pair<int, int>> q;
	q.push(make_pair(0, 0));
	check[0][0] = true;

	while (!q.empty())
	{
		int x = q.front().first;
		int y = q.front().second;
		q.pop();

		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (0 <= nx && nx < n && 0 <= ny && ny < m) {
				if (check[nx][ny] == false && maps[nx][ny] > 0) {
					check[nx][ny] = true;
					maps[nx][ny] = maps[x][y] + 1;
					q.push(make_pair(nx, ny));
				}
			}
		}
	}

	if (maps[n - 1][m - 1] == 1)
		answer = -1;
	else
		answer = maps[n - 1][m - 1];

	return answer;
}

*/ 

/* 프로그래머스 : 가장 큰 수
bool compare(const string &a, const string &b) {
	if (b + a < a + b)
		return true;
	return false;
}

string solution(vector<int> numbers) {
	string answer = "";
	vector<string> strings;
	for (int i : numbers)
		strings.push_back(to_string(i));
	sort(strings.begin(), strings.end(),compare);

	for (auto a = strings.begin(); a < strings.end(); a++)
		answer += *a;

	if (answer[0] == '0')
		answer = "0";

	return answer;
}
*/
/* 프로그래머스 : 프린터 
int solution(vector<int> priorities, int location) {
	queue<int> q;
	vector<int> sorted;
	for (int i = 0; i < priorities.size(); i++) {
		q.push(i);
	}
	while (!q.empty()) {
		int now_index = q.front();
		q.pop();
		if (priorities[now_index] != *max_element(priorities.begin(), priorities.end())) {
			q.push(now_index);
		}
		else {
			sorted.push_back(now_index);
			priorities[now_index] = 0;
		}
	}
	for (int i = 0; i < sorted.size(); i++) {
		if (sorted[i] == location) return i + 1;
	}
}
*/
/* 프로그래머스 : 전화번호 목록 
bool solution(vector<string> phone_book) {
	bool answer = true;
	sort(phone_book.begin(), phone_book.end());
	for (int i = 0; i < phone_book.size() -1 ; i++) {
		if (phone_book[i] == phone_book[i + 1].substr(0, phone_book[i].size())) 
		{
			return false;
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 더 맵게 
int solution(vector<int> scoville, int K) {
	int answer = 0;
	int Hot;
	priority_queue<int> pq;

	for (int i = 0; i < scoville.size(); i++) {
		pq.push(-scoville[i]);
	}

	while (-pq.top() < K && pq.size() >= 2)
	{
		Hot = pq.top();
		pq.pop();
		pq.push(Hot + pq.top() * 2);
		pq.pop();
		answer++;
	}
	if (-pq.top() < K) {
		return -1;
	}

	return answer;
}
*/
/* 프로그래머스 : 기능 개발 
vector<int> solution(vector<int> progresses, vector<int> speeds) {
	vector<int> answer;
	queue<int> queue;

	for (int i = 0; i < progresses.size(); i++) {
		int temp = (100 - progresses[i]) % speeds[i];
		if (temp == 0) {
			queue.push((100 - progresses[i]) / speeds[i]);
		}
		else {
			queue.push((100 - progresses[i]) / speeds[i] + 1);
		}
	}

	while (!queue.empty())
	{
		int count = 1;
		int cur = queue.front();
		queue.pop();

		while (cur >= queue.front() && !queue.empty())
		{
			queue.pop();
			count++;
		}

		answer.push_back(count);
	}

	return answer;
}
*/