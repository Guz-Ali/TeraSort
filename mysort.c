#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MSG "./mysort <file input> <file output> <num_threads>"
#define MEMORY_LIMIT 8000000000
#define BUFFER_SIZE 100

typedef struct thread_task {
	void *(*run_thread)(void *);
	void *args;
	struct thread_task* next;
} thread_task_t;

typedef struct {
	bool is_close;
	int num_threads;
	pthread_t check_thread;
	pthread_mutex_t lock;
	pthread_t* all_ids;
	pthread_cond_t ready;
	thread_task_t* head;
	thread_task_t* tail;
	
} thread_pool_t;

char* temp_file_prefix = "tmp_";

// used for checking the file size, determine internal / external
long check_file_size(char* file) {
	struct stat sbuffer;
	stat(file, &sbuffer);
	return (long)sbuffer.st_size;
}

// opens an output file
FILE* open_file_to_write(char* file) {
	FILE* f;
	f = fopen(file, "w");
	if (f == NULL){
		fprintf(stderr, "open to write [%s] failed", file);
	}
	
	return f;
}

// opens an input file
FILE* open_file_to_read(char* file) {
	FILE* f;
	f = fopen(file, "r");
	if (f == NULL){
		fprintf(stderr, "open to read [%s] failed",file);
	}
	
	return f;
}

// run multi-thread tasks in the thread pool with locks, and waits
void* run_thread(void* args){
	thread_pool_t* thread_pool = (thread_pool_t*) args;
	thread_task_t* task = NULL;
	
	while(true){
		pthread_mutex_lock(&thread_pool->lock);
		//wait until thread finishes
		while(!thread_pool->head && !thread_pool->is_close) {
			pthread_cond_wait(&thread_pool->ready, &thread_pool->lock);
		}
		// pool closed, exit
		if (thread_pool->is_close) {
			pthread_mutex_unlock(&thread_pool->lock);
			pthread_exit(NULL);
		}
		
		//thread finished, switch to next (pool not closed yet)
		task = thread_pool->head;
		thread_pool->head = thread_pool->head->next;
		pthread_mutex_unlock(&thread_pool->lock);
		task->run_thread(task->args);
		free(task);
	}
	
	return NULL;
}

// create a thread pool and allocate space
int start_thread_pool(thread_pool_t** pool, int num_threads) {
	thread_pool_t* thread_pool = (thread_pool_t *) malloc(sizeof(thread_pool_t));
	thread_pool->is_close = false;
	thread_pool->num_threads = num_threads;
	thread_pool->all_ids = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
	thread_pool->head = NULL;
	
	//create lock and ready attr
	if (pthread_mutex_init(&(thread_pool->lock), NULL) != 0)
		return -1;
	if (pthread_cond_init(&(thread_pool->ready), NULL) != 0)
		return -1;
	
	for (int i = 0; i < num_threads; i++) {
		pthread_create(&thread_pool->all_ids[i], NULL, run_thread, (void*)thread_pool);
	}
	(*pool) = thread_pool;
	
	return 0;
}

// destroy a pool of threads when finished
void end_thread_pool(thread_pool_t* thread_pool) {
	thread_task_t* task;
	// cancel if pool is already closed
	if (thread_pool->is_close)
		return;	
	thread_pool->is_close = true;
	pthread_mutex_lock(&thread_pool->lock);
	pthread_cond_broadcast(&thread_pool->ready);
	pthread_mutex_unlock(&thread_pool->lock);
	
	// wait until all threads in the pool finish
	for (int i = 0; i < thread_pool->num_threads; i++) {
		pthread_join(thread_pool->all_ids[i], NULL);
	}
	
	// free everything in pool
	free(thread_pool->all_ids);
	while(thread_pool->head) {
		task = thread_pool->head;
		thread_pool->head = (thread_task_t *) thread_pool->head->next;
		free(task);
	}
	pthread_mutex_destroy(&thread_pool->lock);
	pthread_cond_destroy(&thread_pool->ready);
	free(thread_pool);
}

int add_task_to_pool(thread_pool_t* thread_pool, void *(*task_func)(void *), void* args) {
	thread_task_t* task;
	if (!task_func) 
		return -1;
	
	task = (thread_task_t*) malloc(sizeof(thread_task_t));
	task->run_thread = task_func;
	task->args = args;
	task->next = NULL;

	//add the task to pool (append to tail or start on head if empty)
	pthread_mutex_lock(&thread_pool->lock);
	if (!thread_pool->head) {
		thread_pool->head = task;
	}
	else {
		thread_pool->tail->next = task;
	}
	thread_pool->tail = task;
	pthread_cond_signal(&thread_pool->ready);
	pthread_mutex_unlock(&thread_pool->lock);
	
	return 0;
}
// to be used in external sort
struct file_block {
	long file_size;
	char* input;
	char* output;
	int id;
	size_t start;
};

