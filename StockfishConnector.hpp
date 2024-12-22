#ifndef STOCKFISH_CONNECTOR_HPP
#define STOCKFISH_CONNECTOR_HPP

#include <string>

bool initializeStockfish();
std::string getBestMove(const std::string& position);
void closeStockfish();

#endif // STOCKFISH_CONNECTOR_HPP
