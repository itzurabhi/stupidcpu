#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <array>
#include <bitset>

using Register = uint32_t;

enum class Opcodes;
enum class RegisterIndex : uint8_t;

struct Instruction;
struct CPU;

void SetRegisterValue(CPU &cpu, RegisterIndex, uint32_t);
void SetRegisterValue(CPU &cpu, RegisterIndex, RegisterIndex);
uint32_t GetRegisterValue(const CPU &, RegisterIndex);

enum class Opcodes
{
    Nop,
    Add,
    Sub,
    Mul,
    Mov,
    Ret,
    Call
};
struct Instruction
{
    Opcodes opcode;
    // Register index
    RegisterIndex lRegisterIndex, rRegisterIndex;
    uint32_t lValue, rValue;
};

using Memory = std::vector<Instruction>;

enum class RegisterIndex : uint8_t
{
    None,
    IP,
    SP,
    GRA,
    GRB,
    GRC,
    GRD
};

struct CPU
{
    Register IP, SP, GRA, GRB, GRC, GRD;
    Memory memory;
};

constexpr Instruction createInstr(Opcodes op, RegisterIndex leftRegister, RegisterIndex rightRegister, uint32_t lImmediate, uint32_t rImmediate) noexcept
{
    return Instruction{op, leftRegister, rightRegister, lImmediate, rImmediate};
}

void Reset(CPU &theCPU)
{
    theCPU.IP = 0;
    theCPU.SP = 0;
    theCPU.GRA = 0;
    theCPU.GRB = 0;
    theCPU.GRC = 0;
    theCPU.GRD = 0;
}

void PrintState(const CPU &theCPU)
{
    printf("Registers :\tIP : %d\tSP : %d\tGRA : %d\tGRB : %d\tGRC : %d\tGRD : %d\n", theCPU.IP, theCPU.SP, theCPU.GRA, theCPU.GRB, theCPU.GRC, theCPU.GRD);
}

void Execute(const Instruction &instr, CPU &cpu)
{
    // printf("OpCode %d \n", (uint8_t)instr.opcode);
    switch (instr.opcode)
    {
    case Opcodes::Nop:
        // Does nothing..Just success
        return;
    case Opcodes::Mov:
    {
        // MOV GRA,GRB
        if (instr.lRegisterIndex != RegisterIndex::None && instr.rRegisterIndex != RegisterIndex::None)
        {
            // printf("Moving %d to regiser index %d",)
            SetRegisterValue(cpu, instr.lRegisterIndex, instr.rRegisterIndex);
        }
        // MOV GRA,0x1000
        else if (instr.lRegisterIndex != RegisterIndex::None)
        {
            SetRegisterValue(cpu, instr.lRegisterIndex, instr.lValue);
        }
    }
    break;
    case Opcodes::Add:
    {
        // ADD GRA,GRB
        if (instr.lRegisterIndex != RegisterIndex::None && instr.rRegisterIndex != RegisterIndex::None)
        {
            SetRegisterValue(cpu, instr.lRegisterIndex, GetRegisterValue(cpu, instr.lRegisterIndex) + GetRegisterValue(cpu, instr.rRegisterIndex));
        }
        // ADD GRA,0x1000
        else if (instr.lRegisterIndex != RegisterIndex::None)
        {
            SetRegisterValue(cpu, instr.lRegisterIndex, GetRegisterValue(cpu, instr.lRegisterIndex) + instr.rValue);
        }
        // ADD 0x1000,GRB
        else if (instr.rRegisterIndex != RegisterIndex::None)
        {
            SetRegisterValue(cpu, instr.lRegisterIndex, GetRegisterValue(cpu, instr.rRegisterIndex) + instr.lValue);
        }
    }
    break;
    default:
        printf("Illegal instruction at %d\n", cpu.IP - 1);
        return;
    };
}

void SetRegisterValue(CPU &cpu, RegisterIndex dest, uint32_t immediate)
{

    // printf("Set register index %d to immediate %d\n", (uint8_t)dest, immediate);
    switch (dest)
    {
    case RegisterIndex::IP:
        cpu.IP = immediate;
        break;
    case RegisterIndex::SP:
        cpu.SP = immediate;
        break;
    case RegisterIndex::GRA:
        cpu.GRA = immediate;
        break;
    case RegisterIndex::GRB:
        cpu.GRB = immediate;
        break;
    case RegisterIndex::GRC:
        cpu.GRC = immediate;
        break;
    case RegisterIndex::GRD:
        cpu.GRD = immediate;
        break;

    default:
        break;
    }
}

uint32_t GetRegisterValue(const CPU &cpu, RegisterIndex index)
{
    switch (index)
    {
    case RegisterIndex::IP:
        return cpu.IP;
    case RegisterIndex::SP:
        return cpu.SP;
    case RegisterIndex::GRA:
        return cpu.GRA;
    case RegisterIndex::GRB:
        return cpu.GRB;
    case RegisterIndex::GRC:
        return cpu.GRC;
    case RegisterIndex::GRD:
        return cpu.GRD;
    default:
        return 0;
    }
}

void SetRegisterValue(CPU &cpu, RegisterIndex dest, RegisterIndex src)
{
    // printf("Set register index %d to register %d\n", (uint8_t)dest, (uint8_t)src);
    switch (dest)
    {
    case RegisterIndex::IP:
        cpu.IP = GetRegisterValue(cpu, src);
        break;
    case RegisterIndex::SP:
        cpu.SP = GetRegisterValue(cpu, src);
        break;
    case RegisterIndex::GRA:
        cpu.GRA = GetRegisterValue(cpu, src);
        break;
    case RegisterIndex::GRB:
        cpu.GRB = GetRegisterValue(cpu, src);
        break;
    case RegisterIndex::GRC:
        cpu.GRC = GetRegisterValue(cpu, src);
        break;
    case RegisterIndex::GRD:
        cpu.GRD = GetRegisterValue(cpu, src);
        break;

    default:
        break;
    }
}

main(int argc, char const *argv[])
{

    CPU theCPU;

    // set 12 to register GRA
    theCPU.memory.emplace_back(createInstr(Opcodes::Mov, RegisterIndex::GRA, RegisterIndex::None, 12, 0));

    // set 23 to register GRB
    theCPU.memory.emplace_back(createInstr(Opcodes::Mov, RegisterIndex::GRB, RegisterIndex::None, 23, 0));

    // multiply GRA and GRB and place result in GRA
    theCPU.memory.emplace_back(createInstr(Opcodes::Add, RegisterIndex::GRA, RegisterIndex::GRB, 0, 0));

    Reset(theCPU);

    while (theCPU.IP < theCPU.memory.size())
    {
        printf("BEFORE EXECUTION\n");
        PrintState(theCPU);
        const auto instr = theCPU.memory[theCPU.IP];
        theCPU.IP += 1;
        Execute(instr, theCPU);
        printf("AFTER EXECUTION\n");
        PrintState(theCPU);

        printf("Press [ENTER] to continue execution");
        std::cin.get();
    }
    printf("CPU Halted!\n");
    return 0;
}
