import pandas as pd
import matplotlib.pyplot as plt

# 从CSV文件读取性能指标数据
metrics_df = pd.read_csv('cpu_metrics.csv')

algorithms = metrics_df['Algorithm']
throughput = metrics_df['Job Throughput']
cpu_utilization = metrics_df['CPU Utilization']
avg_turnaround = metrics_df['Average Turnaround Time']
avg_response = metrics_df['Average Response Time']
avg_waiting = metrics_df['Average Waiting Time']

plt.figure(figsize=(14, 8))

# 作业吞吐量
plt.subplot(2, 3, 1)
plt.bar(algorithms, throughput, color='skyblue')
plt.title('Job Throughput')
plt.ylabel('Throughput')
plt.xticks(rotation=30)

# CPU利用率
plt.subplot(2, 3, 2)
plt.bar(algorithms, cpu_utilization, color='lightgreen')
plt.title('CPU Utilization')
plt.ylabel('%')
plt.xticks(rotation=30)

# 平均周转时间
plt.subplot(2, 3, 3)
plt.bar(algorithms, avg_turnaround, color='salmon')
plt.title('Average Turnaround Time')
plt.ylabel('Time')
plt.xticks(rotation=30)

# 平均响应时间
plt.subplot(2, 3, 4)
plt.bar(algorithms, avg_response, color='plum')
plt.title('Average Response Time')
plt.ylabel('Time')
plt.xticks(rotation=30)

# 平均等待时间
plt.subplot(2, 3, 5)
plt.bar(algorithms, avg_waiting, color='khaki')
plt.title('Average Waiting Time')
plt.ylabel('Time')
plt.xticks(rotation=30)

plt.tight_layout()
plt.show()

