import heapq
import matplotlib.pyplot as plt

def priority_scheduling(processes, burst_time, arrival_time, priorities):
    """Simulates priority scheduling (non-preemptive).

    Args:
        processes: A list of process names.
        burst_time: A list of burst times for each process.
        arrival_time: A list of arrival times for each process.
        priorities: A list of priorities for each process (higher priority is a lower number).

    Returns:
        A tuple containing the waiting time, turnaround time, and Gantt chart data for each process.
    """

    n = len(processes)
    waiting_time = [0] * n
    turnaround_time = [0] * n
    remaining_time = burst_time[:]  # Track remaining burst time for each process
    time = 0  # Current time in the system
    queue = []  # Ready queue, implemented as a min-heap based on priority
    gantt_chart_data = []  # Store data for the Gantt chart

    # Initialize the ready queue with processes that have arrived at time 0
    for i in range(n):
        if arrival_time[i] == 0:
            heapq.heappush(queue, (priorities[i], i))

    while len(queue) > 0:
        current_process = heapq.heappop(queue)[1]  # Get the process with the highest priority
        if remaining_time[current_process] > 0:
            start_time = time
            time += remaining_time[current_process]
            remaining_time[current_process] = 0
            turnaround_time[current_process] = time - arrival_time[current_process]
            waiting_time[current_process] = turnaround_time[current_process] - burst_time[current_process]
            gantt_chart_data.append((current_process, start_time, time))

        # Add processes that have arrived while the current process was executing
        for i in range(n):
            if arrival_time[i] <= time and remaining_time[i] > 0:
                heapq.heappush(queue, (priorities[i], i))

    return waiting_time, turnaround_time, gantt_chart_data

# Example inputs
processes = ['P1', 'P2', 'P3']
burst_time = [5, 3, 4]
arrival_time = [0, 1, 2]
priorities = [2, 1, 3]  # Higher priority is a lower number

waiting_time, turnaround_time, gantt_chart_data = priority_scheduling(processes, burst_time, arrival_time, priorities)

# Print results
for i in range(len(processes)):
    print(f"Process {processes[i]}: Waiting Time = {waiting_time[i]}, Turnaround Time = {turnaround_time[i]}")

# Create Gantt chart
fig, ax = plt.subplots()
ax.set_xlabel('Time')
ax.set_ylabel('Process')
ax.set_title('Priority Scheduling Gantt Chart')

for process, start_time, end_time in gantt_chart_data:
    ax.barh(process, end_time - start_time, left=start_time, color='blue')

plt.yticks(range(len(processes)), processes)
plt.grid(True)
plt.show()