// to be used in internal sort
struct internal_block {
	long count;
	char*** buffer;
	char* input;
	long start;
};

void* check(void* args) {
	thread_pool_t* thread_pool = (thread_pool_t*)args;
	while (true) {
		sleep(3);
		pthread_mutex_lock(&thread_pool->lock);
		if (!thread_pool->head) {
			pthread_mutex_unlock(&thread_pool->lock);
			end_thread_pool(thread_pool);
			break;
		}
		pthread_mutex_unlock(&thread_pool->lock);
	}
	
	return NULL;
}

int check_thread(thread_pool_t* thread_pool) {
	thread_pool->check_thread = (pthread_t) malloc(sizeof(pthread_t));
	pthread_create(&thread_pool->check_thread, NULL, check, (void*)thread_pool);
	pthread_join(thread_pool->check_thread, NULL);
	return 0; 
}


int compare(const void *a, const void *b) {

	return (strcmp(*(char**)a, *(char**)b));
}

void* internal_sort(void* args) {
	struct internal_block *block = (struct internal_block*) args;
	char** buffer = *(block->buffer);
	FILE* file_read = open_file_to_read(block->input);
	fseek(file_read, block->start * BUFFER_SIZE, SEEK_SET);
	for (long i = 0; i < block->count; i++) {
		long index = block->start + i;
		fread(&buffer[index][0], BUFFER_SIZE, 1, file_read);
	}
	fclose(file_read);
	return NULL;
}

