# Modified version of the Round Robin function with Gantt chart generation

import matplotlib.pyplot as plt

def round_robin_gantt(processes, burst_time, arrival_time, time_quantum):
    n = len(processes)
    waiting_time = [0] * n
    turnaround_time = [0] * n
    remaining_time = burst_time[:]  # Track remaining burst time for each process
    time = 0  # Current time in the system
    queue = []  # Ready queue
    completed_processes = 0
    execution_sequence = []  # To store execution order for Gantt chart

    while completed_processes < n:
        for i in range(n):
            if arrival_time[i] <= time and remaining_time[i] > 0 and i not in queue:
                queue.append(i)
        
        if len(queue) > 0:
            current_process = queue.pop(0)
            if remaining_time[current_process] > time_quantum:
                execution_sequence.append((processes[current_process], time, time + time_quantum))
                time += time_quantum
                remaining_time[current_process] -= time_quantum
            else:
                execution_sequence.append((processes[current_process], time, time + remaining_time[current_process]))
                time += remaining_time[current_process]
                remaining_time[current_process] = 0
                turnaround_time[current_process] = time - arrival_time[current_process]
                waiting_time[current_process] = turnaround_time[current_process] - burst_time[current_process]
                completed_processes += 1
        else:
            time += 1  # Increment time when no process is ready

    return execution_sequence, waiting_time, turnaround_time

# Example inputs
processes = ['P1', 'P2', 'P3']
burst_time = [5, 3, 4]
arrival_time = [0, 1, 2]
time_quantum = 2

# Generate Gantt Chart Data and waiting/turnaround times
execution_sequence, waiting_time, turnaround_time = round_robin_gantt(processes, burst_time, arrival_time, time_quantum)

# Plotting the Gantt Chart using Matplotlib
fig, ax = plt.subplots()

for p, start, end in execution_sequence:
    ax.broken_barh([(start, end - start)], (processes.index(p) * 10, 9), facecolors=('green'))

# Formatting the plot
ax.set_ylim(0, len(processes) * 10)
ax.set_xlim(0, max([end for _, _, end in execution_sequence]))
ax.set_xlabel('Time')
ax.set_ylabel('Processes')
ax.set_yticks([i * 10 + 5 for i in range(len(processes))])
ax.set_yticklabels(processes)
ax.grid(True)

plt.title("Gantt Chart - Round Robin Scheduling")
plt.show()

# Print results (Waiting and Turnaround times)
for i in range(len(processes)):
    print(f"Process {processes[i]}: Waiting Time = {waiting_time[i]}, Turnaround Time = {turnaround_time[i]}")
