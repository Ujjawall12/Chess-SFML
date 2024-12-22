#include "StockfishConnector.hpp"
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    FILE* stockfishProcess = nullptr;

    std::string executeCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;

        #ifdef _WIN32
        if (stockfishProcess) {
            fprintf(stockfishProcess, "%s\n", command.c_str());
            fflush(stockfishProcess);
            while (fgets(buffer.data(), buffer.size(), stockfishProcess) != nullptr) {
                result += buffer.data();
                if (result.find("bestmove") != std::string::npos) {
                    break;
                }
            }
        }
        #else
        // Implementation for Unix-based systems
        #endif

        return result;
    }
}

bool initializeStockfish() {
    #ifdef _WIN32
    stockfishProcess = _popen("stockfish.exe", "w+");
    if (!stockfishProcess) {
        std::cerr << "Failed to start Stockfish!" << std::endl;
        return false;
    }
    executeCommand("uci");
    return true;
    #else
    // Implementation for Unix-based systems
    #endif
}

std::string getBestMove(const std::string& position) {
    if (!stockfishProcess) {
        std::cerr << "Stockfish is not initialized!" << std::endl;
        return "";
    }
    executeCommand("position fen " + position);
    executeCommand("go depth 10");
    std::string output = executeCommand("");
    size_t pos = output.find("bestmove");
    if (pos != std::string::npos) {
        return output.substr(pos + 9, 4); // Extract move in UCI format
    }
    return "";
}

void closeStockfish() {
    if (stockfishProcess) {
        #ifdef _WIN32
        fprintf(stockfishProcess, "quit\n");
        fflush(stockfishProcess);
        _pclose(stockfishProcess);
        stockfishProcess = nullptr;
        #else
        // Implementation for Unix-based systems
        #endif
    }
}
