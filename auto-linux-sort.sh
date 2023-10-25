#for num_threads in 1 4 8 16
#do 
#	psrecord "time sort -k 1 1GB.txt -o sort.txt --parallel=$num_threads >> linux-sort1GB.log" --log "./psrecord_analysis/linux-sort1GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/linux-sort1GB-$num_threads-threads_log.png"
#	#time sort -k 1 1gb.txt -o sort.txt --parallel=$num_threads >> linux-sort1GB.log
#	rm -rf sort.txt
#done
#
#for num_threads in 1 4 8
#do 
#	psrecord "time sort -k 1 4GB.txt -o sort.txt --parallel=$num_threads >> linux-sort4GB.log" --log "./psrecord_analysis/linux-sort4GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/linux-sort4GB-$num_threads-threads_log.png"
#	#time sort -k 1 4gb.txt -o sort.txt --parallel=$num_threads >> linux-sort4GB.log
#	rm -rf sort.txt
#done
for num_threads in 8 24
do 
	psrecord "time sort -k 1 -S 8G 16GB.txt -o sort.txt --parallel=$num_threads >> linux-sort16GB.log" --log "./psrecord_analysis/linux-sort16GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/linux-sort16GB-$num_threads-threads_log.png"
	#time sort -k 1 16gb.txt -o sort.txt --parallel=$num_threads >> linux-sort16GB.log
	rm -rf sort.txt
done
for num_threads in 16 24
do 
	psrecord "time sort -k 1 -S 8G 64GB.txt -o sort.txt --parallel=$num_threads >> linux-sort64GB.log" --log "./psrecord_analysis/linux-sort64GB-$num_threads-threads_log.txt" --interval 1 --include-children --plot "./psrecord_analysis/linux-sort64GB-$num_threads-threads_log.png"
	#time sort -k 1 64gb.txt -o sort.txt --parallel=$num_threads >> linux-sort64GB.log
	rm -rf sort.txt
done
