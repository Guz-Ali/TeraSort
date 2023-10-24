for num_threads in 1 4 8 16
do
	psrecord "./mysort 1GB.txt sort.txt $num_threads >> mysort1GB.log" --log "./psrecord_analysis/mysort1GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort1GB-$num_threads-threads_log.png"
	echo "mysort 1GB $num_threads finished."
	#./valsort sort.txt >> mysort1GB.log
	rm -rf sort.txt
	find  . -name 'tmp*' -exec rm {} \;
done

for num_threads in 1 4 8 16
do
	psrecord "./mysort 4GB.txt sort.txt $num_threads >> mysort4GB.log" --log "./psrecord_analysis/mysort4GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort4GB-$num_threads-threads_log.png"
	echo "mysort 4GB $num_threads finished."
	#./valsort sort.txt >> mysort4GB.log
	rm -rf sort.txt
	find  . -name 'tmp*' -exec rm {} \;
done

for num_threads in 4 8 24
do
	psrecord "./mysort 16GB.txt sort.txt $num_threads >> mysort16GB.log" --log "./psrecord_analysis/mysort16GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort16GB-$num_threads-threads_log.png"
	echo "mysort 16GB $num_threads finished."
	#./valsort sort.txt >> mysort16GB.log
	rm -rf sort.txt
	find  . -name 'tmp*' -exec rm {} \;
done

for num_threads in 4 8 16 24
do
	psrecord "./mysort 64GB.txt sort.txt $num_threads >> mysort64GB.log" --log "./psrecord_analysis/mysort64GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort64GB-$num_threads-threads_log.png"
	echo "mysort 64GB $num_threads finished."
	#./valsort sort.txt >> mysort64GB.log
	rm -rf sort.txt
	find  . -name 'tmp*' -exec rm {} \;
done
