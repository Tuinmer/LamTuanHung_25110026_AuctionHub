1/Algorithms and Real-World Justification

Insertion Sort (Live Auction Monitor): It is very efficient for nearly-sorted data, reaching O(N) complexity. This is perfect for a live stream where each new bid is only slightly higher than the previous one, so the inner loop only shifts a few positions.

Selection Sort (Top-K Bid Finder): It quickly extracts the largest elements after only a few passes without needing extra memory. Since we only need the Top 3 bids, we can look at the first few passes, making it technically justified for this small set.

Interchange Sort (Price Anomaly Validator): The logic is very simple, and it is easy to add a swap counter for QA testing on small data batches (under 50 records). For this scale, the O(N^2) complexity is irrelevant, so simplicity is best.

Bubble Sort Early Stop (Stabilization Detector): It stops immediately on the first pass if there are no swaps. This makes it extremely fast to detect if a price stream has stabilized.

Quick Sort (Full Historical Sorter): It has O(N log N) average complexity, is fast, and sorts in-place. It is the optimal choice for sorting millions of end-of-day records without using too much memory.

2/Quick Sort Runtime on 100k Records

I tested Quick Sort with 100,000 random bids using the median-of-three pivot and Lomuto partition. The measured runtime is 41 ms. (Screenshot attached).

3/Bug Encountered and Fix

When I initially wrote the insertionSort function, I incorrectly wrote the while loop condition. I originally wrote:

while (index >= 0 &&(temp.amount > v[index].amount || (temp.amount == v[index].amount && temp.timestamp > v[index].timestamp)))

When I tested my insertionSort function with the small.txt, the output was in descending order: [dave] $103, [bob] $102.5, [carol] $101, [alice] $100. So I traced back to the code of insertion sort and looked back at the lesson, realized the loop was wrong, and changed it to while (index >= 0 &&(temp.amount < v[index].amount || (temp.amount == v[index].amount && temp.timestamp < v[index].timestamp)))

4/LLM Prompts Used 

[Green] "why is insertion sort efficient for nearly-sorted data"

[Green] "Interchange and selection sort what the difference?"

[Green] "What is a quicksort wrapper and what its usage"

5/Worst-Case Complexity of Quick Sort (Median-of-Three)

The worst-case complexity of Quick Sort with a median-of-three pivot is still O(n²). Because it is not guaranteed that the median-of-three strategy will always eliminate the O(n^2) worst-case scenario for every input. There is a theoretical worst-case scenario in which a quicksort function with the median-of-three strategy remains O(n^2). This is the case when a[low], a[mid], and a[high] are the maximum value or the minimum value of that specific array. When this happens, i+1 will be placed at the far right or the far left edge, creating 2 highly unbalanced subarrays with sizes n-1 and 0, and when recursing through the remaining array, the same thing will occur again and again, splitting the array into two 2 highly unbalanced subarrays, resulting in the time complexity of O(n^2).