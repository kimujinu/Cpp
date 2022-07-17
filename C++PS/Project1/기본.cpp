#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <math.h>
#include <set>
#include <sstream>

using namespace std;

/* 백준 : 24444 
int N, M, R;
vector<int> graph[100001];
vector<int> vis(100001, 0);
int cnt = 1;

void bfs(int start) {
	queue<int> q;
	vis[start] = cnt++;
	q.push(start);
	while (!q.empty())
	{
		int x = q.front();
		q.pop();

		for (int i = 0; i < graph[x].size(); i++) {
			int nx = graph[x][i];
			if (!vis[nx]) {
				vis[nx] = cnt++;
				q.push(nx);
			}
		}
	}
}

int main() {
	cin >> N >> M >> R;
	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	for (int i = 1; i <= N; i++)
		sort(graph[i].begin(), graph[i].end());
	
	bfs(R);

	for (int i = 1; i <= N;i++) {
		cout << vis[i] << "\n";
	}
}
*/

/* 백준 : 24480 
int N, M, R;
vector<int> graph[100001];
vector<int> vis(100001, 0);
int cnt = 1;

void dfs(int start) {
	vis[start] = cnt++;
	for (int x : graph[start]) {
		if (vis[x] == 0) {
			dfs(x);
		}
	}
}
int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> N >> M >> R;
	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	for (int i = 1; i <= N; i++)
		sort(graph[i].begin(), graph[i].end(),greater<int>());

	dfs(R);
	for (int i = 1; i <= N; i++) {
		cout << vis[i] << "\n";
	}
}
*/
/* 백준 : 24479 
int N, M, R;
vector<int> graph[100001];
vector<int> vis(100001,0);
int cnt = 1;

void dfs(int start) {
	vis[start] = cnt++;
	for (int x : graph[start]) {
		if (vis[x] == 0) {
			dfs(x);
		}
	}
}
int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> N >> M >> R;
	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	for (int i = 1; i <= N; i++)
		sort(graph[i].begin(), graph[i].end());

	dfs(R);
	for (int i = 1; i <= N; i++) {
		cout << vis[i] << "\n";
	}
}
*/
/* 백준 : 15900 
int N;
vector<int> tree[500001];
bool vis[500001];
int result = 0;

void dfs(int level, int count) {
	if (tree[level].size() == 1 && level != 1) {
		result += count;
		return;
	}
	for (int i = 0; i < tree[level].size(); i++) {
		if (!vis[tree[level][i]]) {
			vis[tree[level][i]] = true;
			dfs(tree[level][i], count + 1);
			vis[tree[level][i]] = false;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> N;
	for (int i = 0; i < N - 1; i++) {
		int a, b;
		cin >> a >> b;
		tree[a].push_back(b);
		tree[b].push_back(a);
	}
	vis[1] = true;
	dfs(1, 0);
	if (result % 2 == 1)
		cout << "Yes" << "\n";
	else
		cout << "No" << "\n";
}
*/

/* 백준 
int N, M;
int graph[101][101];

void floyd() {
	for (int k = 1; k <=N; k++) {
		for (int i = 1; i <=N; i++) {
			for (int j = 1; j <=N; j++) {
				if (i == j) {
					continue;
				}
				else if(graph[i][k] && graph[k][j]) {
					if (graph[i][j] == 0) {
						graph[i][j] = graph[i][k] + graph[k][j];
					}
					else {
						graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
					}
				}
			}
		}
	}
}

int main() {
	cin >> N >> M;

	for (int i = 0; i < M; i++) {
		int A, B;
		cin >> A >> B;
		graph[A][B] = graph[B][A] = 1;
	}

	floyd();

	int result = 987654321;
	int ans;
	for (int i = 1; i <= N; i++) {
		int temp = 0;
		for (int j = 1; j <= N; j++) {
			temp += graph[i][j];
		}
		if (result > temp) {
			result = temp;
			ans = i;
		}
	}
	cout << ans << "\n";
}
*/
/* 백준 : 11286 
int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	priority_queue<pair<int,int>> pq;
	int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		int temp;
		cin >> temp;
		if (temp == 0) {
			if (pq.size() > 0) {
				cout << -pq.top().second << "\n";
				pq.pop();
			}
			else {
				cout << 0 << "\n";
			}
		}
		else {
			pq.push(make_pair(-abs(temp), -temp));
		}
	}
}
*/
/* 백준 : 11279
int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	priority_queue<int> pq;
	int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		int temp;
		cin >> temp;
		if (temp == 0) {
			if (pq.size() > 0) {
				cout << pq.top() << "\n";
				pq.pop();
			}
			else {
				cout << 0 << "\n";
			}
		}
		else {
			pq.push(temp);
		}
	}
}
*/
/* 백준 : 1927 
int main() {
	
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	priority_queue<int> pq;
	int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		int temp;
		cin >> temp;
		if (temp == 0) {
			if (pq.size() > 0) {
				cout << -pq.top() << "\n";
				pq.pop();
			}
			else {
				cout << 0 << "\n";
			}
		}
		else {
			pq.push(-temp);
		}
	}
}
*/

