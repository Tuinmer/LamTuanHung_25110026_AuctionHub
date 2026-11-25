#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;       // bid price
    long long timestamp; // Unix ms
    int auctionId;
};

// ---------- Utilities ----------
void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}
void generateBidsToFile(const string& filepath, int numRecords);

// ---------- TODO: Implement these ----------
void insertionSort(vector<Bid>& v);          // Task A
void selectionSort(vector<Bid>& v);          // Task B
void interchangeSort(vector<Bid>& v);        // Task C
bool bubbleSortEarlyStop(vector<Bid>& v);    // Task D — returns true if stable (no swaps on a full pass)
void quickSort(vector<Bid>& v, int lo, int hi); // Task E
int partition(vector<Bid>& v, int low, int high);
void quickSortWrapper(vector<Bid>& v);
bool compareBid(const Bid& a, const Bid& b);

// ---------- Driver ----------
int main(int argc, char* argv[]) {
    // See per-task instructions.

    string smallFile = "test_data/small.txt";
    string largeFile = "test_data/large.txt";
    vector<Bid> bids;
    //use LLM to generate some of the test cases
    cout << "Task A: Insertion Sort \n";
    loadBids(smallFile, bids);
    insertionSort(bids);
    printBids(bids, 4); 
    bids.clear();

    cout << "\nTask B: Selection Sort \n";
    loadBids(smallFile, bids);
    selectionSort(bids);
    for (size_t i = 0; i < min(3, (int)bids.size()); ++i)
        cout << "[" << bids[i].bidderId << "] $" << bids[i].amount
             << " @ " << bids[i].timestamp << "\n";
    bids.clear();

    cout << "\nTask c: Interchange Sort\n";
    generateBidsToFile(largeFile, 50);
    loadBids(largeFile, bids);
    interchangeSort(bids); 
    printBids(bids, 4);
    bids.clear();

    cout << "\nTask D: Bubble Sort: \n";
    vector<Bid> sortedBids = {
        {"alice", 100.0, 1000, 1},
        {"bob",   101.0, 1005, 1},
        {"carol", 102.5, 1003, 1}
    };
    cout << "Test Case 1 (Already Sorted): ";
    bubbleSortEarlyStop(sortedBids); 
    vector<Bid> reversedBids = {
        {"carol", 102.5, 1003, 1},
        {"bob",   101.0, 1005, 1},
        {"alice", 100.0, 1000, 1}
    };
    cout << "Test Case 2 (Reversed): ";
    bubbleSortEarlyStop(reversedBids);

    cout << "\nTask E: Quick Sort\n";
    generateBidsToFile(largeFile, 100000);
    loadBids(largeFile, bids);
    quickSortWrapper(bids);
    printBids(bids, 5);
    return 0;
}
void insertionSort(vector<Bid>& v)
{
    for (int i=1;i<v.size();i++)
    {
        int index = i-1;
        Bid temp = v[i];
        while (index >= 0 &&(temp.amount < v[index].amount || (temp.amount == v[index].amount && temp.timestamp < v[index].timestamp)))
        {
            v[index+1] = v[index];
            index--;
        }
        v[index+1] = temp;
    }
}

void selectionSort(vector<Bid>& v)
{
    for (int i=0;i<v.size();i++)
    {
        double maxValue = v[i].amount;
        int index = i;
        for (int j=i+1;j<v.size();j++)
        {
            if (v[j].amount > v[index].amount || (v[j].amount == v[index].amount && v[j].timestamp < v[index].timestamp))
            {
                maxValue = v[j].amount;
                index=j;
            }
        }
        if (index != i)
        {
            Bid temp = v[i];
            v[i] = v[index];
            v[index] = temp;
        }
    }
}

//used llm to do
void generateBidsToFile(const string& filepath, int numRecords) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist_amount(100.0, 10000.0);
    uniform_int_distribution<long long> dist_time(1600000000000LL, 1700000000000LL);
    uniform_int_distribution<int> dist_auction(1, 100);
    uniform_int_distribution<int> dist_user(1, 1000);

    ofstream outFile(filepath);
    if (!outFile) {
        cerr << "Error: Could not open " << filepath << " for writing.\n";
        return;
    }

    for (int i = 0; i < numRecords; ++i) {
        outFile << "user_" << dist_user(gen) << " "
                << dist_amount(gen) << " "
                << dist_time(gen) << " "
                << dist_auction(gen) << "\n";
    }
    outFile.close();
    cout << "Successfully generated " << numRecords << " records in " << filepath << "\n";
}

void interchangeSort(vector<Bid>& v)
{
    int swapCount = 0;
    for (int i=0;i<v.size();i++)
    {
        for (int j=i+1;j<v.size();j++)
        {
            if (v[i].amount > v[j].amount ||(v[i].amount == v[j].amount && v[i].timestamp > v[j].timestamp))
            {
                swapCount++;
                Bid temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
    cout << "Total swaps: " << swapCount << endl;
}

bool bubbleSortEarlyStop(vector<Bid>& v)
{
    bool firstPass = false;
    for (int i=0;i<v.size();i++)
    {
        bool flag = false;
        for (int j=0;j<v.size()-1-i;j++)
        {
            if (v[j].amount > v[j+1].amount ||(v[j].amount == v[j+1].amount && v[j].timestamp > v[j+1].timestamp))
            {
                Bid temp = v[j];
                v[j] = v[j+1];
                v[j+1] = temp;
                flag = true;
            } 
        }
        if (i==0 && !flag)
        {
            firstPass = true;
        }
        if (!flag) break;
    }
    if (firstPass) {
        cout << "STABLE\n";
        return true;
    } else {
        cout << "UNSTABLE\n";
        return false;
    }
}
bool compareBid(const Bid& a, const Bid& b)
{
    if (a.amount != b.amount)
        return a.amount < b.amount;
    else return a.timestamp < b.timestamp;
}
void medianOfThree(vector<Bid>& v, int low, int high)
{
    int mid = (high - low)/2 +low;
    if (compareBid(v[mid], v[low]))
    {
        Bid temp = v[low];
        v[low] = v[mid];
        v[mid] = temp;
    }
    if (compareBid(v[high], v[mid]))
    {
        Bid temp = v[mid];
        v[mid] = v[high];
        v[high] = temp;
    }
    if (compareBid(v[mid], v[low]))
    {
        Bid temp = v[low];
        v[low] = v[mid];
        v[mid] = temp;
    }
    Bid temp = v[mid];
    v[mid] = v[high];
    v[high] = temp;
}
int partition(vector<Bid>& v, int low, int high)
{
    medianOfThree(v,low,high);
    int i=low-1;
    Bid pivot = v[high];
    for (int j=low;j<high;j++)
    {
        if (compareBid(v[j], pivot))
        {
            i++;
            Bid temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
    Bid temp = v[high];
    v[high] = v[i+1];
    v[i+1] = temp; 
    return i+1;
}
void quickSort(vector<Bid>& v, int low, int high)
{
    if (low<high)
    {
        int pi = partition(v,low,high);
        quickSort(v,low,pi-1);
        quickSort(v,pi+1,high);
    }
}

void quickSortWrapper(vector<Bid>& v) {
    if (v.empty()) return;
    auto start = high_resolution_clock::now();
    quickSort(v, 0, (int)v.size() - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "QuickSort Runtime: " << duration.count() << " ms\n";
}
