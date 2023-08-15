#include<bits/stdc++.h>
using namespace std;
#define fi         first
#define se         second
#define pii pair<int,int>


struct node {
    int art, brt, priority, in;
};
struct fnode{
    int st, art, endt, brt, in;
};
// comparator
bool fcfs_cmp(node a, node b) {
    if(a.art < b.art) return true;
    if(a.art == b.art && a.in < b.in) return true;
    return false;
}
bool sjf_cmp(node a, node b) {
    if(a.art < b.art) return true;
    if(a.art == b.art && a.brt < b.brt)
    return true;
    return false;
}
bool priority_cmp(node a, node b) {
    if(a.art < b.art) return true;
    if(a.art == b.art && a.priority < b.priority) return true;
    return false;
}
bool round_robin_cmp(node a, node b) {
    if(a.art < b.art) return true;
    if(a.art == b.art && a.in < b.in) return true;
    return false;
}
bool own_algorithm_cmp(node a, node b) {
    if(a.art < b.art) return true;
    if(a.art == b.art && a.in < b.in) return true;
    return false;
}
// final print
double avg_rt(vector<fnode> v) {
    double sum = 0;
    for(auto it : v) sum += it.st - it.art;
    sum =  sum / v.size();
    return sum;
}
double avg_wt(vector<fnode> v) {
    double sum = 0;
    for(auto it : v) sum += it.endt - it.art - it.brt;
    sum =  sum / v.size();
    return sum;
}
double avg_tt(vector<fnode> v) {
    double sum = 0;
    for(auto it : v) sum += it.endt - it.art;
    sum =  sum / v.size();
    return sum;
}
void print(vector<fnode> ans, vector<pii> chart) {
    cout << "Gantt Chart\n";
    for(auto it : chart) {
        cout << it.fi << " P" << it.se + 1 << " " ;
    }
    cout << endl << endl;
    for(auto it : ans) {
        cout << "Process: P" << it.in + 1 << " Start time: " <<  setw(3) << it.st << " End time:" << setw(3) << it.endt << " Waiting time: " << setw(3) << it.endt - it.art - it.brt  << " Turnaround time: " << setw(3) << it.endt - it.art << endl;
    }
    cout << endl;
    cout << "Average Response Time: " << avg_rt(ans) << endl;
    cout << "Average waiting time : " << avg_wt(ans) << endl;
    cout << "Average turnaround time : " << avg_tt(ans) << endl;
}
// process part
struct fcfs {
    int n;
    vector<node> v;
    vector<fnode> ans;
    vector<pii> chart;
    fcfs(int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n);
    }

    void process() {
        sort(v.begin(), v.end(), fcfs_cmp);
        int cur = 0, i = 0;
        for(auto it : v) {
            ans[i].in = it.in;
            ans[i].st = cur;
            ans[i].art = it.art;
            ans[i].endt = cur + it.brt;
            ans[i].brt = it.brt;
            if(chart.size() == 0 || chart.back().se != it.in) chart.push_back({cur, it.in});
            cur += it.brt;
            i++;
        }
    }

};
struct non_Preemptive_sjf {
    int n;
    vector<node> v;
    vector<fnode> ans;
    vector<pii> chart;
    non_Preemptive_sjf (int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n);
    }
    void process() {
        sort(v.begin(), v.end(), sjf_cmp);
        int total_time = 0;
        for(int i = 0; i < n; i++) {
            total_time += v[i].brt;
            ans[i].st = -1;
        }
        priority_queue<pii, vector<pii>, greater<pii> > q;
        int tm = 0; // current time
        q.push({v[0].brt, 0});
        for(int i = 1; i < v.size(); i++) {
            while(q.size() && tm < v[i].art) {
                pii p = q.top();
                q.pop();
                chart.push_back({tm, v[p.se].in});
                ans[v[p.se].in].st = tm;
                ans[v[p.se].in].art = v[p.se].art;
                ans[v[p.se].in].brt = v[p.se].brt;
                ans[v[p.se].in].in = v[p.se].in;
                tm += v[p.se].brt;
                ans[v[p.se].in].endt = tm;
            }
            tm = max(tm, v[i].art);
            q.push({v[i].brt, i});
        }
        while(q.size()) {
            pii p = q.top();
            q.pop();
            chart.push_back({tm, v[p.se].in});
            ans[v[p.se].in].st = tm;
            ans[v[p.se].in].art = v[p.se].art;
            ans[v[p.se].in].brt = v[p.se].brt;
            ans[v[p.se].in].in = v[p.se].in;
            tm += v[p.se].brt;
            ans[v[p.se].in].endt = tm;
        }
    }
};
struct Preemptive_sjf {
    int n;
    vector<node> v;
    vector<fnode> ans;
    vector<pii> chart;
    Preemptive_sjf(int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n);
    }
    void process() {
        sort(v.begin(), v.end(), sjf_cmp);
        for(int i = 0; i < n; i++) {
            ans[i].st = -1;
        }
        priority_queue<pii, vector<pii>, greater<pii> > q;
        int tm = 0; // current time
        q.push({v[0].brt, 0});
        for(int i = 1; i < v.size(); i++) {
            while(q.size() && tm < v[i].art) {
                pii p = q.top();
                q.pop();
                if(chart.size() == 0 || chart.back().se != v[p.se].in) chart.push_back({tm, v[p.se].in});
                if(ans[v[p.se].in].st == -1) {
                    ans[v[p.se].in].st = tm;
                    ans[v[p.se].in].art = v[p.se].art;
                    ans[v[p.se].in].brt = v[p.se].brt;
                    ans[v[p.se].in].in = v[p.se].in;
                }
                p.fi--;
                tm++;
                if(p.fi == 0) {
                    ans[v[p.se].in].endt = tm;
                }
                else q.push({p.fi, p.se});
            }
            tm = v[i].art;
            q.push({v[i].brt, i});
        }
        while(q.size()) {
            pii p = q.top();
            q.pop();
            if(chart.size() == 0 || chart.back().se != v[p.se].in) chart.push_back({tm, v[p.se].in});
            if(ans[v[p.se].in].st == -1) {
                ans[v[p.se].in].st = tm;
                ans[v[p.se].in].art = v[p.se].art;
                ans[v[p.se].in].brt = v[p.se].brt;
                ans[v[p.se].in].in = v[p.se].in;
            }
            tm = tm + p.fi;
            ans[v[p.se].in].endt = tm;
        }
    }
};
struct non_Preemptive_priority {
    int n;
    vector<node> v;
    vector<fnode> ans;
    vector<pii> chart;
    non_Preemptive_priority (int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n);
    }
    void process() {
        sort(v.begin(), v.end(), priority_cmp);
        priority_queue<pii, vector<pii>, greater<pii> > q;
        int tm = 0; // current time
        q.push({v[0].priority, 0});
        for(int i = 1; i < v.size(); i++) {
            while(q.size() && tm < v[i].art) {
                pii p = q.top();
                q.pop();
                chart.push_back({tm, v[p.se].in});
                ans[v[p.se].in].st = tm;
                ans[v[p.se].in].art = v[p.se].art;
                ans[v[p.se].in].brt = v[p.se].brt;
                ans[v[p.se].in].in = v[p.se].in;
                tm += v[p.se].brt;
                ans[v[p.se].in].endt = tm;
            }
            tm = max(tm, v[i].art);
            q.push({v[i].priority, i});
        }
        while(q.size()) {
            pii p = q.top();
            q.pop();
            chart.push_back({tm, v[p.se].in});
            ans[v[p.se].in].st = tm;
            ans[v[p.se].in].art = v[p.se].art;
            ans[v[p.se].in].brt = v[p.se].brt;
            ans[v[p.se].in].in = v[p.se].in;
            tm += v[p.se].brt;
            ans[v[p.se].in].endt = tm;
        }
    }
};
struct Preemptive_priority {
    int n;
    vector<node> v;
    vector<fnode> ans;
    vector<pii> chart;
    Preemptive_priority (int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n);
    }
    void process() {
        sort(v.begin(), v.end(), priority_cmp);
        for(int i = 0; i < n; i++) {
            ans[i].st = -1;
        }
        priority_queue<pii, vector<pii>, greater<pii> > q;
        int tm = 0; // current time
        q.push({v[0].priority, 0});
        for(int i = 1; i < v.size(); i++) {
            while(q.size() && tm < v[i].art) {
                pii p = q.top();
                q.pop();
                if(chart.size() == 0 || chart.back().se != v[p.se].in) chart.push_back({tm, v[p.se].in});
                if(ans[v[p.se].in].st == -1) {
                    ans[v[p.se].in].st = tm;
                    ans[v[p.se].in].art = v[p.se].art;
                    ans[v[p.se].in].brt = v[p.se].brt;
                    ans[v[p.se].in].in = v[p.se].in;
                }
                v[p.se].brt--;
                tm++;
                if(v[p.se].brt == 0) {
                    ans[v[p.se].in].endt = tm;
                }
                else q.push({p.fi, p.se});
            }
            tm = max(tm, v[i].art);
            q.push({v[i].priority, i});
        }
        while(q.size()) {
            pii p = q.top();
            q.pop();
            if(chart.size() == 0 || chart.back().se != v[p.se].in) chart.push_back({tm, v[p.se].in});
            if(ans[v[p.se].in].st == -1) {
                ans[v[p.se].in].st = tm;
                ans[v[p.se].in].art = v[p.se].art;
                ans[v[p.se].in].brt = v[p.se].brt;
                ans[v[p.se].in].in = v[p.se].in;
            }
            tm = tm + v[p.se].brt;
            ans[v[p.se].in].endt = tm;
        }
    }
};
struct round_robin {
    int n, tq;
    vector<node> v, ini;
    vector<fnode> ans;
    vector<pii> chart;
    deque<node> od, arr;
    round_robin (int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n); ini.resize(n);
    }
    void process() {
        sort(v.begin(), v.end(), round_robin_cmp);
        int tot_time = 0;
        for(int i = 0; i < n; i++) {
            ini[i] = v[i];
            tot_time += v[i].brt;
            ans[i].st = -1;
        }
        for(int i = 0; i < n; i++) {
            arr.push_back(v[i]);
        }

        cout << "Time Quantum: ";
        cin >> tq;
        cout << endl;

        for(int i = 0; i < tot_time;) {
            while(arr.size() && arr.front().art <= i) {
                od.push_back(arr.front());
                arr.pop_front();
            }
            if(od.size() == 0) {
                ++i;
                continue;
            }
            node temp = od.front();
            if(temp.art > i) {
                i++;
                continue;
            }
            od.pop_front();
            if(temp.brt <= tq) {
                if(ans[temp.in].st == -1) {
                    ans[temp.in].st = i;
                    ans[temp.in].art = temp.art;
                    ans[temp.in].brt = temp.brt;
                    ans[temp.in].in = temp.in;
                }
                chart.push_back({i, temp.in});
                i= i + temp.brt;
                while(arr.size() && arr.front().art <= i) {
                    od.push_back(arr.front());
                    arr.pop_front();
                }
                ans[temp.in].endt = i;
            }
            else {
                if(ans[temp.in].st == -1) {
                    ans[temp.in].st = i;
                    ans[temp.in].art = temp.art;
                    ans[temp.in].brt = temp.brt;
                    ans[temp.in].in = temp.in;
                }
                chart.push_back({i, temp.in});
                node temp1;
                temp1.in = temp.in; temp1.art = temp.in; temp1.brt = temp.brt - tq;
                i = i + tq;
                while(arr.size() && arr.front().art <= i) {
                    od.push_back(arr.front());
                    arr.pop_front();
                }
                od.push_back(temp1);
                ans[temp.in].endt = i;
            }
        }
    }
};
struct own_algorithm {
    int n, tq;
    vector<node> v, ini;
    vector<fnode> ans;
    vector<pii> chart;
    deque<node> od, arr;
    own_algorithm (int n, vector<node> v) {
        this->n = n;
        this->v = v;
        ans.resize(n); ini.resize(n);
    }
    int find_tq() {
         vector<int> V;
         double sum = 0;
         for(auto it : v) {
            V.push_back(it.brt);
         }

         sort(V.begin(), V.end());
         if(V.size() < 4) {
            int sum = 0;
            for(auto it : V) sum += it;
            return sum / V.size();
         }
         else if(V.size() % 2) {
            int sum = V[0] + V.back() + V[V.size() / 2];
            return sum / 3;
         }
         else {
            int sum = V[0] + V.back() + V[V.size() / 2] + V[V.size() / 2 - 1];
            return sum / 4;
         }
    }
    void process() {
        sort(v.begin(), v.end(), own_algorithm_cmp);
        int tot_time = 0;
        for(int i = 0; i < n; i++) {
            ini[i] = v[i];
            tot_time += v[i].brt;
            ans[i].st = -1;
        }
        for(int i = 0; i < n; i++) {
            arr.push_back(v[i]);
        }

        tq = find_tq();

        for(int i = 0; i < tot_time;) {
            while(arr.size() && arr.front().art <= i) {
                od.push_back(arr.front());
                arr.pop_front();
            }
            if(od.size() == 0) {
                ++i;
                continue;
            }
            node temp = od.front();
            if(temp.art > i) {
                i++;
                continue;
            }
            od.pop_front();
            if(temp.brt <= tq) {
                if(ans[temp.in].st == -1) {
                    ans[temp.in].st = i;
                    ans[temp.in].art = temp.art;
                    ans[temp.in].brt = temp.brt;
                    ans[temp.in].in = temp.in;
                }
                chart.push_back({i, temp.in});
                i= i + temp.brt;
                while(arr.size() && arr.front().art <= i) {
                    od.push_back(arr.front());
                    arr.pop_front();
                }
                ans[temp.in].endt = i;
            }
            else {
                if(ans[temp.in].st == -1) {
                    ans[temp.in].st = i;
                    ans[temp.in].art = temp.art;
                    ans[temp.in].brt = temp.brt;
                    ans[temp.in].in = temp.in;
                }
                chart.push_back({i, temp.in});
                node temp1;
                temp1.in = temp.in; temp1.art = temp.in; temp1.brt = temp.brt - tq;
                i = i + tq;
                while(arr.size() && arr.front().art <= i) {
                    od.push_back(arr.front());
                    arr.pop_front();
                }
                od.push_back(temp1);
                ans[temp.in].endt = i;
            }
        }
    }
};

