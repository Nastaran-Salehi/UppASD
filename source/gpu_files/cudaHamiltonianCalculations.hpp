#ifndef __CUDA_HAMILTONIAN_CALCULATIONS_HPP__
#define __CUDA_HAMILTONIAN_CALCULATIONS_HPP__

#include <cuda_runtime.h>

#include "real_type.h"

#include "cudaParallelizationHelper.hpp"

#include "hostMatrix.hpp"
#include "cudaMatrix.hpp"

class CudaHamiltonianCalculations {
private:
	// Local matrices
	typedef struct Exchange {
		unsigned int               mnn;
		cudaMatrix<real,2>         coupling;
		cudaMatrix<unsigned int,1> neighbourCount;
		cudaMatrix<unsigned int,2> neighbourPos;
	} Exchange;

	typedef struct DMinteraction {
		unsigned int               mnn;
		cudaMatrix<real,3,3>       interaction;
		cudaMatrix<unsigned int,1> neighbourCount;
		cudaMatrix<unsigned int,2> neighbourPos;
	} DMinteraction;

	typedef struct TensorialExchange {
		unsigned int               mnn;
		cudaMatrix<real,4,3,3>       tensor;
		cudaMatrix<unsigned int,1> neighbourCount;
		cudaMatrix<unsigned int,2> neighbourPos;
	} TensorialExchange;

	Exchange      ex;
	DMinteraction dm;
	TensorialExchange tenEx;

	bool do_j_tensor = false;

	// Initiation flag
	bool initiated;

	// System size
	unsigned int N;

	// Parallelization helper
	CudaParallelizationHelper &parallel;


public:
	// Parallelization helpers
	class SetupNeighbourList;
	class SetupNeighbourListDM;
	class SetupNeighbourListExchangeTensor;
	class HeisgeJij;
	class HeisJijTensor;
	class HeisgeJijElement;

	// Constructor
	CudaHamiltonianCalculations();

	// Initiate
	bool initiate(
		const hostMatrix<real,2>         &ncoup, 
		const hostMatrix<unsigned int,2> &nlist,
		const hostMatrix<unsigned int,1> &nlistsize,
		const hostMatrix<real,3,3>       &dm_ncoup, 
		const hostMatrix<unsigned int,2> &dm_nlist,
		const hostMatrix<unsigned int,1> &dm_nlistsize,
		const int			 do_dm,
		const int  			 do_j_tensor,
		const hostMatrix<real,4,3,3> j_tensor); 

	// Initiated
	bool isInitiated() {return initiated;}

	// Release
	void release();


	// Calculate "heisge"
	void heisge(cudaMatrix<real,3,3> &beff, 
		const cudaMatrix<real,3,3> &emomM, 
		const cudaMatrix<real,3,3> &external_field);
};

#endif

