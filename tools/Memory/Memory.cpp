#include "pin.H"

#include <fstream>
#include <iostream>
#include <string>

KNOB<std::string> OutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "-",
                             "Specify memory trace output file");

std::ofstream file_out;

std::ostream &tool_out() {
  if (OutputFile.Value() == "-") {
    return std::cout;
  } else {
    if (!file_out.is_open()) {
      file_out.open(OutputFile.Value().c_str());
    }

    return file_out;
  }
}

void trace_read(void *ins_ptr, void *addr, int32_t size) {
  tool_out() << "R," << addr << "," << size << '\n';
}

void trace_write(void *ins_ptr, void *addr, int32_t size) {
  tool_out() << "W," << addr << "," << size << '\n';
}

void on_instruction(INS ins, void *) {
  auto operands = INS_MemoryOperandCount(ins);
  for (auto mem_op = 0u; mem_op < operands; ++mem_op) {
    if (INS_MemoryOperandIsRead(ins, mem_op)) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_read,
                               IARG_INST_PTR, IARG_MEMORYOP_EA, mem_op,
                               IARG_MEMORYREAD_SIZE, IARG_END);
    }

    if (INS_MemoryOperandIsWritten(ins, mem_op)) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_write,
                               IARG_INST_PTR, IARG_MEMORYOP_EA, mem_op,
                               IARG_MEMORYWRITE_SIZE, IARG_END);
    }
  }
}

void finalize(int32_t code, void *) {
  if (file_out.is_open()) {
    file_out.close();
  }
}

int main(int argc, char **argv) {
  if (PIN_Init(argc, argv)) {
    return 1;
  }

  INS_AddInstrumentFunction(on_instruction, nullptr);
  PIN_AddFiniFunction(finalize, nullptr);

  PIN_StartProgram();
}