/* 백준 : 1769 
int main() {
	string s;
	int count = 0;
	cin >> s;
	while (s.length() > 1)
	{	
		count++;
		int temp = 0;
		for (char c : s) {
			temp += c - '0';
		}
		s = to_string(temp);
	}
	cout << count << "\n";
	if (stoi(s) % 3 == 0) {
		cout << "YES";
	}
	else {
		cout << "NO";
	}
}
*/
/* 백준 : 1057 
int main() {
	int n;
	double k, i;
	int count = 0;
	cin >> n >> k >> i;

	while (k != i)
	{
		k = ceil(k / 2);
		i = ceil(i / 2);
		count++;
	}
	cout << count;
}
*/
/* 백준 : 9613 
int gcd(int a, int b) {
	if (b == 0)
		return a;
	else {
		return gcd(b, a % b);
	}
}

int main() {
	int t;
	int v[101];
	cin >> t;
	for (int i = 0; i < t; i++) {
		long long result = 0;
		int n;
		cin >> n;
		for (int j = 0; j < n; j++) {
			cin >> v[j];
		}
		for (int j = 0; j < n; j++) {
			for (int k = j+1; k < n; k++) {
				result += gcd(v[j], v[k]);
			}
		}
		cout << result << "\n";
	}
}
*/

/* 백준 : 10974 
int N,result = 0;
vector<int> V;
int vis[9];

void backtracking(int value,vector<int> v,int level) {
	if (level == N) {
		for (int i : v) {
			cout << i << " ";
		}
		cout << "\n";
	}
	for (int i = 1; i <= N; i++) {
		if (!vis[i])
		{
			vis[i] = 1;
			v.push_back(i);
			backtracking(i, v, level+1);
			v.pop_back();
			vis[i] = 0;;
		}
	}
}

int main() {
	cin >> N;
	backtracking(0,V,0);
}
*/

