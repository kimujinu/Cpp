#include <bits/stdc++.h>
using namespace std;

int main(void) {

}
/* 프로그래머스 : 문자열 내 마음대로 정렬하기 
int cn;
bool compare(string a, string b) {
	if (a[cn] != b[cn]) {
		return a[cn] < b[cn];
	}
	return a < b;
}
vector<string> solution(vector<string> strings, int n) {
	cn = n;
	sort(strings.begin(), strings.end(), compare);
	return strings;
}
*/
/* 프로그래머스 : 두 정수 사이의 합
long long solution(int a, int b) {
	long long answer = 0;
	if (a > b) {
		for (int i = b; i <= a; i++) {
			answer += i;
		}
	}
	else {
		for (int i = a; i <= b; i++) {
			answer += i;
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 나누어 떨어지는 배열
vector<int> solution(vector<int> arr, int divisor) {
	vector<int> answer;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] % divisor == 0) {
			answer.push_back(arr[i]);
		}
	}
	if (answer.size() == 0) {
		answer.push_back(-1);
	}
	sort(answer.begin(), answer.end());
	return answer;
} 
*/
/* 프로그래머스 : 같은 숫자는 싫어
vector<int> solution(vector<int> arr)
{
	arr.erase(unique(arr.begin(), arr.end()), arr.end());
	return arr;
}
*/
/* 프로그래머스 : 가운데 글자 가져오기 
string solution(string s) {
	string answer = "";
	if (s.length() % 2 == 0) {
		return s.substr(s.length() / 2 - 1, 2);
	}
	else {
		return s.substr(s.length() / 2, 1);
	}
	return answer;
}
*/
/* 프로그래머스 : 부족한 금액 계산하기
long long solution(int price, int money, int count)
{
	long long answer = 0;
	for (int i = 1; i <= count; i++) {
		answer += price * i;
	}
	if (money >= answer) {
		return 0;
	}
	return answer-money;
}
*/
/* 프로그래머스 : 나머지가 1이되는 수 찾기
int solution(int n) {
	int answer = 0;
	for (int i = 1; i <= n; i++) {
		if (n % i == 1) {
			return i;
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 직사각형 
int solution(vector<vector<int>> sizes) {
	int answer = 0;
	int w = 0;
	int h = 0;
	for (int i = 0; i < sizes.size(); i++) {
		w = max(w, min(sizes[i][0], sizes[i][1]));
		h = max(h, max(sizes[i][0], sizes[i][1]));
	}
	answer = w * h;
	return answer;
}
*/
/* 프로그래머스 : 2016년 
string solution(int a, int b) {
	string answer = "";
	vector<string> date = { "FRI", "SAT", "SUN", "MON", "TUE", "WED", "THU" };
	vector<int> month = {31,29,31,30,31,30,31,31,30,31,30,31};
	int diff = b - 1;
	for (int idx = 0; idx < a - 1; idx++) {
		diff += month[idx];
	}
	return date[diff % 7];
}
*/
/*프로그래머스 : 두개 뽑아서 더하기
vector<int> solution(vector<int> numbers) {
	vector<int> answer;
	for (int i = 0; i < numbers.size(); i++) {
		for (int j = i + 1; j < numbers.size(); j++) {
			answer.push_back(numbers[i] + numbers[j]);
		}
	}
	sort(answer.begin(), answer.end());
	answer.erase(unique(answer.begin(), answer.end()),answer.end()); // 중복제거
	return answer;
}
*/

