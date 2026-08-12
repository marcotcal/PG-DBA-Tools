#!/bin/bash
read -r cpu user nice system idle iowait irq softirq rest < /proc/stat
total1=$((user + nice + system + idle + iowait + irq + softirq))
idle1=$idle

sleep 1

read -r cpu user nice system idle iowait irq softirq rest < /proc/stat
total2=$((user + nice + system + idle + iowait + irq + softirq))
idle2=$idle

# calculate difference between 2 times
diff_total=$((total2 - total1))
diff_idle=$((idle2 - idle1))
cpu_uso=$(( (100 * (diff_total - diff_idle)) / diff_total ))

echo "Uso da CPU (último segundo): ${cpu_uso}%"

