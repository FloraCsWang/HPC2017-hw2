linker: omp_solved2.c omp_solved3.c omp_solved4.c omp_solved5.c omp_solved6.c jacobi2D-omp.cpp gs2D-omp.cpp
	gcc  -fopenmp omp_solved2.c -o omp_solved2
	gcc  -fopenmp omp_solved3.c -o omp_solved3
	gcc  -fopenmp omp_solved4.c -o omp_solved4
	gcc  -fopenmp omp_solved5.c -o omp_solved5
	gcc  -fopenmp omp_solved6.c -o omp_solved6
	g++  -fopenmp jacobi2D-omp.cpp -o jacobi2D-omp
	g++  -fopenmp gs2D-omp.cpp -o gs2D-omp


clean:
	rm -f  omp_solved2 *~
	rm -f  omp_solved3 *~
	rm -f  omp_solved4 *~
	rm -f  omp_solved5 *~
	rm -f  omp_solved6 *~
	rm -f  jacobi2D-omp *~
	rm -f  gs2D-omp