/* 프로그래머스 : 예산
int solution(vector<int> d, int budget) {
	int answer = 0;
	int temp = 0;
	sort(d.begin(), d.end());
	for (int i = 0; i < d.size(); i++) {
		if (budget >= temp+d[i]) {
			temp += d[i];
			answer++;
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 체육복
int solution(int n, vector<int> lost, vector<int> reserve) {
	int answer = n - lost.size();

	sort(lost.begin(), lost.end());
	sort(reserve.begin(), reserve.end());

	for (int i = 0; i < lost.size(); i++) {
		for (int j = 0; j < reserve.size(); j++) {
			if (lost[i] == reserve[j]) {
				lost[i] = -1;
				reserve[j] = -1;
				answer++;
			}
		}
	}

	for (int i = 0; i < lost.size(); i++) {
		for (int j = 0; j < reserve.size(); j++) {
			if (lost[i] + 1 == reserve[j] || lost[i] - 1 == reserve[j]) {
				reserve.erase(reserve.begin() + j);
				answer++;
				break;
			}
		}
	}
	return answer;
}
*/
/* 프로그래머스 : 모의고사
const vector<int> one = { 1,2,3,4,5 };
const vector<int> two = { 2,1,2,3,2,4,2,5 };
const vector<int> thr = { 3,3,1,1,2,2,4,4,5,5 };

vector<int> solution(vector<int> answers) {
	vector<int> answer;
	vector<int> temp(3);
	for (int i = 0; i < answers.size(); i++) {
		if (answers[i] == one[i % 5]) {
			temp[0]++;
		}
		if (answers[i] == two[i % 8]) {
			temp[1]++;
		}
		if (answers[i] == thr[i % 10]) {
			temp[2]++;
		}
	}
	int temp_max = *max_element(temp.begin(), temp.end());
	for (int i = 0; i < 3; i++) {
		if (temp[i] == temp_max) {
			answer.push_back(i+1);
		}
	}
	return answer;
}
*/
/* 프로그래머스 : K번째수 
vector<int> solution(vector<int> array, vector<vector<int>> commands) {
	vector<int> answer;
	vector<int> temp;

	for (int i = 0; i < commands.size(); i++) {
		temp = array;
		sort(temp.begin() + commands[i][0]-1, temp.begin() + commands[i][1]);
		answer.push_back(temp[commands[i][0] + commands[i][2] - 2]);
	}
	return answer;
}
*/
/*
string solution(vector<string> participant, vector<string> completion) {
	string answer = "";
	sort(participant.begin(), participant.end());
	sort(completion.begin(), completion.end());
	for (int i = 0; i < participant.size(); i++) {
		if (participant[i] != completion[i]) {
			return participant[i];
		}
	}
	return participant[participant.size()-1];
}
*/
/*
bool checkPrime(int x) {
	if (x < 2) {
		return false;
	}
	for (int i = 2; i <= sqrt(x); i++) {
		if (x%i == 0) {
			return false;
		}
	}
	return true;
}

int solution(vector<int> nums) {
	int answer = 0;
	for (int i = 0; i < nums.size(); i++) {
		for (int j = i+1; j < nums.size(); j++) {
			for (int k = j+1; k < nums.size(); k++) {
				if (checkPrime(nums[i] + nums[j] + nums[k])) {
					answer++;
				}
			}
		}
	}
	return answer;
}
*/
/*
int solution(vector<int> a, vector<int> b) {
	int answer = 0;
	for (int i = 0; i < a.size(); i++) {
		answer += (a[i] * b[i]);
	}
	return answer;
}
*/
/* 프로그래머스
int solution(vector<int> absolutes, vector<bool> signs) {
	int answer = 0;
	for (int i = 0; i < absolutes.size(); i++) {
		if (signs[i]) {
			answer += absolutes[i];
		}
		else {
			answer -= absolutes[i];
		}
	}
	return answer;
}
*/
/* 프로그래머스 
int solution(vector<int> numbers) {
	int answer = 0;
	bool sample[10] = { 0,0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < numbers.size(); i++) {
		sample[numbers[i]] = true;
	}
	for (int i = 0; i < 10; i++) {
		if (!sample[i]) {
			answer += i;
		}
	}
	return answer;
}
*/

