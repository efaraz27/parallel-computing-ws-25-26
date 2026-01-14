#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define PING_PONG_LIMIT 20

int main(int argc, char *argv[]) {
    int world_rank, world_size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        if (world_rank == 0) {
            fprintf(stderr, "Error: This program must be run with exactly 2 processes, but %d were provided.\n",
                    world_size);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
        MPI_Finalize();
        return -1;
    }

    int ping_pong_count = 0;
    const int partner_rank = (world_rank + 1) % 2;
    const int tag = 0;

    double start_time = MPI_Wtime();

    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank == ping_pong_count % 2) {
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, tag, MPI_COMM_WORLD);
            printf("Rank %d sent ping_pong_count %d to Rank %d\n",
                   world_rank, ping_pong_count, partner_rank);
            fflush(stdout);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank %d received ping_pong_count %d from Rank %d\n",
                   world_rank, ping_pong_count, partner_rank);
            fflush(stdout);
        }
    }

    double end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("Total ping-pong time for %d exchanges: %f seconds\n",
               PING_PONG_LIMIT, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}