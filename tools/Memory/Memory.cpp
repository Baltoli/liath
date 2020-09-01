#include <fstream>
#include <iostream>
#include <string>

#include "pin.H"

KNOB<std::string> OutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "-",
                             "Specify memory trace output file");

KNOB<std::string> SymbolName(
    KNOB_MODE_WRITEONCE, "pintool", "s", "-",
    "Specify symbol to trace (whole app if not specified)");

KNOB<bool> TraceReadValues(KNOB_MODE_WRITEONCE, "pintool", "v", "0",
                           "Trace the concrete values read from memory");

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

void trace_read(void *ins_ptr, void *addr, int32_t size, INS ins) {
  tool_out() << "R," << size << "," << (uintptr_t)addr << ","
             << (uintptr_t)ins_ptr << '\n';

  if (TraceReadValues.Value()) {
    if (size == 4) {
      int32_t i_val;
      float f_val;

      PIN_SafeCopy(&i_val, addr, size);
      PIN_SafeCopy(&f_val, addr, size);

      tool_out() << "[\n  int=" << std::dec << i_val << "\n  float=" << f_val
                 << "\n  ptr=0x" << std::hex << i_val << "\n]\n\n";
    } else if (size == 8) {
      int64_t i_val;
      double f_val;

      PIN_SafeCopy(&i_val, addr, size);
      PIN_SafeCopy(&f_val, addr, size);

      tool_out() << "[\n  int=" << std::dec << i_val << "\n  float=" << f_val
                 << "\n  ptr=0x" << std::hex << i_val << "\n]\n\n";
    } else {
      tool_out() << "  [unknown value]\n";
    }
  }
}

void trace_write(void *ins_ptr, void *addr, int32_t size, INS ins) {
  tool_out() << "W," << size << "," << (uintptr_t)addr << ","
             << (uintptr_t)ins_ptr << '\n';
}

void after_memop(void *ins_ptr) { tool_out() << "  next: " << ins_ptr << '\n'; }

void instrument_instruction(INS ins) {
  auto operands = INS_MemoryOperandCount(ins);
  for (auto mem_op = 0u; mem_op < operands; ++mem_op) {
    if (INS_MemoryOperandIsRead(ins, mem_op)) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_read,
                               IARG_INST_PTR, IARG_MEMORYOP_EA, mem_op,
                               IARG_MEMORYREAD_SIZE, IARG_PTR, ins, IARG_END);
    }

    if (INS_MemoryOperandIsWritten(ins, mem_op)) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_write,
                               IARG_INST_PTR, IARG_MEMORYOP_EA, mem_op,
                               IARG_MEMORYWRITE_SIZE, IARG_PTR, ins, IARG_END);
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

  tool_out() << "mode,addr,size\n";
  PIN_StartProgram();
}
