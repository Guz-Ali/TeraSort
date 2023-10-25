## CS 553 Cloud Computing PA3

### Team members:
 - Ali Guzelyel, A20454373
 - Oladapo Ogunnaike, A20435197


### Internal and External Sorting (TeraSort)

In this practice, we are tackling situations when the data does not fit into the memory. 

#### Internal Sort:

This is when the sorting algorithms can be achieved inside memory, since the data is less than the memory available.

### External Sort:

This is when the sorting algorithms cannot be achieved inside the memory, since the data is much larger than the memory available. (ex. 8 gb memory, 64 gb data). 

### What we did

We established the right algorithms to turn internal quicksort into an external quicksort algorithm. By doing so, we were able to work on data that were larger than the memory available. We are actually running the test cases with a server node that has 128 GB memory, but to simulate '8 GB mem vs 64 GB data', we are limiting the memory usage to 8 GB. 

In addition, we are also analyzing the cpu and memory usage, and disk i/o by comparing it to linux-sort results.
 
### Files
 - mysort.c
 - auto-gensort.sh
 - Makefile
 - README.md


### How to install & requirements
 - The folder comes with gensort and valsort for Linux. If you need to see more information on these, please visit: http://www.ordinal.com/gensort.html
 - For CPU and memory utilization analsis, check out 'psrecord' this page: https://github.com/astrofrog/psrecord . You might need to install some packages to do a in-depth analysis.
 - Since we are testing external sort with large files, large storage capacity is needed. (min 128 GB to test out sorting 64GB data)
 - Other required packages: psrecord, python-matplotlib, python-tk, sysstat

### How to run code
 - First do makefile ```make```
 - Then generate the data (ascii 1GB, 4GB, 16GB, 64GB) by running ```sh auto-gensort.sh```
 - You can automate the tests by running ```sh auto-linux-sort.sh``` for linux sort or ```auto-mysort.sh``` for mysort (our algorithm). The second one checks for accuracy with _valsort_ after sorting. Take a look at the log files to see the results. In addition, look at psrecord_analysis folder to see other results.
 - To run only one sorting command, run ```./mysort input_file output_file num_threads```
 - To use linux sort and time it, run ```time sort input_file -o output_file --parallel=num_threads```
 - To check accuracy of the sorting algorithms, run ```./valsort input_file```
 - To run any of the code in the background add '&' to the end, run ```command args &```


### Notes
 - Running the valsort command on linux sort does not give accurate results. However, we can just assume that the linux sort is OK. The job here is to test if mysort is successful in sorting.
 - While running the tests, Chameleon instance server went down, so we lost the data. So while re-running the tests, we redacted the tests that took too much time.

### Resources Used

#### Libraries
 - stdio.h
 - stdlib.h
 - sys/time.h
 - sys/stat.h
 - unistd.h
 - pthread.h
 - math.h
 - stdbool.h
 - string.h

#### External Sort
 - Definition https://en.wikipedia.org/wiki/External_sorting

#### gensort / valsort
 - Gensort and Valsort Documentation http://www.ordinal.com/gensort.html

#### linux sort
 -  Linux Sort Documentation http://man7.org/linux/man-pages/man1/sort.1.html

#### psrecord
 - Psrecord Documentation https://pypi.org/project/psrecord/
 - Installing psrecord on Linux https://unix.stackexchange.com/questions/554/how-to-monitor-cpu-memory-usage-of-a-single-process
 - Hotfix for pip install package_name, (including path to $PATH) https://askubuntu.com/questions/900312/pip-install-saws-command-not-found

#### nohup
 - Since we have a task that is going to run for a long while, it's better to take some precaution (learned the hard way) so that the tasks complete without errors, like ssh timeout, broken pipe etc. To run command in the background with nohup, run ```nohup <command> &```
 - nohup Documentation https://www.digitalocean.com/community/tutorials/nohup-command-in-linux
 - Further resource on independent tasks https://superuser.com/questions/513496/how-can-i-run-a-command-from-the-terminal-without-blocking-it

#### Chameleon Lease, Instance, Setup, Connect
 - Chameleon Documentation https://chameleoncloud.readthedocs.io/en/latest/technical/reservations.html
 - Chameleon Hardware https://chameleoncloud.org/hardware/
 - Further Chameleon Tutorial https://github.com/grc-iit/scs-tutorial/wiki/8.-Chameleon-Cloud
 - Connecting to Github from remote server https://stackoverflow.com/questions/68775869/message-support-for-password-authentication-was-removed

#### Additional Coding tips
 - To remove files starting with "_name_", run ```find  . -name 'name*' -exec rm {} \;``` https://superuser.com/questions/482435/how-to-remove-all-files-starting-with-a-certain-string-in-linux
