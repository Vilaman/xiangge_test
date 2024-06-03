#include <algorithm>
#include <iostream>
#include <vector>

struct Interval {
    int start;
    int end;
    Interval(int s, int e) : start(s), end(e) {}
};

class Solution {
public:
    std::vector<Interval> merge(std::vector<Interval>& intervals) {
        std::vector<Interval> res;
        if (intervals.empty()) {
            return res;
        }

        for (const Interval& interval : intervals) {
            std::cout << "-[" << interval.start << "," << interval.end << "] ";
        }
        std::cout << std::endl;
        std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

        for (const Interval& interval : intervals) {
            std::cout << "-[" << interval.start << "," << interval.end << "] ";
        }
        std::cout << std::endl;

        res.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); ++i) {
            if (intervals[i].start <= res.back().end) {
                res.back().end = std::max(res.back().end, intervals[i].end);
            } else {
                res.push_back(intervals[i]);
            }
        }
        return res;
    }
};

int main() {
    std::vector<Interval> intervals{{20, 60}, {10, 30}, {80, 100}, {150, 180}};
    Solution so;
    std::vector<Interval> merged = so.merge(intervals);

    for (const Interval& interval : merged) {
        std::cout << "[" << interval.start << "," << interval.end << "] ";
    }
    std::cout << std::endl;

    return 0;
}