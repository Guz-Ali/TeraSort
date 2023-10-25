import matplotlib.pyplot as plt      
if __name__ == "__main__":
    data = []
    with open("./psrecord_analysis_arch/linux-sort64GB-24-threads_diskio_log.txt", "r") as file:
        lines = file.readlines()

    for line in lines[9:]:
        columns = line.split()
        time = columns[0]
        kb_rd = float(columns[3])
        kb_wr = float(columns[4])
        kb_ccwr = float(columns[5])

        data.append((kb_rd+kb_wr+kb_ccwr)//1000)

    x = range(len(data))

    plt.plot(x, data)
    plt.xlabel("Time (s)")
    plt.ylabel("Disk I/O (MB)")
    plt.title("Disk I/O by time")
    plt.show()