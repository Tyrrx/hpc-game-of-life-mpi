echo "------------------- 1 Threads -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 1 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 2048 1 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 1 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 2048 1 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 1 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 2048 1 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 1 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 2048 1 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 1 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 2048 1 1 >> run4out.txt 2>&1

echo "------------------- 2 Threads 2,1 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 2 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 2048 2 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 2 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 2048 2 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 2 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 2048 2 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 2 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 2048 2 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 2 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 2048 2 1 >> run4out.txt 2>&1

echo "------------------- 4 Threads 2,2 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 1024 2 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 1024 2 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 1024 2 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 1024 2 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 1024 2 2 >> run4out.txt 2>&1

echo "------------------- 4 Threads 1,4 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 512 1 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 512 1 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 512 1 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 512 1 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 512 1 4 >> run4out.txt 2>&1

echo "------------------- 4 Threads 4,1 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 2048 4 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 2048 4 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 2048 4 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 2048 4 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 4 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 2048 4 1 >> run4out.txt 2>&1

echo "------------------- 6 Threads 3,2 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 682 1024 3 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 682 1024 3 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 682 1024 3 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 682 1024 3 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 682 1024 3 2 >> run4out.txt 2>&1

echo "------------------- 6 Threads 2,3 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 682 2 3 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 682 2 3 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 682 2 3 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 682 2 3 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 682 2 3 >> run4out.txt 2>&1

echo "------------------- 6 Threads 6,1 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 341 2048 6 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 341 2048 6 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 341 2048 6 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 341 2048 6 1 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 341 2048 6 1 >> run4out.txt 2>&1

echo "------------------- 6 Threads 1,6 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 341 1 6 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 341 1 6 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 341 1 6 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 341 1 6 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 6 ./cmake-build-debug/hpc_game_of_life_mpi 250 2048 341 1 6 >> run4out.txt 2>&1

echo "------------------- 8 Threads 4,2 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 1024 4 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 1024 4 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 1024 4 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 1024 4 2 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 512 1024 4 2 >> run4out.txt 2>&1

echo "------------------- 8 Threads 2,4 -------------------" >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 512 2 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 512 2 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 512 2 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 512 2 4 >> run4out.txt 2>&1
/bin/time -f "Time:%E,Memory:%M\n" mpirun --oversubscribe -n 8 ./cmake-build-debug/hpc_game_of_life_mpi 250 1024 512 2 4 >> run4out.txt 2>&1
