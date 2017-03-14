#include "fault_injector.hh"

namespace fault_injector
{
	uint32_t inject_fault(uint32_t num_bitflips, Instruction_SECDED * inst)
	{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, CODEWORD_SIZE - 1);
		for(uint32_t i = 0; i < num_bitflips; i++)
		{
			uint8_t index = 0;
			//randomly flip a bit in the 39bit codeword, do not flip the same bit twice
			do
			{
				index = dist(rng);
			} while((std::find((*inst).faults_locations.begin(), (*inst).faults_locations.end(), index) != (*inst).faults_locations.end()));
			(*inst).inject_fault(index);
		}
		return num_bitflips;
	}

	uint32_t inject_faults(uint32_t instructions_to_affect, uint32_t bitflips_per_instruction, std::vector<Instruction_SECDED>* instructions)
	{
		uint32_t counter = 0;
		std::set<uint64_t> indexes;

    std::mt19937_64 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937_64::result_type> dist(0, (*instructions).size() - 1);

		//randomly select which unique instructions to affect
		//do not affect the same instruction twice
		for(uint32_t i = 0; i < instructions_to_affect; i++)
		{
			uint64_t index = 0;
			do
			{
				index = dist(rng);
			} while((std::find(indexes.begin(), indexes.end(), index) != indexes.end()));
			indexes.insert(index);
			std::cout << "Injecting at " << std::dec << index << std::endl;
	 		counter += inject_fault(bitflips_per_instruction, &((*instructions)[index]));
		}
		return counter;
	}
}