/* 백준 : 1476 
int main() {
	int E, S, M;
	int year = 1;
	cin >> E >> S >> M; // 15 28 19
	while (true)
	{
		if ((year - E) % 15 == 0 && (year - S) % 28 == 0 && (year - M) % 19 == 0) {
			cout << year;
			exit(0);
		}
		else {
			year++;
		}
	}
}
*/
/* 백준 : 1436 
int main()
{
	int n, count = 0,result=0;
	cin >> n;
	int ans = 0;
	while (count != n)
	{
		result++;
		int temp = result;
		while (temp != 0)
		{
			if (temp % 1000 == 666) {
				++count;
				break;
			}
			else {
				temp /= 10;
			}
		}
	}
	cout << result;
}
*/
/* 백준 : 2668 
int N;
vector<int> v;
int graph[101];
int vis[101];
int result = 0;

void dfs(int level,int cur) {
	if (vis[cur] == 0) {
		vis[cur] = level;
		dfs(level, graph[cur]);
	}
	else if (cur == level) {
		v.push_back(level);
		return;
	}
}

int main() {
	cin >> N;
	for (int i = 1; i <=N; i++) {
		cin >> graph[i];
	}
	for (int i = 1; i <= N; i++) {
		memset(vis, 0, sizeof(vis)); // 메모리 특정값으로 세팅
		dfs(i,i);
	}
	cout << v.size() << "\n";
	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << "\n";
	}
}
*/
/* 백준 : 13565 
string graph[1001];
int vis[1001][1001];
int dx[] = { 0,0,1,-1 };
int dy[] = { 1,-1,0,0 };
int M, N;
string result = "NO";

void dfs(int a,int b) {
	if (a == M - 1)
		result = "YES";
	for (int i = 0; i < 4; i++) {
		int nx = a + dx[i];
		int ny = b + dy[i];
		if (ny < 0 || nx < 0 || ny >= N || nx >= M)
			continue;
		if (graph[nx][ny] == '0' && !vis[nx][ny]) {
			vis[nx][ny] = 1;
			dfs(nx, ny);
		}
	}
}

int main() {
	cin >> M >> N;
	for (int i = 0; i < M; i++) {
		cin >> graph[i];
	}
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < N; j++) {
			if (graph[i][j] == '0') {
				vis[i][j] = 1;
				dfs(i, j);
			}
		}
	}
	cout << result;
}
*/
/* 백준 : 1009
int main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int a, b;
		cin >> a >> b;
		int result = 1;
		for (int j = 0; j < b;j++) {
			result = (result * a) % 10;
		}
		if (result == 0) {
			result = 10;
		}
		cout << result << "\n";
	}
}
*/
/* 백준 : 2455 
int main() {
	int result = 0;
	int temp = 0;
	int a, b;
	cin >> a >> b;
	temp = b;
	for (int i = 0; i < 3; i++) {
		int c, d;
		cin >> c >> d;
		temp = temp + (d - c);
		result = max(result, temp);
	}
	cout << result;
}
*/
/* 백준 : 10162 
int main() {
	int T;
	cin >> T;
	int A = 0;
	int B = 0;
	int C = 0;
	if (T % 10 != 0) {
		cout << "-1";
	}else {
		A = T / 300;
		B = (T % 300) / 60;
		C = ((T % 300) % 60) / 10;
		cout << A << " " << B << " " << C;
	}
	return 0;
}
*/
/* 백준 : 13305 

int dis[100000];
int city[100000];

int main() {
	int N;
	long long result = 0;
	cin >> N;
	for (int i = 0; i < N - 1; i++) {
		cin >> dis[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> city[i];
	}
	long long value = 1000000000;
	for (int i = 0; i < N-1; i++) {
		if (city[i] < value) {
			value = city[i];
		}
		result += value * dis[i];
	}
	cout << result;
}
*/
/* 백준 : 1789 
int main() {
	long long S;
	cin >> S;
	int count = 0;
	int num = 1;
	long long total = 0;
	while (total <= S)
	{
		total += num;
		count++;
		num++;
	}
	cout << count - 1;
}
*/
/* 백준 : 1453 
int main()
{
	int N,result=0;
	cin >> N;
	int array[101] = {0};
	for (int i = 0; i < N; i++) {
		int temp;
		cin >> temp;
		if (!array[temp])
			array[temp] = 1;
		else
			result++;
	}
	cout << result;
}
*/
/* 백준 : 2161 
int main()
{
	int N;
	cin >> N;
	queue<int> q;
	for (int i = 1; i <= N; i++) {
		q.push(i);
	}
	while (q.size() > 1)
	{
		int temp = q.front();
		q.pop();
		int temp2 = q.front();
		q.pop();
		q.push(temp2);
		cout << temp << " ";
	}
	cout << q.front();
}
*/
/* 백준 : 3085
int N;
int result = 1;
char graph[51][51];

void check() {
	int temp = 1;
	for (int i = 0; i < N; i++) {
		temp = 1;
		for (int j = 0; j < N; j++) {
			if (graph[i][j] == graph[i][j + 1]) {
				temp++;
			}
			else {
				temp = 1;
			}
			result = max(temp, result);
		}
	}

	for (int i = 0; i < N; i++) {
		temp = 1;
		for (int j = 0; j < N; j++) {
			if (graph[j][i] == graph[j+1][i]) {
				temp++;
			}
			else {
				temp = 1;
			}
			result = max(temp, result);
		}
	}
}

int main() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> graph[i][j];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N-1; j++) {
			swap(graph[i][j], graph[i][j + 1]);
			check();
			swap(graph[i][j], graph[i][j + 1]);

			swap(graph[j][i], graph[j + 1][i]);
			check();
			swap(graph[j][i], graph[j + 1][i]);
		}
	}
	cout << result;
}
*/

