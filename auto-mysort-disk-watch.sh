for num_threads in 4 8 16 24
do
	sort_command="./mysort 64GB.txt sort.txt $num_threads"
	log_name="./psrecord_analysis/mysort64GB-$num_threads-threads_log.txt"
	png_name="./psrecord_analysis/mysort64GB-$num_threads-threads_log.png"
	diskio_log_name="./psrecord_analysis/mysort64GB-$num_threads-threads_diskio_log.txt"

	pidstat -d 1 -e ./mysort 64GB.txt sort.txt $num_threads >> $diskio_log_name
	#pidstat -d 1 -e psrecord "./mysort 1GB.txt sort.txt $num_threads" --log $log_name --interval 1 --include-children --plot $png_name >> $diskio_log_name
	#pidstat -d 1 -e "psrecord \"$sort_command\" --log $log_name --interval 1 --include-children --plot $png_name" >> $diskio_log_name  
	echo "mysort 64GB $num_threads with diskio finished."
        #./valsort sort.txt >> mysort64GB.log
        rm -rf sort.txt
        find  . -name 'tmp*' -exec rm {} \;
done
