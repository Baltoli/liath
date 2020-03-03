#include "pin.H"

#include <fstream>
#include <iostream>
#include <map>

std::map<ADDRINT, bool> MallocMap;

std::ofstream LogFile;

KNOB<std::string> LogFileName(KNOB_MODE_WRITEONCE, "pintool", "o",
                              "memprofile.out", "Memory trace file name");

void LogBeforeMalloc(ADDRINT size) {
  LogFile << "[*] malloc(" << std::dec << size << ")\n";
}

void LogAfterMalloc(ADDRINT addr) {
  if (addr == 0x0) {
    std::cerr << "[-] Error: malloc() return value was NULL. Heap full!?!";
    return;
  }

  auto it = MallocMap.find(addr);
  if (it != MallocMap.end()) {
    if (it->second) {
      it->second = false;
    } else {
      std::cerr << "[-] Error: allocating memory not freed!?!\n" << std::endl;
    }
  } else {
    MallocMap.insert({addr, false});
    LogFile << "\t\t= 0x" << std::hex << addr << std::endl;
  }
}

void LogFree(ADDRINT addr) {
  auto it = MallocMap.find(addr);
  if (it != MallocMap.end()) {
    if (it->second) {
      LogFile << "[*] Multiple free at address 0x" << std::hex << addr << '\n';
    } else {
      it->second = true;
      LogFile << "[*] free(0x" << std::hex << addr << ")" << std::endl;
    }
  } else {
    LogFile << "[*] Freeing unallocated memory at address 0x" << std::hex
            << addr << '\n';
  }
}

void CustomInstrumentation(IMG img, void *v) {
  for (SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym)) {
    auto undFuncName =
        PIN_UndecorateSymbolName(SYM_Name(sym), UNDECORATION_NAME_ONLY);

    if (undFuncName == "malloc") {
      auto allocRtn = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

      if (RTN_Valid(allocRtn)) {
        RTN_Open(allocRtn);

        RTN_InsertCall(allocRtn, IPOINT_BEFORE, (AFUNPTR)LogBeforeMalloc,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);

        RTN_InsertCall(allocRtn, IPOINT_AFTER, (AFUNPTR)LogAfterMalloc,
                       IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);

        RTN_Close(allocRtn);
      }
    } else if (undFuncName == "free") {
      auto freeRtn = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));

      if (RTN_Valid(freeRtn)) {
        RTN_Open(freeRtn);

        RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)LogFree,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);

        RTN_Close(freeRtn);
      }
    }
  }
}

void FinalFunc(int32_t code, void *v) {
  for (auto p : MallocMap) {
    if (!p.second)
      LogFile << "[*] Memory at address 0x" << std::hex << p.first
              << " allocated but not freed" << std::endl;
  }

  LogFile.close();
}
int main(int argc, char **argv) {
  PIN_InitSymbols();
  PIN_Init(argc, argv);

  LogFile.open(LogFileName.Value().c_str());
  IMG_AddInstrumentFunction(CustomInstrumentation, nullptr);

  PIN_AddFiniFunction(FinalFunc, nullptr);
  PIN_StartProgram();

  return 0;
}
