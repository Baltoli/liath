#include <fstream>
#include <iostream>
#include <string>

#include "pin.H"

KNOB<std::string> OutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "-",
                             "Specify memory trace output file");

KNOB<std::string> SymbolName(
    KNOB_MODE_WRITEONCE, "pintool", "s", "-",
    "Specify symbol to trace (whole app if not specified)");

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

void instrument_instruction(INS ins) {
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

void on_instruction(INS ins, void *) { instrument_instruction(ins); }

void instrument_symbol(IMG img, void *sym) {
  auto symbol_name = static_cast<char *>(sym);

  for (auto sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec)) {
    for (auto rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn)) {
      auto sym = RTN_Sym(rtn);

      if (SYM_Valid(sym)) {
        auto mangled = SYM_Name(sym);
        auto name = PIN_UndecorateSymbolName(mangled, UNDECORATION_NAME_ONLY);

        if (name == symbol_name) {
          RTN_Open(rtn);

          for (auto ins = RTN_InsHead(rtn); INS_Valid(ins);
               ins = INS_Next(ins)) {
            instrument_instruction(ins);
          }

          RTN_Close(rtn);
        }
      }
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

  PIN_InitSymbols();

  if (SymbolName.Value() == "-") {
    INS_AddInstrumentFunction(on_instruction, nullptr);
  } else {
    IMG_AddInstrumentFunction(instrument_symbol,
                              (void *)SymbolName.Value().c_str());
  }

  PIN_AddFiniFunction(finalize, nullptr);

  PIN_StartProgram();
}