/* 11170 : 0의 개수
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int t,N,M,result;
	cin >> t;
	for (int i = 0; i < t; i++) {
		result = 0;
		cin >> N >> M;
		for (int j = N; j <= M; j++) {
			string a = to_string(j);
			for (int k = 0; k < a.length(); k++) {
				if (a[k] == '0') {
					result += 1;
				}
			}
		}
		cout << result << "\n";
	}
}
*/
/*3058 : 짝수를 찾아라
int main(void) {
    ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int t,a,resultA,resultB;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int a[7];
		resultA = 0;
		resultB = 100;
		for (int j = 0; j < 7; j++) {
			cin >> a[j];
		}
		sort(a,a+7);
		for (int j = 0; j < 7; j++) {
			if (a[j] % 2 == 0) {
				resultA += a[j];
				if (resultB > a[j]) {
					resultB = a[j];
				}
			}
		}
		cout << resultA << " " << resultB << "\n";
	}
}
*/
/* 2744 : 대소문자 바꾸기
int main(void) {
    ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	string str;
	cin >> str;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] >= 65 && str[i] <= 90) {
			str[i] += 32;
		}
		else if (str[i] >= 97 && str[i] <= 122) {
			str[i] -= 32;
		}
	}
	cout << str;
}
*/
/*
int Rev(int);
int main(void) {
    ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int X, Y;
	cin >> X >> Y;
	cout << Rev(Rev(X) + Rev(Y));
}

int Rev(int x) {
	string a;
	a = to_string(x);
	reverse(a.begin(),a.end());
	return stoi(a);
}
*/
/*
int main(void) {
    ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int N,M;
	cin >> N >> M;
	string result;
	string str = to_string(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < str.length(); j++) {
			if (result.length() < M) {
				result += str[j];
			}
		}
	}
	cout << result;
}
*/
/*
int n, xi, result;
int main(void) {
    ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		result = 0;
		vector<int> v;
		cin >> n;
		for (int j = 0; j < n; j++) {
			cin >> xi;
			v.push_back(xi);
		}
		sort(v.begin(),v.end());
		for (int j = n-1; j > 0; j--) {
			result += v[j] - v[j-1];
		}
		cout << 2 * result << "\n";
	}
}
*/
/* 14582 : 오늘도 졌다
int jemiScore, startScore;
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	string result = "No";
	int jeminis[9];
	int startlink[9];
	for (int i = 0; i < 9; i++) {
		cin >> jeminis[i];
	}
	for (int i = 0; i < 9; i++) {
		cin >> startlink[i];
	}
	for (int i = 0; i < 9; i++) {
		jemiScore += jeminis[i];
		if (jemiScore > startScore) {
			result = "Yes";
		}
		startScore += startlink[i];
	}
	cout << result;
}
*/
/*
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	string str;
	cin >> str;
	vector<string> v;
	for (int i = 0; i < str.length(); i++) {
		v.push_back(str.substr(i));
	}
	sort(v.begin(), v.end());
	for (string i : v) {
		cout << i << "\n";
	}
}
*/
/* 2693 : N번째 큰 수
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	
	int T,a;
	cin >> T;
	for (int i = 0; i < T; i++) {
		vector<int> v;
		for (int j = 0; j < 10; j++) {
			cin >> a;
			v.push_back(a);
		}
		sort(v.begin(), v.end());
		cout << v[7] << "\n";
	}
}
*/

/* 9076 : 점수 집계
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int T,a;
	cin >> T;
	for (int i = 0; i < T; i++) {
		vector<int> v;
		for (int j = 0; j < 5; j++) {
			cin >> a;
			v.push_back(a);
		}
		sort(v.begin(), v.end());
		if (v[3] - v[1] >= 4) {
			cout << "KIN\n";
		}
		else
		{
			cout << v[1] + v[2] + v[3] << "\n";
		}
	}
}
*/

/* 5800 : 성적통계
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	
	int K,a,b,c;
	cin >> K;
	for (int i = 0; i < K; i++) {
		vector<int> v;
		cin >> a;
		c = 0;
		for (int j = 0; j < a; j++) {
			cin >> b;
			v.push_back(b);
		}
		sort(v.begin(), v.end(), greater<int>());
		for (int e = 0; e < a - 1; e++) {
			if (v[e] - v[e + 1] > c) {
				c = v[e] - v[e + 1];
			}
		}
		cout << "Class " << i + 1 << "\n";
		cout << "Max " << v[0] << ", " << "Min " << v[a - 1] << ", " << "Largest gap " << c << "\n";
	}
}
*/

/* 5635 : 생일
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int n;
	cin >> n;
	vector<pair<pair<int, int>, pair<int, string>>>name(n);
	for (int i = 0; i < n; i++) {
		cin >> name[i].second.second >> name[i].second.first >> name[i].first.second >> name[i].first.first;
	}
	sort(name.begin(), name.end());
	cout << name[n - 1].second.second << "\n" << name[0].second.second;
}
*/
/*
bool prime_check(int);
int M, N, primeTotal, primeMin;
int main(void) {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	cin >> M;
	cin >> N;
	for (int i = N; i >= M; i--) {
		if (prime_check(i)) {
			primeTotal += i;
			primeMin = i;
		}
	}
	if (!primeTotal) {
		cout << "-1";
	}
	else {
		cout << primeTotal << "\n" << primeMin;
	}

}

bool prime_check(int x) {
	if (x == 1) {
		return false;
	}
	for (int i = 2; i<=int(sqrt(x)); i++) {
		if (x%i == 0) {
			return false;
		}
	}
	return true;
}
*/