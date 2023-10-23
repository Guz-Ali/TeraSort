for num_threads in 1 4 8 16
do
	#psrecord "./mysort 1GB.txt sort.txt $num_threads >> mysort1GB.log" --log "mysort1GB-CPU-MEM.txt" --interval 1
	psrecord "./mysort 1gb.txt sort.txt $num_threads >> mysort1GB.log" --log "./psrecord_analysis/mysort1GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort1GB-$num_threads-threads_log.png"
	./valsort sort.txt >> mysort1GB.log
	rm -rf sort.txt
done

for num_threads in 1 4 8 16
do
	psrecord "./mysort 4gb.txt sort.txt $num_threads >> mysort4GB.log" --log "./psrecord_analysis/mysort4GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort4GB-$num_threads-threads_log.png"
	#./mysort 4GB.txt sort.txt $num_threads >> mysort4GB.log
	./valsort sort.txt >> mysort4GB.log
	rm -rf sort.txt
done

for num_threads in 1 4 8 16
do
	psrecord "./mysort 16gb.txt sort.txt $num_threads >> mysort16GB.log" --log "./psrecord_analysis/mysort16GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort16GB-$num_threads-threads_log.png"
	#./mysort 16GB.txt sort.txt $num_threads >> mysort16GB.log
	./valsort sort.txt >> mysort16GB.log
	rm -rf sort.txt
done

for num_threads in 1 4 8 16
do
	psrecord "./mysort 64gb.txt sort.txt $num_threads >> mysort64GB.log" --log "./psrecord_analysis/mysort64GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/mysort64GB-$num_threads-threads_log.png"
	#./mysort 64GB.txt sort.txt $num_threads >> mysort64GB.log
	./valsort sort.txt >> mysort64GB.log
	rm -rf sort.txt
done
