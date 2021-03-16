
# Assignment 3 Report
### Yunfan Yang 30067857

## Q2 - Written answer

### A. Make a table with these timings, and a bar graph

| Threads      | Timings (s) |
|--------------|-------------|
| 1 (Original) | 4.275       |
| 1            | 4.802       |
| 2            | 2.522       |
| 3            | 1.879       |
| 4            | 1.793       |
| 6            | 1.587       |
| 8            | 1.385       |
| 12           | 1.598       |
| 16           | 1.502       |
| 24           | 1.388       |
| 32           | 1.475       |

![q2-chart.png](https://media.discordapp.net/attachments/821581662839767081/821581718410362940/q2-chart.png?width=688&height=413)

### B. When you run your implementation with N threads, you should see N-times speed up compared to the original single threaded program. Do you observe this in your timings for all values of N?

No. It starts with 2x and then slowly become lower and lower.

### C. Why do you stop seeing the speed up after some value of N?

Swiching threads takes time for CPU, as there are more threads, each threads will get less workload, when the workload is too small, it takes more time because CPU is frequently switching threads, and it basically cancels out the benefit by having multiple threads. 

## Q4 - Written question 

| # Threads | Time Observed | Speed Up |
|-----------|---------------|----------|
| Original  | 45.8739s      | 1.0      |
| 1         | 47.7243s      | 0.9612   |
| 2         | 24.3292s      | 1.8855   |
| 3         | 16.5678s      | 2.7688   |
| 4         | 13.6956s      | 3.3495   |
| 8         | 13.1108s      | 3.4989   |
| 16        | 13.0690s      | 3.5105   |

### Are the timings what you expected them to be? If not, explain why they differ.

The timing is not as expected, the speeding-up gets levelling off. As mentioned before, there are more threads than actually needed, there is a balance between thread amount and workload, only falling into the balance will get the optimized time.