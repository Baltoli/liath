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

void on_instruction(INS ins, void *) {}

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
