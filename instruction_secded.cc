#include "instruction_secded.hh"

Instruction_SECDED::Instruction_SECDED(uint32_t instruction, uint64_t offset)
{
	this->secded = SECDED(instruction);
	this->offset = offset;
	this->original_secded = this->secded.clone();
}

std::string Instruction_SECDED::to_string()
{
	std::stringstream ss;
	ss << "Should be " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->original_secded.secded << " "
										 << "0x" << std::setfill('0') << std::setw(8) << std::hex << this->original_secded.instruction << std::endl;
	ss << "Is " << "0x" << std::setfill('0') << std::setw(2) << std::hex << (uint32_t)this->secded.secded << " "
							<< "0x" << std::setfill('0') << std::setw(8) << std::hex << this->secded.instruction << std::endl;
	if(!this->faults_locations.size())
	{
		ss << "Faults (0)." << std::endl;
	}
	else
	{
		ss << "Faults (" << std::dec << this->faults_locations.size() << "):";
		for(uint8_t i : this->faults_locations)
		{
			ss << " " << std::dec << (uint32_t)i;
		}
		ss << std::endl;
	}
	return ss.str();
}

uint32_t Instruction_SECDED::inject_fault(uint8_t location)
{
	this->faults_locations.insert(location);
	if(location < 32) this->secded.instruction ^= 0x1 << location;
	else this->secded.secded ^= 0x1 << (location - 32);
	return 1;
}

Instruction_SECDED::~Instruction_SECDED()
{
	
}