/* 백준 : 2740 (행렬 곱셈)
int v1[101][101];
int v2[101][101];
int result[101][101];

int main() {
	int N, M, K;
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> v1[i][j];
		}
	}
	cin >> M >> K;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < K; j++) {
			cin >> v2[i][j];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < K; j++) {
			for (int k = 0; k < M; k++) {
				result[i][j] += v1[i][k] * v2[k][j];
			}
			cout << result[i][j] << " ";
		}
		cout << "\n";
	}
}
*/
/*백준 : 11723 (비트마스크) 
  And 연산 (&): 대응하는 두 비트가 모두 1이면 1 반환
  1010 & 1111 = 1010
  OR 연산 ( | ): 대응하는 두 비트 중 하나라도 1이면 1 반환
  1010 & 1111 = 1111
  Shift 연산 («,») : 비트를 왼쪽 또는 오른쪽으로 한칸씩 이동
  0011 « 2 = 1100
  1010 » 1 = 0101
  XOR 연산 (^): 대응하는 두 비트가 다르면 1 같으면 0 반환
  1010 ^ 1100 = 0110
  Not 연산 (~) : 비트를 반전
  ~1111 = 0000

int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int T;
	cin >> T;

	string s;
	int value, bit = 0;
	for (int i = 0; i < T; i++) {
		cin >> s;
		if (s == "add") {
			cin >> value;
			bit |= (1 << value);
		}
		else if (s == "remove") {
			cin >> value;
			bit &= ~(1 << value);
		}
		else if (s == "check") {
			cin >> value;
			if (bit & (1 << value))
				cout << 1 << '\n';
			else
				cout << 0 << '\n';
		}
		else if (s == "toggle") {
			cin >> value;
			bit ^= (1 << value);
		}
		else if (s == "all") {
			bit = (1 << 21) - 1;
		}
		else if (s == "empty") {
			bit = 0;
		}
	}
	return 0;
}
*/

/* 백준 : 2775 
int recursion(int a, int b) {
	if (a == 0)
		return b;
	if (b == 1)
		return 1;
	return recursion(a - 1, b) + recursion(a, b - 1);
}

int main()
{
	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int k, n;
		cin >> k >> n;
		cout << recursion(k, n) << "\n";
	}
}
*/
/* 백준 : 10926 
int main()
{
	string s;
	cin >> s;
	cout << s << "??!";
}
*/
/* 백준 : 14490 
vector<string> split(string input, char delimiter) { // split 직접 구현
	vector<string> result;
	stringstream ss(input);
	string temp;

	while (getline(ss,temp,delimiter))
	{
		result.push_back(temp);
	}
	return result;
}

int gcd(int a, int b) {
	if (b == 0)
		return a;
	else {
		return gcd(b, a % b);
	}
}

int main() {
	string s;
	cin >> s;
	vector<string> v;
	v = split(s, ':');
	int N = gcd(stoi(v[0]), stoi(v[1]));
	cout << stoi(v[0])/N << ":" << stoi(v[1])/N;
}
*/

/* 백준 : 18406
int main() {
	int N;
	cin >> N;
	int result1 = 0;
	int result2 = 0;
	string temp = to_string(N);
	string temp_1 = temp.substr(0,temp.length()/2);
	string temp_2 = temp.substr(temp.length() / 2 ,temp.length());
	for (int i = 0; i < temp.length() / 2; i++) {
		result1 += (temp_1[i] - '0');
		result2 += (temp_2[i] - '0');
	}
	if (result1 == result2)
		cout << "LUCKY";
	else
		cout << "READY";
}
*/
/*백준 : 11478 
int main() {
	string s;
	cin >> s;
	set<string> set;
	string result = "";
	for (int i = 0; i < s.size(); i++) {
		for (int j = i; j < s.size(); j++) {
			result += s[j];
			set.insert(result);
		}
		result = "";
	}
	cout << set.size();
}
*/
/* 백준 : 7567 
int main() {
	string s;
	cin >> s;
	int result = 10;
	for (int i = 1; i < s.length(); i++) {
		if (s[i - 1] == s[i])
			result += 5;
		else
			result += 10;
	}
	cout << result;
}
*/
/* 백준 : 1652
int main() {
	int N;
	int result1 = 0;
	int result2 = 0;
	string v[101];
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> v[i];
	}
	for (int i = 0; i < N; i++) {
		int j = 0;
		for (j = 0; j < N-1; j++) {
			if (v[i][j] == '.' && (v[i][j] == v[i][j + 1])) {
				result1++;
				while (v[i][j] != 'X' && j<N-1)
				{
					j++;
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N-1; j++) {
			if (v[j][i] == '.' && (v[j][i] == v[j+1][i])) {
				result2++;
				while (v[j][i] != 'X' && j < N - 1)
				{
					j++;
				}
			}
		}
	}
	cout << result1 << " " << result2;
}
*/
/* 백준 : 하얀 칸 
int main() {
	string v[8];
	int result = 0;
	for (int i = 0; i < 8; i++) {
		cin >> v[i];
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0 && v[i][j] == 'F') {
				result++;
			}
		}
	}
	cout << result;
}
*/
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