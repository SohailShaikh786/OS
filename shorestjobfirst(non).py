import matplotlib.pyplot as plt

class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.completion_time = 0
        self.turnaround_time = 0
        self.waiting_time = 0

# Function to plot Gantt Chart
def plot_gantt_chart(gantt_chart):
    fig, gnt = plt.subplots()

    # Setting labels for x and y axis
    gnt.set_xlabel('Time')
    gnt.set_ylabel('Processes')

    # Set x-axis limits
    gnt.set_xlim(0, gantt_chart[-1][2])

    # Set y-axis ticks
    gnt.set_yticks([i + 1 for i in range(len(set([p[0] for p in gantt_chart])))])
    gnt.set_yticklabels([f"P{p[0]}" for p in gantt_chart])

    # Plotting the processes as bars in the Gantt chart
    for process, start_time, end_time in gantt_chart:
        gnt.broken_barh([(start_time, end_time - start_time)], (process, 0.9), facecolors='green')

    # Set chart title
    plt.title('Gantt Chart - SJF Non-preemptive Scheduling')
    plt.show()

# Function to simulate SJF Non-preemptive Scheduling
def sjf_non_preemptive_scheduling(processes):
    # Sort processes by Arrival Time initially
    processes.sort(key=lambda x: x.arrival_time)
    
    completed_processes = []
    ready_queue = []
    current_time = 0
    gantt_chart = []

    while processes or ready_queue:
        # Move processes that have arrived by current_time to ready_queue
        ready_queue += [p for p in processes if p.arrival_time <= current_time]
        processes = [p for p in processes if p.arrival_time > current_time]

        # Sort ready queue by burst time (shortest job first)
        ready_queue.sort(key=lambda x: x.burst_time)

        if ready_queue:
            # Select the process with the shortest burst time
            current_process = ready_queue.pop(0)
            start_time = current_time
            current_time += current_process.burst_time
            end_time = current_time

            # Update process details
            current_process.completion_time = end_time
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time
            completed_processes.append(current_process)

            # Add to Gantt chart data
            gantt_chart.append((int(current_process.pid[1:]), start_time, end_time))  # Process ID without 'P'
        else:
            # If no process in ready queue, advance time to the next arrival
            current_time = processes[0].arrival_time

    # Print the results
    print("PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting")
    for process in completed_processes:
        print(f"{process.pid}\t{process.arrival_time}\t{process.burst_time}\t{process.completion_time}\t{process.turnaround_time}\t{process.waiting_time}")

    # Calculate average turnaround time and waiting time
    total_turnaround_time = sum([p.turnaround_time for p in completed_processes])
    total_waiting_time = sum([p.waiting_time for p in completed_processes])
    
    print(f"\nAverage Turnaround Time: {total_turnaround_time / len(completed_processes):.2f}")
    print(f"Average Waiting Time: {total_waiting_time / len(completed_processes):.2f}")

    # Plot Gantt chart
    plot_gantt_chart(gantt_chart)

# Input processes for SJF Non-preemptive
process_list = [
    Process("P1", 0, 8),
    Process("P2", 1, 4),
    Process("P3", 2, 9),
    Process("P4", 3, 5)
]

# Run SJF Non-preemptive simulation
sjf_non_preemptive_scheduling(process_list)
