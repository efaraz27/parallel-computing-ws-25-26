#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

double g_apprx1 = 0.0;
double g_apprx2 = 0.0;
double g_apprx3 = 0.0;
double g_apprx4 = 0.0;
long long g_nt_global = 0;
const int g_num_threads = 4;

void *calculate_pi_segment(void *arg) {
  long thread_id = (long)arg;

  long long block_size = g_nt_global / g_num_threads;
  long long remainder = g_nt_global % g_num_threads;

  long long start_i = 0;
  for (int i = 1; i < thread_id; i++) {
    start_i += block_size;
    if (i <= remainder) {
      start_i++;
    }
  }

  long long current_block_size = block_size;
  if (thread_id <= remainder) {
    current_block_size++;
  }
  long long end_i = start_i + current_block_size;

  double local_apprx = 0.0;
  long long n = start_i;

  int sign = (n % 2 == 0) ? 1 : -1;
  double denominator = 2.0 * n + 1.0;

  for (; n < end_i; n++) {
    local_apprx += (double)sign / denominator;
    denominator += 2.0;
    sign *= -1;
  }

  if (thread_id == 1)
    g_apprx1 = local_apprx;
  else if (thread_id == 2)
    g_apprx2 = local_apprx;
  else if (thread_id == 3)
    g_apprx3 = local_apprx;
  else if (thread_id == 4)
    g_apprx4 = local_apprx;

  return NULL;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    return 1;
  }

  g_nt_global = atoll(argv[1]);

  pthread_t t1, t2, t3, t4;

  // Create Threads, passing the thread ID (1-4) as the argument
  if (pthread_create(&t1, NULL, calculate_pi_segment, (void *)1) != 0) {
    perror("pthread_create failed for t1");
    return 1;
  }
  if (pthread_create(&t2, NULL, calculate_pi_segment, (void *)2) != 0) {
    perror("pthread_create failed for t2");
    return 1;
  }
  if (pthread_create(&t3, NULL, calculate_pi_segment, (void *)3) != 0) {
    perror("pthread_create failed for t3");
    return 1;
  }
  if (pthread_create(&t4, NULL, calculate_pi_segment, (void *)4) != 0) {
    perror("pthread_create failed for t4");
    return 1;
  }

  // Join Threads
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  // Aggregate Results from global variables
  double total_apprx = g_apprx1 + g_apprx2 + g_apprx3 + g_apprx4;

  double pi_approximation = total_apprx * 4.0;

  printf("%lld\n", g_nt_global);
  printf("%d\n", g_num_threads);
  printf("%f\n", pi_approximation);

  return 0;
}
