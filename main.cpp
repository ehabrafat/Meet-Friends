#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

struct Time {
	string startStr;
	string endStr;
	double startNum;
	double endNum;
	int row, col;
	Time() : startStr("--:--"), endStr("--:--"), startNum(0), endNum(0), row{ -1 }, col{ -1 }{}
	Time(const string& startStr, const string& endStr, double startNum, double endNum) : startStr(startStr), endStr(endStr), startNum(startNum), endNum(endNum), row{ -1 }, col{ -1 } {}
	Time(const string& startStr, const string& endStr, double startNum, double endNum, int row, int col) : startStr(startStr), endStr(endStr), startNum(startNum), endNum(endNum), row{ row }, col{ col } {}
};
struct comp {
	bool operator()(const Time& b1, const Time& b2) {
		return b1.startNum > b2.startNum;
	}
};
double timeToNum(const string& time) {
	int delIdx = time.find(':');
	if (delIdx == string::npos) return 0;
	auto [hours, min] = make_tuple(time.substr(0, delIdx), time.substr(delIdx + 1));
	return stoi(hours) + ((double)stoi(min) / 60.00);
}
vector<pair<string, string>> timesToMeet(const vector<pair<string, string>>& dailyBounds, vector<vector<pair<string, string>>>& calendars, int dur)
{
	for (vector<pair<string, string>>& calendar : calendars) {
		sort(calendar.begin(), calendar.end(), [](const pair<string, string>& p1, const pair<string, string>& p2) {
			return timeToNum(p1.first) < timeToNum(p2.first);
			});
	}
	vector<pair<string, string>> availableTimes{};
	pair<string, int> maxEnd{ "--:--" , 0 };
	pair<string, int> minStart{ "--:--" , 24 };
	for (const auto& bound : dailyBounds) {
		Time cur(bound.first, bound.second, timeToNum(bound.first), timeToNum(bound.second));
		if (cur.startNum > maxEnd.second) {
			maxEnd.first = cur.startStr, maxEnd.second = cur.startNum;
		}
		if (cur.endNum < minStart.second)
			minStart.first = cur.endStr, minStart.second = cur.endNum;
	}
	priority_queue<Time, vector<Time>, comp> clsQueue{};
	for (int i = 0; i < calendars.size(); ++i) {
		auto [start, end] = make_tuple(calendars[i][0].first, calendars[i][0].second);
		clsQueue.push(Time(start, end, timeToNum(start), timeToNum(end), i, 0));
	}
	while (!clsQueue.empty())
	{
		Time topEl = clsQueue.top();
		clsQueue.pop();
		if (topEl.startNum > maxEnd.second) {
			availableTimes.push_back(make_pair(maxEnd.first, topEl.startStr));
		}
		if (topEl.col + 1 < calendars[topEl.row].size()) {
			auto p = calendars[topEl.row][topEl.col + 1];
			clsQueue.push(Time(p.first, p.second, timeToNum(p.first), timeToNum(p.second), topEl.row, topEl.col + 1));
		}
		if (topEl.endNum > maxEnd.second)
			maxEnd.first = topEl.endStr, maxEnd.second = topEl.endNum;
	}
	if (minStart.second > maxEnd.second)
		availableTimes.push_back(make_pair(maxEnd.first, minStart.first));
	vector<pair<string, string>> ans{};
	for (const auto& block : availableTimes) {
		if ((timeToNum(block.second) - timeToNum(block.first)) * 60 >= dur)
			ans.push_back(block);
	}
	return ans;
}
int main()
{
	vector<pair<string, string>> dailyBounds{
		{"8:00", "20:00"},
		{"9:00", "18:00"},
		{"6:00", "20:00"},
	};
	vector<vector<pair<string, string>>> calendars{
		{{"10:00", "12:00"}, {"15:30", "16:00"}},
		{{"11:00", "13:00"}},
		{{"11:00", "12:00"}, {"13:00", "15:00"}, {"16:15", "17:00"}},
	};
	int dur = 30;
	vector<pair<string, string>> ans = timesToMeet(dailyBounds, calendars, dur);
	if (ans.empty()) {
		cout << "There's no time to meet.\n";
		return 0;
	}
	for (const auto& avlTime : ans) {
		cout << "[" << avlTime.first << " -> " << avlTime.second << "]\n";
	}
}
