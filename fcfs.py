import matplotlib.pyplot as plt

# Assuming you have a list of processes with pid and burst time
processes = [
    {"pid": "P1", "burst_time": 6},
    {"pid": "P2", "burst_time": 22},
    {"pid": "P3", "burst_time": 19},
    {"pid": "P4", "burst_time": 13},
]

def gantt_chart(processes):
    fig, ax = plt.subplots()
    ax.set_ylim(0, len(processes) + 1)
    ax.set_xlim(0, sum(p["burst_time"] for p in processes))
    ax.set_yticks(range(len(processes)))
    ax.set_yticklabels([p["pid"] for p in processes])
    ax.set_xlabel("Time")
    ax.set_ylabel("Process")

    current_time = 0
    for i, p in enumerate(processes):
        ax.barh(i, p["burst_time"], left=current_time, align='center')
        current_time += p["burst_time"]

    plt.show()

def calculate_turnaround_time(processes):
    turnaround_times = []
    current_time = 0
    for p in processes:
        turnaround_times.append(current_time + p["burst_time"])
        current_time += p["burst_time"]
    return turnaround_times

def calculate_waiting_time(processes):
    waiting_times = [0]
    for i in range(1, len(processes)):
        waiting_times.append(waiting_times[i-1] + processes[i-1]["burst_time"])
    return waiting_times

turnaround_times = calculate_turnaround_time(processes)
waiting_times = calculate_waiting_time(processes)

print("Turnaround Times:", turnaround_times)
print("Waiting Times:", waiting_times)

average_turnaround_time = sum(turnaround_times) / len(turnaround_times)
average_waiting_time = sum(waiting_times) / len(waiting_times)

print("Average Turnaround Time:", average_turnaround_time)
print("Average Waiting Time:", average_waiting_time)

gantt_chart(processes)