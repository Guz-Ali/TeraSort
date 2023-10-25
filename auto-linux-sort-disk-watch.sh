for num_threads in 16 24
do
        diskio_log_name="./psrecord_analysis/linux-sort-64GB-$num_threads-threads_diskio_log.txt"
        pidstat -d 1 -e sort -k 1 -S 8G 64GB.txt -o sort.txt --parallel=$num_threads >> $diskio_log_name
        echo "linux-sort 64GB $num_threads with diskio finished."
        rm -rf sort.txt
done
~     
