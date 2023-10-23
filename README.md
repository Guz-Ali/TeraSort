## CS 553 Cloud Computing PA 3

### Team members:
 - Ali Guzelyel, A20454373
 - Oladapo Ogunnaike, A20435197


### Internal and External Sorting (TeraSort)

In this practice, we are tackling situations when the data does not fit into the memory. 

#### Internal Sort:

This is when the sorting algorithms can be achieved inside memory, since the data is less than the memory available.

### External Sort:

This is when the sorting algorithms cannot be achieved inside the memory, since the data is much larger than the memory available. (ex. 8 gb memory, 64 gb data)

### What we did

We established the right algorithms to turn internal quicksort into an external quicksort algorithm. By doing so, we were able to work on data that were larger than the memory available.
 
### Files
 - mysort.c
 - auto-gensort.sh
 - Makefile
 - README.md


### How to install & requirements
 - The folder comes with gensort and valsort for Linux. If you need to see more information on these, please visit: http://www.ordinal.com/gensort.html
 - For CPU and memory utilization analsis, check out 'psrecord' this page: https://github.com/astrofrog/psrecord . You might need to install some packages to do a in-depth analysis.
 - Since we are testing external sort with large files, large storage capacity is needed. (min 128 GB to test out sorting 64GB data)

### How to run code
 - First do makefile ```make```
 - Then generate the data (ascii 1GB, 4GB, 16GB, 64GB) by running ```sh auto-gensort.sh```
 - You can automate the tests by running ```sh auto-linux-sort.sh``` for linux sort or ```auto-mysort.sh``` for mysort (our algorithm). The second one checks for accuracy with _valsort_ after sorting. Take a look at the log files to see the results. In addition, look at psrecord_analysis folder to see other results.
 - To run only one sorting command, run ```./mysort input_file output_file num_threads```
 - To use linux sort and time it, run ```time sort input_file -o output_file --parallel=num_threads```
 - To check accuracy of the sorting algorithms, run ```./valsort input_file```


### Notes
 - Running the valsort command on linux sort does not give accurate results. However, we can just assume that the linux sort is OK. The 