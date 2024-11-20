import matplotlib.pyplot as plt

class Process:
    def __init__(self, pid, arrival_time, burst_time):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.completion_time = 0
        self.turnaround_time = 0
        self.waiting_time = 0

def plot_gantt_chart(gantt_chart):
    fig, gnt = plt.subplots()
    
    gnt.set_xlabel('Time')
    gnt.set_ylabel('Processes')
    
    # Set x and y axis limits
    gnt.set_xlim(0, gantt_chart[-1][1])
    gnt.set_ylim(0, len(set([p[0] for p in gantt_chart])) + 1)
    
    gnt.set_yticks([i + 1 for i in range(len(set([p[0] for p in gantt_chart])))])
    gnt.set_yticklabels([f"P{p}" for p in sorted(set([p[0] for p in gantt_chart]))])
    
    for process, start, end in gantt_chart:
        gnt.broken_barh([(start, end - start)], (process, 0.9), facecolors='green')
    
    plt.title('Gantt Chart - SJF Preemptive Scheduling')
    plt.show()

def shortest_job_first_preemptive(processes):
    time = 0
    completed = 0
    n = len(processes)
    ready_queue = []
    gantt_chart = []

    while completed != n:
        # Add newly arrived processes to the ready queue
        for process in processes:
            if process.arrival_time == time:
                ready_queue.append(process)

        # Sort by shortest remaining time (SRTF), break ties with arrival time
        ready_queue.sort(key=lambda p: (p.remaining_time, p.arrival_time))

        if ready_queue:
            # Select process with the shortest remaining time
            current_process = ready_queue[0]

            # Gantt Chart records
            if gantt_chart and gantt_chart[-1][0] == current_process.pid:
                gantt_chart[-1] = (current_process.pid, gantt_chart[-1][1], time + 1)
            else:
                gantt_chart.append((current_process.pid, time, time + 1))

            # Execute the process for 1 time unit
            current_process.remaining_time -= 1

            # If the process is completed
            if current_process.remaining_time == 0:
                current_process.completion_time = time + 1
                current_process.turnaround_time = current_process.completion_time - current_process.arrival_time
                current_process.waiting_time = current_process.turnaround_time - current_process.burst_time
                ready_queue.pop(0)  # Remove from ready queue
                completed += 1

        time += 1

    # Print results
    print("PID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time")
    for process in processes:
        print(f"{process.pid}\t{process.arrival_time}\t\t{process.burst_time}\t\t{process.completion_time}\t\t{process.turnaround_time}\t\t{process.waiting_time}")
    
    # Plot Gantt Chart
    plot_gantt_chart(gantt_chart)

# Example usage
processes = [
    Process(1, 0, 8),
    Process(2, 1, 4),
    Process(3, 2, 9),
    Process(4, 3, 5),
]

shortest_job_first_preemptive(processes)