// routing part
void type1(int c) {
    int n;
    cout << "Number of process, n: ";
    cin >> n;
    vector<node> v(n);
    for(int i = 0; i < n; i++) {
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> v[i].brt;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> v[i].art;
        v[i].in = i;
        cout << endl;
    }
    if(c == 1) {
        fcfs temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
    else if(c == 2) {
        non_Preemptive_sjf temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
    else if(c == 3) {
        Preemptive_sjf temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
    else if(c == 6) {
        round_robin temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
    else {
        own_algorithm temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
}
void type2 (int c) {
    int n;
    cout << "Number of process, n: ";
    cin >> n;
    vector<node> v(n);
    for(int i = 0; i < n; i++) {
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> v[i].brt;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> v[i].art;
        cout << "Enter the priority of P" << i + 1 << ": ";
        cin >> v[i].priority;
        v[i].in = i;
        cout << endl;
    }
    if(c == 4) {
        non_Preemptive_priority temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
    else {
        Preemptive_priority temp(n, v);
        temp.process();
        print(temp.ans, temp.chart);
    }
}
void type3() {
    int n;
    cout << "Number of process, n: ";
    cin >> n;
    vector<node> v(n);
    for(int i = 0; i < n; i++) {
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> v[i].brt;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> v[i].art;
        cout << "Enter the priority of P" << i + 1 << ": ";
        cin >> v[i].priority;
        v[i].in = i;
        cout << endl;
    }

    fcfs temp1(n, v);
    non_Preemptive_sjf temp2(n, v);
    Preemptive_sjf temp3(n, v);
    non_Preemptive_priority temp4(n, v);
    Preemptive_priority temp5(n, v);
    round_robin temp6(n, v);
    own_algorithm temp7(n, v);

    temp1.process();
    temp2.process();
    temp3.process();
    temp4.process();
    temp5.process();
    temp6.process();
    temp7.process();

    cout << left << setw(30) << "Algo: FCFS " << "Avg Response T.: " << setw(5) << avg_rt(temp1.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp1.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp1.ans) << endl;
    cout << left << setw(30) << "Algo: Non-Preemptive-SJF " << "Avg Response T.: " << setw(5) << avg_rt(temp2.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp2.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp2.ans) << endl;
    cout << left << setw(30) << "Algo: Preemptive-SJF " << "Avg Response T.: " << setw(5) << avg_rt(temp3.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp3.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp3.ans) << endl;
    cout << left << setw(30) << "Algo: Non-Preemptive-Priority " << "Avg Response T.: " << setw(5) << avg_rt(temp4.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp4.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp4.ans) << endl;
    cout << left << setw(30) << "Algo: Preemptive-Priority " << "Avg Response T.: " << setw(5) << avg_rt(temp5.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp5.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp5.ans) << endl;
    cout << left << setw(30) << "Algo: Round-Robin " << "Avg Response T.: " << setw(5) << avg_rt(temp6.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp6.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp6.ans) << endl;
    cout << left << setw(30) << "Algo: Your-Own-Algorithm " << "Avg Response T.: " << setw(5) << avg_rt(temp7.ans) << " Avg Waiting T.: " << setw(5) << avg_wt(temp7.ans) << " Avg. Turnaround T: " << setw(5) << avg_tt(temp7.ans) << endl;


}
int main() {
    while(true) {
        cout << "1: FCFS" << endl;
        cout << "2: Non-Preemptive-SJF" << endl;
        cout << "3: Preemptive-SJF" << endl;
        cout << "4: Non-Preemptive-Priority" << endl;
        cout << "5: Preemptive-Priority" << endl;
        cout << "6: Round-Robin" << endl;
        cout << "7: Your-Own-Algorithm" << endl;
        cout << "8: Compare-All" << endl;
        cout << "9: Exit" << endl;
        cout << "Input your Choice: ";

        int c;
        cin >> c;
        cout << endl;
        if(c == 1 || c == 2 || c == 3 || c == 6 || c == 7) type1(c);
        else if(c == 4 || c == 5) type2(c);
        else if(c == 8) type3();
        else return 0;

        cout << endl;
    }

}