void* external_sort(void* args) {
	char temp_file_name[12];
	struct file_block *block = (struct file_block*) args;
	// open file to read from and file to write to
	// set pointer to start for new batch
	FILE* file_write;
	FILE* file_read = open_file_to_read(block->input);
	snprintf(temp_file_name, 12, "%s%i", temp_file_prefix, block->id);
	file_write = open_file_to_write(temp_file_name);
	fseek(file_read, block->start, SEEK_SET);
	long count = block->file_size / BUFFER_SIZE;

	// allocate buffer to read from file
	char** buffer;
	buffer = (char**) malloc(count * sizeof(char*));
	for (int i=0; i < count; i++) {
		buffer[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
	}
	
	// read from file and close
	for (int i = 0; i < count; i++) {
		fread(&buffer[i][0], BUFFER_SIZE, 1, file_read);
	}
	fclose(file_read);
	
	// use the built-in quicksort function
	qsort(buffer, count, sizeof(char*), compare);
	
	// write sorted data
	for (int i = 0; i < count; i++) {
		fwrite(&buffer[i][0], BUFFER_SIZE, 1, file_write);
	}
	
	// free buffer and close file
	for (int i = 0; i < count; i++) {
		free(buffer[i]);
	}
	free(buffer);
	fclose(file_write);
	
	return NULL;
}

bool is_finish(long* seek, int c) {
	for (int i = 0; i < c; i++) {
		if (seek[i] >= 0)
			return false;
	}
	return true;
}

void compare_write(char** buffer, char** min_so_far, long* seek, int count, FILE** file_read, FILE* file_write, long* file_read_count, char* file_write_final_name) {
	char* min = *min_so_far;
	int min_i;
	for (int i = 0; i < count; i++) {
		if (seek[i] != -1) {
			min_i = i;
			strcpy(min, buffer[i]);
			break;
		}
	}

	for (int i = 0; i < count; i++) {
		if (seek[i] == -1)
			continue;
		if (strcmp((const char*) min, (const char*) buffer[i]) > 0) {
			strcpy(min, buffer[i]);
			min_i = i;
		}
	}
	
	if (seek[min_i] != -1 && seek[min_i] < file_read_count[min_i]) {
		seek[min_i]++;
	}
	else {
		seek[min_i] = -1;
	}
	
	fwrite(min, BUFFER_SIZE, 1, file_write);
	
	if (is_finish(seek, count))
		return;
	if (seek[min_i] != -1) {
		if(fseek(file_read[min_i], seek[min_i] * BUFFER_SIZE, SEEK_SET) == 0) {
			fread(&buffer[min_i][0], BUFFER_SIZE, 1, file_read[min_i]);
		}
	}
}


void* merge_files(char* file_write_final_name, int count, long sum, int num_threads) {
	FILE* file_read[count];
	FILE* file_write = open_file_to_write(file_write_final_name);
	long file_read_count[count];
	long seek[count];
	char** buffer;
	
	for (int i = 0; i < count; i++) {
		char file_name[12];
		snprintf(file_name, 12, "%s%i", temp_file_prefix, i);
		file_read[i] = open_file_to_read(file_name);
		file_read_count[i] = ceil(check_file_size(file_name) / BUFFER_SIZE);
		seek[i] = 0;
	}
	
	buffer = (char**) malloc(count * sizeof(char*));
	for (int i = 0; i < count; i++) {
		buffer[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
		fread(&buffer[i][0], BUFFER_SIZE, 1, file_read[i]);
	}
	
	char* min = (char*) malloc(BUFFER_SIZE * sizeof(char));
	for (int i = 0; i < sum; i++) {
		compare_write(buffer, &min, seek, count, file_read, file_write, file_read_count, file_write_final_name);
	}
	
	// free buffer and close file write
	for (int i = 0; i < count; i++) {
		free(buffer[i]);
	}
	free(buffer);
	fclose(file_write);
	
	return NULL;
}

void mysort(char* input_file_name, char* output_file_name, int num_threads) {
	long file_size = check_file_size(input_file_name);
	thread_pool_t* thread_pool = NULL;
	if (start_thread_pool(&thread_pool, num_threads) != 0) {
		printf("thread pool start failed.\n");
		return;
	}
	
	if (file_size <= MEMORY_LIMIT) {
		struct internal_block *blocks = (struct internal_block *) malloc(sizeof(struct internal_block) * num_threads);
		long num_lines = file_size / BUFFER_SIZE;
		char** buffer = (char**) malloc(num_lines * sizeof(char*));
		for (long i = 0; i < num_lines; i++) {
			buffer[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
		}
		
		long num_line_per_thread = num_lines / num_threads;
		for (long i = 0; i < num_threads; i++) {
			blocks[i].count = num_line_per_thread;
			blocks[i].start = num_line_per_thread * i;
			blocks[i].input = input_file_name;
			blocks[i].buffer = &buffer;

			add_task_to_pool(thread_pool, internal_sort, (void*) &blocks[i]);
		}
	
		check_thread(thread_pool);

		qsort(buffer, num_lines, sizeof(char*), compare);
		FILE* file_write = open_file_to_write(output_file_name);
		for (long i = 0; i < num_lines; i++) {
			fwrite(&buffer[i][0], BUFFER_SIZE, 1, file_write);
		}
			
		// free buffer and close file write
		for (long i = 0; i < num_lines; i++) {
			free(buffer[i]);
		}
		fclose(file_write);
	}
	else {
		long memory_per_thread = MEMORY_LIMIT / num_threads;
		int count = ceil(file_size / memory_per_thread);
		struct file_block *blocks = (struct file_block*) malloc(sizeof(struct file_block) * count);
		for (int i = 0; i < count; i++) {
			blocks[i].file_size = memory_per_thread;
			blocks[i].start = i * memory_per_thread;
			blocks[i].id = i;
			blocks[i].input = input_file_name;
			
			add_task_to_pool(thread_pool, external_sort, (void*) &blocks[i]);
		}
		
		check_thread(thread_pool);
		merge_files(output_file_name, count, file_size / BUFFER_SIZE, num_threads);
	}
}

int main(int argc, char** argv) {
	char* input_file_name;
	char* output_file_name;
	int num_threads;
	struct timeval start, end;
	double executionTime;

	if (argc != 4) {
		fprintf(stderr, MSG);
		return 1;
	}
	
	input_file_name = argv[1];
	output_file_name = argv[2];
	num_threads = atoi(argv[3]);
	
	if (!(num_threads > 0)) return 1;
	
	// sort and time
	gettimeofday(&start, NULL);
	mysort(input_file_name, output_file_name, num_threads);
	gettimeofday(&end, NULL);
	executionTime = (double) (end.tv_sec - start.tv_sec)
					+ ((double) end.tv_usec - (double) start.tv_usec) / 1000000;
	printf("INPUT: %s\n OUTPUT: %s\n THREADS: %d\n EXEC_TIME: %lf\n", input_file_name, output_file_name, num_threads, executionTime);
	
	return 0;
}